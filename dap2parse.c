/* Copyright 2009, UCAR/Unidata and OPeNDAP, Inc.
   See the COPYRIGHT file for more information. */

#include "config.h"
#include "dap2parselex.h"
#include "dap2tab.h"

/* Forward */

static void addedges(OCnode* node);
static void setroot(OCnode*,OClist*);
static int isglobalname(const char* name);
static int isdodsname(const char* name);
static OCnode* newocnode(char* name, OCtype octype, DAP2parsestate* state);
static OCtype octypefor(Object etype);
static OClist* scopeduplicates(OClist* list);
static int check_int32(char* val, long* value);


/****************************************************/

/* Switch to DAS parsing SCAN_WORD definition */

/* Use the initial keyword to indicate what we are parsing */
void
dap2_tagparse(DAP2parsestate* state, int kind)
{
    switch (kind) {
    case SCAN_DATASET:
    case SCAN_ERROR:
	break;
    case SCAN_ATTR:
	dap2setwordchars(state->lexstate,1);
        break;
    default:
        fprintf(stderr,"tagparse: Unknown tag argument: %d\n",kind);
    }
}


Object
dap2_datasetbody(DAP2parsestate* state, Object name, Object decls)
{
    OCnode* root = newocnode((char*)name,OC_Dataset,state);
    char* dupname = NULL;
    OClist* dups = scopeduplicates((OClist*)decls);
    if(dups != NULL) {
	/* Sometimes, some servers (i.e. Thredds)
           return a dds with duplicate field names
           at the dataset level; simulate an errorbody response
        */
	ocnodes_free(dups);
        dap2_parse_error(state,"Duplicate dataset field names: %s",name,dupname);
	state->error = OC_ENAMEINUSE;
	return (Object)NULL;
    }
    root->subnodes = (OClist*)decls;
    OCASSERT((state->root == NULL));
    state->root = root;
    state->root->root = state->root; /* make sure to cross link */
    addedges(root);
    setroot(root,state->ocnodes);
    return NULL;
}

Object
dap2_attributebody(DAP2parsestate* state, Object attrlist)
{
    OCnode* node;
    /* Check for and remove attribute duplicates */
    OClist* dups = scopeduplicates((OClist*)attrlist);
    if(dups != NULL) {
        ocnodes_free(dups);	
	dap2_parse_error(state,"Duplicate attribute names in same scope");
	state->error = OC_ENAMEINUSE; /* semantic error */
	return NULL;
    }
    node = newocnode(NULL,OC_Attributeset,state);
    OCASSERT((state->root == NULL));
    state->root = node;
    /* make sure to cross link */
    state->root->root = state->root;
    node->subnodes = (OClist*)attrlist;
    addedges(node);
    return NULL;
}

void
dap2_errorbody(DAP2parsestate* state,
	  Object code, Object msg, Object ptype, Object prog)
{
    state->error = OC_EDAPSVC;
    state->code     = nulldup((char*)code);
    state->message  = nulldup((char*)msg);
    /* Ignore ptype and prog for now */
}

void
dap2_unrecognizedresponse(DAP2parsestate* state)
{
    /* see if this is an HTTP error */
    unsigned int httperr = 0;
    int i;
    char iv[32];
    sscanf(state->lexstate->input,"%u ",&httperr);
    sprintf(iv,"%u",httperr);
    state->lexstate->next = state->lexstate->input;
    /* Limit the amount of input to prevent runaway */
    for(i=0;i<4096;i++) {if(state->lexstate->input[i] == '\0') break;}
    state->lexstate->input[i] = '\0';
    dap2_errorbody(state,iv,state->lexstate->input,NULL,NULL);
}

Object
dap2_declarations(DAP2parsestate* state, Object decls, Object decl)
{
    OClist* alist = (OClist*)decls;
    if(alist == NULL)
	 alist = oclistnew();
    else
	oclistpush(alist,(void*)decl);
    return alist;
}

Object
dap2_arraydecls(DAP2parsestate* state, Object arraydecls, Object arraydecl)
{
    OClist* alist = (OClist*)arraydecls;
    if(alist == NULL)
	alist = oclistnew();
    else
	oclistpush(alist,(void*)arraydecl);
    return alist;
}

Object
dap2_arraydecl(DAP2parsestate* state, Object name, Object size)
{
    long value;
    OCnode* dim;
    if(!check_int32(size,&value)) {
	dap2_parse_error(state,"Dimension not an integer");
	state->error = OC_EDIMSIZE; /* signal semantic error */
    }
    if(name != NULL)
	dim = newocnode((char*)name,OC_Dimension,state);
    else {
	dim = newocnode(NULL,OC_Dimension,state);
	dim->dim.anonymous = 1;
    }
    dim->dim.declsize = value;
    return dim;
}

Object
dap2_attrlist(DAP2parsestate* state, Object attrlist, Object attrtuple)
{
    OClist* alist = (OClist*)attrlist;
    if(alist == NULL)
	alist = oclistnew();
    else {
	if(attrtuple != NULL) {/* NULL=>alias encountered, ignore */
            oclistpush(alist,(void*)attrtuple);
	}
    }
    return alist;
}

Object
dap2_attrvalue(DAP2parsestate* state, Object valuelist, Object value, Object etype)
{
    OClist* alist = (OClist*)valuelist;
    if(alist == NULL) alist = oclistnew();
    /* Watch out for null values */
    if(value == NULL) value = "";
    oclistpush(alist,(void*)strdup(value));
    return alist;
}

Object
dap2_attribute(DAP2parsestate* state, Object name, Object values, Object basetype)
{
    OCnode* att;
    att = newocnode((char*)name,OC_Attribute,state);
    att->basetype = octypefor(basetype);
    att->att.values = (OClist*)values;
    return att;
}

Object
dap2_attrset(DAP2parsestate* state, Object name, Object attributes)
{
    OCnode* attset;
    attset = newocnode((char*)name,OC_Attributeset,state);
    /* Check var set vs global set */
    attset->att.isglobal = isglobalname(name);
    attset->att.isdods = isdodsname(name);
    attset->subnodes = (OClist*)attributes;
    addedges(attset);
    return attset;
}

static int
isglobalname(const char* name)
{
    int len = strlen(name);
    int glen = strlen("global");
    const char* p;
    if(len < glen) return 0;
    p = name + (len - glen);
    if(strcasecmp(p,"global") != 0)
	return 0;
    return 1;
}

static int
isdodsname(const char* name)
{
    size_t len = strlen(name);
    size_t glen = strlen("DODS");
    if(len < glen) return 0;
    if(ocstrncmp(name,"DODS",glen) != 0)
	return 0;
    return 1;
}

static int
dimension(DAP2parsestate* state, OCnode* node, OClist* dimensions)
{
    unsigned int rank = oclistlength(dimensions);
    if(rank >= OC_MAX_RANK) {
        dap2_parse_error(state,"Too many dimensions");
	state->error = OC_ERANK;
	return 0;
    }
    node->array.dimensions = (OClist*)dimensions;
    node->array.rank = rank;
    return 1;
}

Object
dap2_makebase(DAP2parsestate* state, Object name, Object basetype, Object dimensions)
{
    OCnode* node;
    node = newocnode((char*)name,OC_Atomic,state);
    node->basetype = octypefor(basetype);
    if(!dimension(state,node,(OClist*)dimensions)) {
	ocnode_free(node);
	return NULL;
    }
    return node;
}

Object
dap2_makestructure(DAP2parsestate* state, Object name, Object dimensions, Object fields)
{
    OCnode* node;
    OClist* dups = scopeduplicates((OClist*)fields);
    if(dups != NULL) {
	ocnodes_free(dups);
        dap2_parse_error(state,"Duplicate structure field names in same structure: %s",(char*)name);
	state->error = OC_ENAMEINUSE; /* semantic error */
	return (Object)NULL;
    }
    node = newocnode(name,OC_Structure,state);
    node->subnodes = fields;
    if(!dimension(state,node,(OClist*)dimensions)) {
	ocnode_free(node);
	return NULL;
    }
    addedges(node);
    return node;
}

Object
dap2_makesequence(DAP2parsestate* state, Object name, Object members)
{
    OCnode* node;
    OClist* dups = scopeduplicates((OClist*)members);
    if(dups != NULL) {
	ocnodes_free(dups);
        dap2_parse_error(state,"Duplicate sequence member names in same sequence: %s",(char*)name);
	return (Object)NULL;
    }
    node = newocnode(name,OC_Sequence,state);
    node->subnodes = members;
    addedges(node);
    return node;
}

Object
dap2_makegrid(DAP2parsestate* state, Object name, Object arraydecl, Object mapdecls)
{
    OCnode* node;
    /* Check for duplicate map names */
    OClist* dups = scopeduplicates((OClist*)mapdecls);
    if(dups != NULL) {
	ocnodes_free(dups);
        dap2_parse_error(state,"Duplicate grid map names in same grid: %s",(char*)name);
	state->error = OC_ENAMEINUSE; /* semantic error */
	return (Object)NULL;
    }
    node = newocnode(name,OC_Grid,state);
    node->subnodes = (OClist*)mapdecls;
    oclistinsert(node->subnodes,0,(void*)arraydecl);
    addedges(node);
    return node;
}

static void
addedges(OCnode* node)
{
    unsigned int i;
    if(node->subnodes == NULL) return;
    for(i=0;i<oclistlength(node->subnodes);i++) {
        OCnode* subnode = (OCnode*)oclistget(node->subnodes,i);
	subnode->container = node;
    }
}

static void
setroot(OCnode* root, OClist* ocnodes)
{
    size_t i;
    for(i=0;i<oclistlength(ocnodes);i++) {
	OCnode* node = (OCnode*)oclistget(ocnodes,i);
	node->root = root;
    }
}

int
dap2error(DAP2parsestate* state, const char* msg)
{
    return dap2semanticerror(state,OC_EINVAL,msg);
}

int
dap2semanticerror(DAP2parsestate* state, OCerror err, const char* msg)
{
    dap2_parse_error(state,msg);
    state->error = err; /* semantic error */
    return 0;
}

static char*
flatten(char* s, char* tmp, size_t tlen)
{
    int c;
    char* p,*q;
    strncpy(tmp,s,tlen);
    tmp[tlen] = '\0';
    p = (q = tmp);
    while((c=*p++)) {
	switch (c) {
	case '\r': case '\n': break;
	case '\t': *q++ = ' '; break;
	case ' ': if(*p != ' ') *q++ = c; break;
	default: *q++ = c;
	}
    }
    *q = '\0';
    return tmp;
}

/* Create an ocnode and capture in the state->ocnode list */
static OCnode*
newocnode(char* name, OCtype octype, DAP2parsestate* state)
{
    OCnode* node = ocnode_new(name,octype,state->root);
    oclistpush(state->ocnodes,(void*)node);
    return node;
}

static int
check_int32(char* val, long* value)
{
    char* ptr;
    int ok = 1;
    long iv = strtol(val,&ptr,0); /* 0=>auto determine base */
    if((iv == 0 && val == ptr) || *ptr != '\0') {ok=0; iv=1;}
    else if(iv > OC_INT32_MAX || iv < OC_INT32_MIN) ok=0;
    if(value != NULL) *value = iv;
    return ok;
}

static OClist*
scopeduplicates(OClist* list)
{
    unsigned int i,j;
    unsigned int len = oclistlength(list);
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

static OCtype
octypefor(Object basetype)
{
    switch ((long)basetype) {
    case SCAN_BYTE: return OC_Int8;
    case SCAN_INT16: return OC_Int16;
    case SCAN_UINT16: return OC_UInt16;
    case SCAN_INT32: return OC_Int32;
    case SCAN_UINT32: return OC_UInt32;
    case SCAN_FLOAT32: return OC_Float32;
    case SCAN_FLOAT64: return OC_Float64;
    case SCAN_URL: return OC_URL;
    case SCAN_STRING: return OC_String;
    default: abort();
    }
    return OC_NAT;
}

void
dap2_parse_error(DAP2parsestate* state, const char *fmt, ...)
{
    size_t len, suffixlen, prefixlen;
    va_list argv;
    char* tmp = NULL;
    va_start(argv,fmt);
    (void) vfprintf(stderr,fmt,argv) ;
    (void) fputc('\n',stderr) ;
    len = strlen(state->lexstate->input);
    suffixlen = strlen(state->lexstate->next);
    prefixlen = (len - suffixlen);
    tmp = (char*)ocmalloc(len+1);
    flatten(state->lexstate->input,tmp,prefixlen);
    (void) fprintf(stderr,"context: %s",tmp);
    flatten(state->lexstate->next,tmp,suffixlen);
    (void) fprintf(stderr,"^%s\n",tmp);
    (void) fflush(stderr);	/* to ensure log files are current */
    ocfree(tmp);
    va_end(argv);
}

static void
dap2_parse_cleanup(DAP2parsestate* state)
{
    dap2lexcleanup(&state->lexstate);
    if(state->ocnodes != NULL) ocnodes_free(state->ocnodes);
    state->ocnodes = NULL;
    free(state);
}

static DAP2parsestate*
dap2_parse_init(char* buf)
{
    DAP2parsestate* state = (DAP2parsestate*)ocmalloc(sizeof(DAP2parsestate)); /*ocmalloc zeros*/
    MEMCHECK(state,NULL);
    if(buf==NULL) {
        dap2_parse_error(state,"dap_parse_init: no input buffer");
	state->error = OC_EINVAL; /* semantic error */
	dap2_parse_cleanup(state);
	return NULL;
    }
    dap2lexinit(buf,&state->lexstate);
    return state;
}

/* Wrapper for dapparse */
OCerror
DAP2parse(OCstate* conn, OCtree* tree, char* parsestring)
{
    DAP2parsestate* state = dap2_parse_init(parsestring);
    int parseresult;
    OCerror ocerr = OC_NOERR;
    state->ocnodes = oclistnew();
    state->conn = conn;
    if(ocdebug >= 2)
	dap2debug = 1;
    parseresult = dap2parse(state);
    if(parseresult == 0) {/* 0 => parse ok */
	if(state->error == OC_EDAPSVC) {
	    /* we ended up parsing an error message from server */
            conn->error.code = nulldup(state->code);
            conn->error.message = nulldup(state->message);
	    tree->root = NULL;
	    /* Attempt to further decipher the error code */
	    if(state->code != NULL
                && (strcmp(state->code,"404") == 0 /* tds returns 404 */
		    || strcmp(state->code,"5") == 0)) /* hyrax returns 5 */
		ocerr = OC_ENOFILE;
	    else
	        ocerr = OC_EDAPSVC;
	} else if(state->error != OC_NOERR) {
	    /* Parse failed for semantic reasons */
	    ocerr = state->error;
	} else {
            tree->root = state->root;
	    state->root = NULL; /* avoid reclaim */
            tree->nodes = state->ocnodes;
	    state->ocnodes = NULL; /* avoid reclaim */
            tree->root->tree = tree;
	    ocerr = OC_NOERR;
	}
    } else { /* Parse failed */
	switch (tree->dxdclass) {
	case OCDAS: ocerr = OC_EDAS; break;
	case OCDDS: ocerr = OC_EDDS; break;
	case OCDATADDS: ocerr = OC_EDATADDS; break;
	default: ocerr = OC_EDAPSVC;
	}		
    }
    dap2_parse_cleanup(state);
    return ocerr;
}

