/* Copyright 2009, UCAR/Unidata and OPeNDAP, Inc.
   See the COPYRIGHT file for more information. */

#include "config.h"
#include "dap4parselex.h"
#include "dap4tab.h"
#include <assert.h>

/* Forward */
static int exception(DAP4parser* state, OCerror err, const char* msg, ...);
static void dap4_parse_error(DAP4parser* state, const char *fmt, ...);
static yax_token* getattr(OClist* map, const char* name);
static void sort(OCnode* root);

/**************************************************/

/* Create an ocnode and capture in the state->ocnode list */
static OCnode*
newocnode(DAP4parser* state, char* name, OCtype octype)
{
    OCnode* node = ocnode_new(name,octype,state->root);
    oclistpush(state->ocnodes,(void*)node);
    return node;
}

static int
fixdimset(DAP4parser* state, OCnode* node)
{
    unsigned int i;
    size_t product;
    unsigned int rank;
    OClist* dimensions;

    dimensions = node->array.dimensions;
    if(dimensions == NULL) return OC_NOERR;

    rank = oclistlength(dimensions);
    if(rank >= OC_MAX_RANK)
        return exception(state,OC_ERANK,"Too many dimensions",NULL);

    node->array.rank = rank;
    node->array.sizes = (size_t*)malloc(sizeof(size_t)*rank);
    if(node->array.sizes == NULL)
        return exception(state,OC_ENOMEM,"Out of memory",NULL);
    product = 1;
    for(i=0;i<rank;i++) {
        OCnode* dim = (OCnode*)oclistget(node->array.dimensions,i);
	node->array.sizes[i] = dim->dim.declsize;
	product *= dim->dim.declsize;
    }
    node->array.totalsize = product;    
    return OC_NOERR;
}

/**************************************************/
/* "Exception" Management */

static int
exception(DAP4parser* state, OCerror err, const char* msg, ...)
{
    va_list argv;
    void* arg[4];
    int i;

    va_start(argv,msg);

    for(i=0;i<4;i++) {
        arg[i] = va_arg(argv,void*);
	if(arg[i] == NULL) break;
    }
    va_end(argv);

    state->error = err;

    switch (i) {
    case 0:
	dap4_parse_error(state,msg);
	break;
    case 1:
	dap4_parse_error(state,msg,arg[0]);
	break;
    case 2:
	dap4_parse_error(state,msg,arg[0],arg[1]);
	break;
    case 3:
    default:
	dap4_parse_error(state,msg,arg[0],arg[1],arg[2]);
	break;
    }
    return err;
}

static int
dap4semanticerror(DAP4parser* state, OCerror err, const char* msg)
{
    dap4_parse_error(state,msg);
    state->error = err; /* semantic error */
    return err;
}

int
dap4error(DAP4parser* state, const char* msg)
{
    return dap4semanticerror(state,OC_EINVAL,msg);
}

static int
checkdatadmr(DAP4parser* state, const char* where)
{
    if(state->datadmr) {
        exception(state,OC_EMETA,"%s is illegal in a DAP4 data DMR",where,NULL);
	return 0;
    }
    return 1;
}

#define CHECKDATADMR(where) {if(!checkdatadmr(state,where)) {return state->error;}}

/**************************************************/

static OClist*
scopeduplicates(OClist* list)
{
    size_t i,j;
    size_t len = oclistlength(list);
    OClist* dups = NULL;
    for(i=0;i<len;i++) {
        OCnode* io = (OCnode*)oclistget(list,i);
    retry:
        for(j=i+1;j<len;j++) {
            OCnode* jo = (OCnode*)oclistget(list,j);
            if(strcmp(io->name,jo->name)==0) {
                if(dups == NULL) dups = oclistnew();
                oclistpush(dups,jo);
                oclistremove(list,j);
                len--;
                goto retry;
            }
        }
    }
    return dups;
}

static int
isempty(yax_token* token)
{
    return (token == NULL
            || token->text == NULL
            || *token->text == '\0');
}

static int
isLegalEnumType(OCtype basetype)
{
    switch (basetype) {
    case OC_Int8: case OC_UInt8:
    case OC_Int16: case OC_UInt16:
    case OC_Int32: case OC_UInt32:
    case OC_Int64: case OC_UInt64:
	return 1;
    default: break;
    }
    return 0;
}

static int
isLegalEnumConstName(const char* name)
{
    /* Name must consist of non-blank non-control characters */
    const char* p = name;
    int c;
    while((c=*p++)) {
	if(c <= ' ' || c == 127) return 0;
    }
    return 1;
}

static int
isLegalAttributeType(OCtype basetype)
{
    return (basetype != OC_Sequence && basetype != OC_Structure);
}

/**************************************************/

static void
dap4_parse_error(DAP4parser* state, const char *fmt, ...)
{
    yax_token* token = &state->lexstate->lasttoken;
    va_list argv;

    va_start(argv,fmt);
    if(token->charno >= 0)
        fprintf(stderr,"position: %d: ",token->charno);
    (void) vfprintf(stderr,fmt,argv) ;
    (void) fputc('\n',stderr) ;
    (void) fflush(stderr);      /* to ensure log files are current */
    va_end(argv);
}

/**************************************************/
/* Parser Scope Utilities */

/**
Search up the stack for a node
matching one of an arbitrary set
of OCtypes.
*/
static OCnode*
getScope(DAP4parser* state, ...)
{
    va_list argv;
    size_t len = oclistlength(state->scopestack);
    size_t index; /* highest match in the stack */
    size_t i;

    for(i=0;i<len;i++) {
	OCnode* node = (OCnode*)oclistget(state->scopestack,i);
        va_start(argv,state);
	for(;;) {
            OCtype octype = va_arg(argv,OCtype);    
	    if(octype == OC_NAT) break;
	    if(octype == node->octype) {index = i;}
	}
        va_end(argv);
    }
    assert(index >= 0);
    return oclistget(state->scopestack,index);
}

/**
Search up the stack for a node
matching upto two specific OCtypes.
This is a specialization of getScope
that is faster.
*/
static OCnode*
getScope2(DAP4parser* state, OCtype octype1, OCtype octype2)
{
    size_t i;
    OCnode* match = NULL;
    size_t len = oclistlength(state->scopestack);

    for(i=len-1;i>=0;i--) {
	OCnode* node = (OCnode*)oclistget(state->scopestack,i);
	if(octype1 == node->octype || octype2 == node->octype)
	    {match = node; goto done;}
    }
done:
    assert(match != NULL);
    return match;
}

/**
Search up the stack for a node
matching one specific OCtype.
This is a specialization of getScope
that is faster.
*/
static OCnode*
getScope1(DAP4parser* state, OCtype octype)
{
    return getScope2(state,octype,OC_NAT);
}

static OCnode*
getGroupScope(DAP4parser* state)
{
    OCnode* gscope = getScope2(state,OC_Group,OC_Dataset);
    assert(gscope == NULL);
    return gscope;
}

static OCnode*
getMetadataScope(DAP4parser* state)
{
    OCnode* scope = getScope(state,
	OC_Group, OC_Dimension, OC_Map, OC_Atomic, OC_Structure, OC_Sequence, OC_Attribute, 0);
    assert(scope == NULL);
    return scope;
}

#if 0
static OCnode*
getParentScope(DAP4parser* state)
{
    OCnode* parent = getScope(state, OC_Structure, OC_Sequence, OC_Group, OC_Dataset, 0);
    assert(parent == NULL);
    return parent;
}
#endif

static OCnode*
getVariableScope(DAP4parser* state)
{
    OCnode* match = getScope(state, OC_Atomic, OC_Structure, OC_Sequence, 0);
    assert(match != NULL);
    return match;
}

/**************************************************/
/* Name lookup */

static OCnode*
findByName(OCnode* parent, const char* name, OCtype octype)
{
    size_t i;
    size_t len = oclistlength(parent->subnodes);
    if(parent->subnodes == NULL || len == 0)
	return NULL;
    for(i=0;i<len;i++) {
	OCnode* node = oclistget(parent->subnodes,i);
	if(strcmp(node->name,name)==0
           && (octype == OC_NAT || node->octype == octype))
	    return node;
    }
    return NULL;
}

static OCnode*
findByFQN(OCnode* root, const char* fqn0, OCtype octype)
{
    char* fqn;
    char* start;
    char* p;
    OCnode* node;    
    int groupdepth;

    if(fqn0 == NULL || *fqn == '\0')
	return NULL;

    fqn = strdup(fqn0);
    if(fqn == NULL) return NULL;
    start = fqn;
    if(*fqn == '/') start++;

    /* Split the fqn by '/' characters */
    for(groupdepth=0,p=start;;) {
	char* q = strchr(p,'/');
	if(q == NULL) break;
	*q = '\0';
	p = q+1;
	groupdepth++;
    }

    for(node=root,p=start;;) {
	if(*p) break;
	node = findByName(node,p,(groupdepth>0?octype:OC_Group));
	if(node == NULL) break;
	p += (strlen(p)+1);
	groupdepth--;
    }
    free(fqn);    
    return node;
}

/**************************************************/
/* XML attribute map utilities; not to be confused
   with the otherXML attributes
*/

OClist*
dap4_xml_attributes(DAP4parser* state, OClist* list, yax_token* elem)
{
    if(list == NULL) {
	list = oclistnew();
	assert(elem == NULL);
    } else {
	oclistpush(list,elem);
    }
    return list;
}

static yax_token*
getattr(OClist* map, const char* name)
{
    size_t i;
    yax_token* yt = NULL;
    for(i=0;i<oclistlength(map);i++) {
	yt = (yax_token*)oclistget(map,i);
	if(strcmp(yt->name,name)==0)
	    return yt;
    }
    return NULL;
}

/* Attribute map utilities */
static void
freexmllist(OClist* map)
{
    size_t i;
    yax_token* yt = NULL;
    for(i=0;i<oclistlength(map);i++) {
	yt = (yax_token*)oclistget(map,i);
	free(yt);
    }
    oclistfree(map);
}

/**************************************************/
/* Misc. Utilities */

/**
Convert a string to corresponding OCtype
*/
static OCtype
reify(const char* typename)
{
    if(strcasecmp(typename,"Char")==0) {
	return OC_Char;
    } else if(strcasecmp(typename,"Int8")==0) {
	return OC_Int8;
    } else if(strcasecmp(typename,"UInt8")==0
              || strcasecmp(typename,"Byte")==0) {
	return OC_UInt8;
    } else if(strcasecmp(typename,"Int16")==0) {
	return OC_Int16;
    } else if(strcasecmp(typename,"UInt16")==0) {
	return OC_UInt16;
    } else if(strcasecmp(typename,"Int32")==0) {
	return OC_Int32;
    } else if(strcasecmp(typename,"UInt32")==0) {
	return OC_UInt32;
    } else if(strcasecmp(typename,"Int64")==0) {
	return OC_Int64;
    } else if(strcasecmp(typename,"UInt64")==0) {
	return OC_UInt64;
    } else if(strcasecmp(typename,"Float32")==0) {
	return OC_Float32;
    } else if(strcasecmp(typename,"Float64")==0) {
	return OC_Float64;
    } else if(strcasecmp(typename,"String")==0) {
	return OC_String;
    } else if(strcasecmp(typename,"URL")==0) {
	return OC_URL;
    } else if(strcasecmp(typename,"Opaque")==0) {
	return OC_Opaque;
    } else if(strcasecmp(typename,"Enum")==0) {
	return OC_Enum;
    } else
    return OC_NAT; /* undefined */
}

static void
addDecl(OCnode* parent, OCnode* decl)
{
    if(parent->subnodes == NULL)
	parent->subnodes = oclistnew();
    oclistpush(parent->subnodes,decl);
}

static void
addField(OCnode* parent, OCnode* field)
{
    addDecl(parent,field);
}

static void
addAttribute(OCnode* parent, OCnode* attr)
{
    if(parent->attributes == NULL)
	parent->attributes = oclistnew();
    oclistpush(parent->attributes,attr);
}

static void
addValue(OCnode* attr, const char* value)
{
    if(attr->att.values == NULL)
	attr->att.values = oclistnew();
    oclistpush(attr->att.values,strdup(value));
}

static void
addEnumConst(OCnode* parent, const char* name, long long lvalue)
{
    OCeconst* econst = (OCeconst*)calloc(1,sizeof(OCeconst));
    if(econst == NULL) return;
    econst->name = strdup(name);
    econst->value = lvalue;
    if(parent->econsts == NULL)
	parent->econsts = oclistnew();
    oclistpush(parent->econsts,econst);
}

static void
addDimRef(OCnode* var, OCnode* dim)
{
    if(var->array.dimensions == NULL)
	var->array.dimensions = oclistnew();
    oclistpush(var->array.dimensions,dim);
    var->array.rank++;
}

static void
addMap(OCnode* parent, OCnode* refvar)
{
    OCmap* map = (OCmap*)calloc(1,sizeof(OCmap));
    if(map == NULL) return;
    map->variable = refvar;
    if(parent->maps == NULL)
	parent->maps = oclistnew();
    oclistpush(parent->maps,map);
}

/**************************************************/
/* action definitions */

int
dap4_enterdataset(DAP4parser* state, OClist* xmlattrs)
{
    int stat = OC_NOERR;
    float ndapversion, ndmrversion;
    yax_token* name = getattr(xmlattrs, "name");
    yax_token* dapversion = getattr(xmlattrs, "dapversion");
    yax_token* dmrversion = getattr(xmlattrs, "dmrversion");

    if(isempty(name))
        return exception(state,OC_EMETA,"Empty dataset name attribute",NULL);
    /* convert and test version numbers */
    stat = sscanf(dapversion->text,"%g",&ndapversion);
    if(stat == EOF || stat != 1 || ndapversion != DAPVERSION)
        return exception(state,OC_EMETA,"Dataset dapVersion mismatch: %s",dapversion->text,NULL);
    stat = sscanf(dmrversion->text,"%g",&ndmrversion);
    if(stat == EOF || stat != 1 || ndmrversion != DMRVERSION)
        return exception(state,OC_EMETA,"Dataset dmrVersion mismatch: %s",dmrversion->text,NULL);
    OCnode* dataset = newocnode(state, name->text, OC_Dataset);
    state->root = dataset;
    state->conn->version.dapversion = ndapversion;
    state->conn->version.dmrversion = ndmrversion;
    oclistpush(state->scopestack,dataset);
    freexmllist(xmlattrs);
    return OC_NOERR;
}

int
dap4_leavedataset(DAP4parser* state)
{
    state->root = (OCnode*)oclistpop(state->scopestack);
    sort(state->root);
    if(oclistlength(state->scopestack) > 0)
        return exception(state,OC_EMETA,"Dataset: nested dataset",0);
    return OC_NOERR;
}

int
dap4_entergroup(DAP4parser* state, yax_token* name)
{
    if(isempty(name))
        return exception(state,OC_EMETA,"Empty group name",NULL);
    OCnode* parent = getGroupScope(state);
    OCnode* group = newocnode(state, name->text, OC_Group);
    addDecl(parent,group);
    oclistpush(state->scopestack,group);
    return OC_NOERR;
}

int
dap4_leavegroup(DAP4parser* state)
{
    oclistpop(state->scopestack);
    return OC_NOERR;
}

int
dap4_enterenumdef(DAP4parser* state, OClist* xmlattrs)
{
    yax_token* name;
    yax_token* basetype;
    const char* basetypename;
    OCtype baseoctype;

    CHECKDATADMR("enterenumdef");
    name = getattr(xmlattrs, "name");
    basetype = getattr(xmlattrs, "basetype");
    if(isempty(name))
        return exception(state,OC_EMETA,"Enumdef: Empty Enum Declaration name",0);
    if(basetypename == NULL) {
	baseoctype = DEFAULTENUMBASETYPE;
    } else {
	OCnode* dapenum;
	OCnode* parent;
	const char* typename = basetype->text;
        baseoctype = reify(typename);
        if(baseoctype == OC_NAT || !isLegalEnumType(baseoctype))
	    return exception(state,OC_EMETA,"Enumdef: Invalid Enum Declaration Type name: %s",basetype->text,NULL);
        dapenum = newocnode(state, name->text, OC_Enumeration);
	dapenum->basetype = baseoctype;
        parent = getGroupScope(state);
        addDecl(parent,dapenum);
        oclistpush(state->scopestack,dapenum);
    }
    freexmllist(xmlattrs);
    return OC_NOERR;
}

int
dap4_leaveenumdef(DAP4parser* state)
{
    OCnode* eparent;
    CHECKDATADMR("leaveenumdef");
    eparent = (OCnode*) oclistpop(state->scopestack);
    if(eparent->econsts == NULL || oclistlength(eparent->econsts) == 0)
        return exception(state,OC_EMETA,"Enumdef: no enum constants specified",0);
    return OC_NOERR;
}

int
dap4_enumconst(DAP4parser* state, yax_token* name, yax_token* value)
{
    int stat = OC_NOERR;
    long long lvalue;
    OCnode* parent;

    CHECKDATADMR("enumconst");
    if(isempty(name))
        return exception(state,OC_EMETA,"Enumconst: Empty enum constant name",NULL);
    if(isempty(value))
        return exception(state,OC_EMETA,"Enumdef: Invalid enum constant value: %s",value->text,0);
    stat = sscanf(value->text,"%lld",&lvalue);
    if(stat == EOF || stat != 1 || lvalue < 0)
        return exception(state,OC_EMETA,"Enumconst: illegal value: %s",value->text,NULL);
    parent = getScope1(state,OC_Enumeration);
    /* Verify that the name is a legal enum constant name, which is restricted
       vis-a-vis other names */
    if(!isLegalEnumConstName(name->text))
        return exception(state,OC_EMETA,"Enumconst: illegal enumeration constant name: %s",name->text,0);
    addEnumConst(parent, name->text, lvalue);
    return OC_NOERR;
}

int
dap4_enterdimdef(DAP4parser* state, OClist* xmlattrs)
{
    int stat = OC_NOERR;
    yax_token* name;
    yax_token* size;    
    OCnode* dim;
    OCnode* parent;
    long long lvalue;

    CHECKDATADMR("enterdimdef");
    name = getattr(xmlattrs, "name");
    size = getattr(xmlattrs, "size");
    if(isempty(name))
        return exception(state,OC_EMETA,"Dimdef: Empty dimension declaration name",NULL);
    if(isempty(size))
        return exception(state,OC_EMETA,"Dimdef: Empty dimension declaration size",0);
    stat = sscanf(size->text,"%lld",&lvalue);
    if(stat == EOF || stat != 1 || lvalue <= 0)
        return exception(state,OC_EMETA,"Illegal dimension length: %s",size->text,NULL);
    dim = newocnode(state, name->text, OC_Dimension);
    dim->dim.declsize = lvalue;

    parent = getGroupScope(state);
    addDecl(parent,dim);
    oclistpush(state->scopestack,dim);
    freexmllist(xmlattrs);
    return OC_NOERR;
}

int
dap4_leavedimdef(DAP4parser* state)
{
    CHECKDATADMR("leavedimdef");
    oclistpop(state->scopestack);
    return OC_NOERR;
}

int
dap4_dimref(DAP4parser* state, yax_token* nameorsize)
{
    int stat = OC_NOERR;
    OCnode* dim;
    OCnode* var;
    int isname;

    var = getVariableScope(state);
    assert(var != NULL);

    isname = strcmp(nameorsize->name,"name") == 1 ? 1 : 0;
    if(isname && isempty(nameorsize))
        return exception(state,OC_EMETA,"Dimref: Empty dimension reference name",0);
    else if(isempty(nameorsize))
        return exception(state,OC_EMETA,"Dimref: Empty dimension size",NULL);
    if(isname) {
        dim = findByFQN(state->root,nameorsize->text,OC_Dimension);
        if(dim == NULL)
            return exception(state,OC_EMETA,"Unknown dimension: %s",nameorsize->text,0);
    } else {/* Size only is given; presume a number; create unique anonymous dimension */
        /* Note that we create it in the root group*/
	long long anonsize;
        size_t len, i;
        OCnode* root = state->root;
        OCnode* dim;

	stat = sscanf(nameorsize->text,"%lld",&anonsize);
        if(stat == EOF || stat != 1 || anonsize <= 0)
            return exception(state,OC_EMETA,"Illegal dimension length: %s",nameorsize->text,NULL);
        if(root->subnodes == NULL)
	    root->subnodes = oclistnew();
        len = oclistlength(root->subnodes);
	dim = NULL;
        for(i=0;i<len;i++) {
	    dim = oclistget(root->subnodes,i);
	    if(dim->octype != OC_Dimension || dim->name != NULL)
		continue;
	    if(dim->dim.declsize == anonsize)
		break;
        }
	if(dim == NULL) {
	    dim = newocnode(state, NULL, OC_Dimension);
	    dim->dim.anonymous = 1;	
	    dim->dim.declsize = anonsize;
	    addDecl(root,dim);
	}
    }
    addDimRef(var,dim);
    return OC_NOERR;
}

int
dap4_enteratomicvariable(DAP4parser* state, yax_token* open, yax_token* name)
{
    const char* typename;
    OCtype basetype;
    OCnode* parent;
    OCnode* var;

    if(isempty(name))
        return exception(state,OC_EMETA,"Atomicvariable: Empty dimension reference name",0);
    typename = open->name;
    basetype = reify(typename);
    if(basetype == OC_NAT);
        return exception(state,OC_EMETA,"AtomicVariable: Illegal type: %s",open->name,NULL);
    /* Do type substitutions */
    var = newocnode(state, name->text, OC_Atomic);
    var->basetype = basetype;
    /* Look at the parent scope */
    parent = oclisttop(state->scopestack);
    switch (parent->octype) {
    case OC_Dataset:
    case OC_Group:
	addDecl(parent,var);
        break;
    case OC_Structure:
    case OC_Sequence:
	addField(parent,var);
	break;
    default:
        assert(0);
    }
    oclistpush(state->scopestack,var);
    return OC_NOERR;
}

int
dap4_leaveatomicvariable(DAP4parser* state, yax_token* close)
{
    OCtype basetype;
    char* typename;
    OCnode* var;

    typename = close->name;
    basetype = reify(typename);    
    if(basetype == OC_NAT)
        return exception(state,OC_EMETA,"AtomicVariable: Illegal type: %s",close->name,0);
    var = getScope1(state,OC_Atomic);
    if(var->basetype != basetype)
	return exception(state,OC_EMETA,"variable: open/close type mismatch: <%s> </%s>,NULL",
		         octypename(var->basetype),octypename(basetype),0);
    oclistpop(state->scopestack);
    return fixdimset(state,var);
}

int
dap4_enterenumvariable(DAP4parser* state, OClist* xmlattrs)
{
    yax_token* name;
    yax_token* enumtype;
    OCnode* var;
    OCnode* parent;
    OCnode* enumdef;

    name = getattr(xmlattrs, "name");
    enumtype = getattr(xmlattrs, "enum");
    if(isempty(name))
        return exception(state,OC_EMETA,"Enumvariable: Empty variable name",0);
    if(isempty(enumtype))
        return exception(state,OC_EMETA,"Enumvariable: Empty enum type name",NULL);
    enumdef = findByFQN(state->root,enumtype->text, OC_Enumeration);
    if(enumdef == NULL)
        return exception(state,OC_EMETA,"EnumVariable: no such enum: %s",name->text,0);
    var = newocnode(state, name->text, OC_Atomic);
    var->enumdef = enumdef;
    parent = getGroupScope(state);
    addDecl(parent,var);
    oclistpush(state->scopestack,var);
    freexmllist(xmlattrs);
    return OC_NOERR;
}

int
dap4_leaveenumvariable(DAP4parser* state)
{
    OCnode* var;

    var = getScope1(state,OC_Atomic);
    assert(var != NULL);
    oclistpop(state->scopestack);
    return OC_NOERR;
}

int
dap4_entermap(DAP4parser* state, yax_token* name)
{
    OCnode* var;
    OCnode* parent;
    OCmap* map;

    CHECKDATADMR("entermap");
    if(isempty(name))
        return exception(state,OC_EMETA,"Mapref: Empty map name",NULL);
    var = findByFQN(state->root,name->text, OC_Atomic);
    if(var == NULL)
        return exception(state,OC_EMETA,"Mapref: undefined variable: %s",name->text,0);
    /* Pull the top variable scope */
    parent = getScope(state, OC_Atomic, OC_Structure, OC_Sequence, 0);
    addMap(parent,var);
    oclistpush(state->scopestack,map);
    return OC_NOERR;
}

int
dap4_leavemap(DAP4parser* state)
{
    CHECKDATADMR("leavemap");
    oclistpop(state->scopestack);
    return OC_NOERR;
}

int
dap4_enterstructurevariable(DAP4parser* state, yax_token* name)
{
    OCnode* var;
    OCnode* parent;

    if(isempty(name))
        return exception(state,OC_EMETA,"Structure: Empty structure name",0);
    var = newocnode(state, name->text, OC_Structure);
    /* Look at the parent scope */
    parent = oclisttop(state->scopestack);
    switch (parent->octype) {
    case OC_Dataset:
    case OC_Group:
	addDecl(parent,var);
	break;
    case OC_Structure:
    case OC_Sequence:
	addField(parent,var);
        break;
    default:
	assert(0);
    }
    oclistpush(state->scopestack,var);
    return OC_NOERR;
}

int
dap4_leavestructurevariable(DAP4parser* state)
{
    OCnode* struc = getScope1(state,OC_Structure);
    OClist* dups = scopeduplicates(struc->subnodes);
    if(dups != NULL) {
        ocnodes_free(dups);
	return exception(state,OC_ENAMEINUSE,"Duplicate structure field names in same structure: %s",struc->name,NULL);
    }
    assert(struc != NULL);
    oclistpop(state->scopestack);
    return fixdimset(state,struc);
}

int
dap4_entersequencevariable(DAP4parser* state, yax_token* name)
{
    OCnode* var;
    OCnode* parent;

    if(isempty(name))
        return exception(state,OC_EMETA,"Sequence: Empty sequence name",0);
    var = newocnode(state, name->text, OC_Sequence);
    /* Look at the parent scope */
    parent = oclisttop(state->scopestack);
    switch (parent->octype) {
    case OC_Dataset:
    case OC_Group:
	addDecl(parent,var);
	break;
    case OC_Structure:
    case OC_Sequence:
	addField(parent,var);
    default:
	assert(0);
    }
    oclistpush(state->scopestack,var);
    return OC_NOERR;
}

int
dap4_leavesequencevariable(DAP4parser* state)
{
    OCnode* seq = getScope1(state,OC_Sequence);
    OClist* dups = scopeduplicates(seq->subnodes);
    if(dups != NULL) {
        ocnodes_free(dups);
	return exception(state,OC_ENAMEINUSE,"Duplicate sequence field names in same sequence: %s",seq->name,NULL);
    }
    oclistpop(state->scopestack);
    return fixdimset(state,seq);
}

int
dap4_enteratomicattribute(DAP4parser* state, OClist* xmlattrs, OClist* nslist)
{
    OCnode* parent;
    OCnode* attr;
    char* typename;
    OCtype octype;
    yax_token* name = getattr(xmlattrs,"name");
    yax_token* atype = getattr(xmlattrs,"type");
    yax_token* value = getattr(xmlattrs,"value");

    parent = getMetadataScope(state);

    if(isempty(name))
        return exception(state,OC_EMETA,"Empty attribute name",NULL);

    typename = (atype == NULL ? "Int32" : atype->text);
    octype = reify(typename);

    if(!isLegalAttributeType(octype))
        return exception(state,OC_EMETA,"Attribute: Invalid attribute type: %s",typename,NULL);

    attr = newocnode(state,name->text,OC_Attribute);
    if(attr == NULL)
        return exception(state,OC_ENOMEM,"out of memory",0);
    attr->basetype = octype;
    if(value != NULL)
	addValue(attr,value->text);
    if(nslist != null && oclistlength(nslist) > 0)
	attr->att.nslist = nslist;
    addAttribute(parent,attr);
    oclistpush(state->scopestack,attr);
    freexmllist(xmlattrs);
    return OC_NOERR;
}

int
dap4_leaveatomicattribute(DAP4parser* state)
{
    OCnode* attr = (OCnode*)oclistpop(state->scopestack);
    /* Ensure that the attribute has at least one value ? Should this be an error?*/
    if(oclistlength(attr->att.values) == 0)
        return exception(state,OC_EMETA,"AtomicAttribute: attribute has no values",0);
    return OC_NOERR;
}

int
dap4_entercontainerattribute(DAP4parser* state, OClist* xmlattrs, OClist* nslist)
{
    OCnode* parent;
    OCnode* attr;
    yax_token* name = getattr(xmlattrs,"name");

    parent = getMetadataScope(state);

    if(isempty(name))
        return exception(state,OC_EMETA,"Empty container attribute name",NULL);

    attr = newocnode(state,name->text,OC_Attributeset);
    if(attr == NULL)
        return exception(state,OC_ENOMEM,"out of memory",0);
    if(nslist != null && oclistlength(nslist) > 0)
	attr->att.nslist = nslist;
    addAttribute(parent,attr);
    oclistpush(state->scopestack,attr);
    freexmllist(xmlattrs);
    return OC_NOERR;
}

int
dap4_leavecontainerattribute(DAP4parser* state)
{
    oclistpop(state->scopestack);
    return OC_NOERR;
}

int
dap4_value(DAP4parser* state, yax_token* value)
{
    OCnode* attr = getScope1(state,OC_Attribute);
    addValue(attr,value->text);
    return OC_NOERR;
}

OClist*
dap4_namespace_list(DAP4parser* state, OClist* list, yax_token* elem)
{
    if(list == NULL) {
	list = oclistnew();
	assert(elem == NULL);
    } else {
	oclistpush(list,elem->text);
    }
    return list;
}

int
dap4_otherxml(DAP4parser* state, OClist* xmlattrs, OClist* body)
{
    OCnode* parent;
    OCnode* other;
    yax_token* name;
    char* sname = NULL;

    name = getattr(xmlattrs,"name");
    if(!isempty(name))
	sname = name->text;
    other = newocnode(state,sname,OC_XML);
    /* Only use 1st element in the body, if any */
    if(oclistlength(body) > 0)
        other->xml.document = oclistget(body,0);
    parent = getMetadataScope(state);
    addAttribute(parent,other);
    freexmllist(xmlattrs);
    return OC_NOERR;
}

OClist*
dap4_otherxml_attributes(DAP4parser* state, OClist* list, OCDOM* elem)
{
    if(list == NULL) {
	list = oclistnew();
	assert(elem == NULL);
    } else {
	oclistpush(list,elem);
    }
    return list;
}

OClist*
dap4_xmlbody(DAP4parser* state, OClist* list, OCDOM* elem)
{
    if(list == NULL) {
	list = oclistnew();
	assert(elem == NULL);
    } else {
	oclistpush(list,elem);
    }
    return list;
}

OCDOM*
dap4_xmlelement(DAP4parser* state, yax_token* open, OClist* attrs, OClist* body, yax_token* close) 
{
    if(!((open->name == null) ^ (close->name == null))) {
	exception(state,OC_EMETA,"XML Open/close name mismatch",NULL);
	return NULL;
    }
    if(strcmp(open->name,close->name) != 0) {
	exception(state,OC_EMETA,"XML Open/close name mismatch: %s :: %s",open->name,close->name,NULL);
	return NULL;
    }
    OCDOM* node = (OCDOM*)calloc(1,sizeof(OCDOM));
    node->flags = DOMFLAG_ELEMENT;
    node->name = nulldup(open->name);
    node->attrs = attrs;
    node->subnodes = body;
    return node;
}

OCDOM*
dap4_xmltext(DAP4parser* state, yax_token* text)
{
    OCDOM* node = (OCDOM*)calloc(1,sizeof(OCDOM));
    node->flags = DOMFLAG_TEXT;
    node->text = strdup(text->text);
    return node;
}

int
dap4_entererror(DAP4parser* state, OClist* xmlattrs)
{
    int stat = OC_NOERR;
    yax_token* xhttpcode = getattr(xmlattrs, "httpcode");
    const char* shttpcode = (xhttpcode == NULL ? "400" : xhttpcode->text);
    int httpcode;
    stat = sscanf(shttpcode,"%u",&httpcode);
    if(stat == EOF || stat != 1)     
        return exception(state,OC_EMETA,"Error Response; illegal http code: %s",shttpcode,0);
    state->conn->error.httpcode = httpcode;
    freexmllist(xmlattrs);
    return OC_NOERR;
}

int
dap4_leaveerror(DAP4parser* state)
{
    state->conn->error.ocerr = OC_ERESPONSE;
    return OC_NOERR;
}

int
dap4_errormessage(DAP4parser* state, yax_token* value)
{
    char* msg = yax_unescape(value->text,NULL);
    state->conn->error.message = msg;
    return OC_NOERR;
}

int
dap4_errorcontext(DAP4parser* state, yax_token* value)
{
    char* cxt = yax_unescape(value->text,NULL);
    state->conn->error.context = cxt;
    return OC_NOERR;
}

int
dap4_errorotherinfo(DAP4parser* state, yax_token* value)
{
    char* other = yax_unescape(value->text,NULL);
    state->conn->error.otherinfo = other;
    return OC_NOERR;
}

/**************************************************/

static void
dap4_parse_cleanup(DAP4parser* state)
{
    dap4lexcleanup(&state->lexstate);
    if(state->ocnodes != NULL) ocnodes_free(state->ocnodes);
    state->ocnodes = NULL;
    free(state);
}

static DAP4parser*
dap4_parse_init(char* buf)
{
    DAP4parser* state = (DAP4parser*)ocmalloc(sizeof(DAP4parser)); /*ocmalloc zeros*/
    MEMCHECK(state,NULL);
    if(buf==NULL) {
        dap4_parse_error(state,"dap_parse_init: no input buffer");
        state->error = OC_EINVAL; /* semantic error */
        dap4_parse_cleanup(state);
        return NULL;
    }
    dap4lexinit(buf,&state->lexstate);
    return state;
}

/* Wrapper for dapparse */
OCerror
DAP4parse(OCstate* conn, OCtree* tree, char* parsestring)
{
    DAP4parser* state = dap4_parse_init(parsestring);
    int parseresult;
    OCerror ocerr = OC_NOERR;
    state->ocnodes = oclistnew();
    state->conn = conn;
    if(ocdebug >= 2)
        dap4debug = 1;
    parseresult = dap4parse(state);
    if(parseresult == 0) {/* 0 => parse ok */
        if(conn->error.ocerr != OC_NOERR) {
            tree->root = NULL;
            /* Attempt to further decipher the error code */
            if(conn->error.httpcode == 404)
                ocerr = OC_ENOFILE;
            else
                ocerr = conn->error.ocerr;
        } else {
            tree->root = state->root;
            state->root = NULL; /* avoid reclaim */
            tree->nodes = state->ocnodes;
            state->ocnodes = NULL; /* avoid reclaim */
            tree->root->tree = tree;
            ocerr = OC_NOERR;
        }
    }
    dap4_parse_cleanup(state);
    return ocerr;
}

/**
 * Sort helper
 *
 * @param node the current node we are traversing
 * @param sortlist the new list in sorted order
 */
static void
sortR(OCnode* node, OClist* sortlist)
{
    size_t i;
    OClist* list;
    oclistpush(sortlist,node);
    switch (node->octype) {
    case OC_Dataset:
    case OC_Group:
        /* Walk the decls in this group in order:
           global attributes, dimensions, enums, variables, groups */
        list = node->attributes;
	for(i=0;i<oclistlength(list);i++) {
	    sortR((OCnode*)oclistget(list,i),sortlist);
        }
        list = node->array.dimensions;
	for(i=0;i<oclistlength(list);i++) {
	    sortR((OCnode*)oclistget(list,i),sortlist);
        }
	/* Process the enum definitions */
        list = node->subnodes;
	for(i=0;i<oclistlength(list);i++) {
	    OCnode* subnode = oclistget(list,i);
	    if(subnode->octype == OC_Enumeration)
		sortR(subnode,sortlist);
        }
	/* Process the top-level variables */
        list = node->subnodes;
	for(i=0;i<oclistlength(list);i++) {
	    OCnode* subnode = oclistget(list,i);
	    if(subnode->container != NULL
		&& subnode->container->octype != OC_Group
		&& subnode->container->octype != OC_Dataset)
		continue; /* not top-level */
	    if(subnode->octype == OC_Atomic 
		|| subnode->octype == OC_Sequence
		|| subnode->octype == OC_Structure)
		sortR(node,sortlist);
        }
	/* Process the inner groups */
        list = node->subnodes;
	for(i=0;i<oclistlength(list);i++) {
	    OCnode* subnode = oclistget(list,i);
	    if(subnode->octype == OC_Group)
		sortR(subnode,sortlist);
        }
        break;
    case OC_Grid:
    case OC_Sequence:
    case OC_Structure:
	list = node->subnodes;
        for(i = 0;i < oclistlength(list);i++)
            sortR((OCnode*)oclistget(list,i), sortlist);
	/* fall thru */
    case OC_Atomic:
        list = node->attributes;
	for(i=0;i<oclistlength(list);i++) {
	    sortR((OCnode*)oclistget(list,i),sortlist);
        }
        break;
    case OC_Attributeset:
        list = node->subnodes;
	for(i=0;i<oclistlength(list);i++) {
            sortR((OCnode*)oclistget(list,i), sortlist);
        }
        break;
    default:
        break;
    }
}

/**
 * Sort the nodelist into prefix left to right order
 */
static void
sort(OCnode* root)
{
    size_t i;
    OClist* sorted = oclistnew();
    OCtree* tree = root->tree;
    sortR(root, sorted);
    /* Assign indices */
    for(i = 0;i < oclistlength(sorted);i++) {
	OCnode* node = (OCnode*)oclistget(sorted,i);	
	node->order = i;
    }
    oclistfree(tree->nodes);
    tree->nodes = sorted;
}

