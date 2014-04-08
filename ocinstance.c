/* Copyright 2009, UCAR/Unidata and OPeNDAP, Inc.
   See the COPYRIGHT file for more information. */

#include "config.h"
#include "ocinternal.h"
#include "ocdebug.h"
#include "ocdump.h"

/* Forward*/
static void ocinstance_clear(OCinstance*);
static OCerror ocread(OCdata*, XXDR*, char*, size_t, size_t, size_t);

#ifdef OCDEBUG

static void
octrace(char* proc, OCstate* state, OCinstance* instance)
{
    OCbytes* buffer = ocbytesnew();
    ocdumpinstancepath(state,instance,buffer);
    fprintf(stderr,"%s: %s\n",proc,ocbytescontents(buffer));
}

#else

#define octrace(proc,state,instance)
#endif /*OCDEBUG*/

OCinstance*
ocinstance_new(OCstate* state, OCinstance* parent)
{
    OCinstance* instance;
    OCASSERT(state != NULL);
    instance = state->instancelist;
    /* Search for an unused instance node*/
    while(instance != NULL && instance->header.occlass != OC_None) {
	instance = instance->next;
    }
    if(instance == NULL) {
	instance = (OCinstance*)calloc(1,sizeof(OCinstance));
	if(instance==NULL)
	    return (OCinstance*)NULL;
        instance->next = state->instancelist;
        state->instancelist = instance;
    }
    instance->parent = parent;    
    ocinstance_clear(instance);
    return instance;
}

void
ocinstance_free(OCstate* state, OCinstance* instance)
{
    OCinstance* p;
    OCASSERT(instance != NULL);
    if(instance->header.occlass != OC_Instance) return; /* ignore */
    /* Search recursively for all instances for which this is parent */
    for(p=state->instancelist;p!=NULL;p=p->next) {
	if(p->header.occlass == OC_Instance && p->parent == instance) {
	    ocinstance_free(state,p);
        }
    }
    ocinstance_clear(instance);
    instance->parent = NULL;
}

static void
ocinstance_clear(OCinstance* instance)
{
    OCinstance* next = instance->next; /* save some values */
    OCinstance* parent = instance->parent;
    /* ok, zero out everything */
    memset((void*)instance,sizeof(OCinstance),0);
    /* set/restore non-null fields */
    instance->header.magic = OCMAGIC;
    instance->header.occlass = OC_Instance;
    instance->next = next;
    instance->parent = parent;
    instance->mode = OCDT_OTHER;
}

/* Use this to attach to a data tree for a DATADDS */
OCerror
ocinstance_attach(OCstate* state, OCnode* root, OCinstance* instance)
{
    assert(root->tree->dxdclass == OCDATADDS);
    assert(root->octype == OC_Dataset);

    if(root->tree->data.data == NULL)
	return OCTHROW(OC_ENODATA);

    /* fill in the instance */
    instance->parent = NULL;
    instance->ocdata = root->tree->data.data;

    octrace("attach",state,instance);

    return OCTHROW(OC_NOERR);
}

OCerror
ocinstance_container(OCstate* state, OCinstance* instance, OCinstance* container)
{
    OCnode* template;
    OCinstance* super;

    OCASSERT(state != NULL);
    OCASSERT(container != NULL);

    template = instance->ocdata->template;

    if(template->container == NULL)
	return OCTHROW(OC_EBADTYPE);

    super = instance->parent;
    if(super == NULL)
	return OCTHROW(OC_EBADTYPE);

    /* Clone the container (or at least part of it) */
    container->parent = super->parent;
    container->ocdata = super->ocdata;
    container->mode = super->mode;

    octrace("container", state, container);

    return OC_NOERR;
}

OCerror
ocinstance_root(OCstate* state, OCinstance* instance, OCinstance* root)
{
    OCnode* template;
    OCASSERT(state != NULL);
    OCASSERT(root != NULL);

    template = instance->ocdata->template;
    root->ocdata = template->tree->data.data;
    root->mode = OCDT_OTHER;
    
    octrace("root", state, root);

    return OC_NOERR;
}

OCerror
ocinstance_ithfield(OCstate* state, OCinstance* container, size_t index, OCinstance* field)
{
    OCnode* template;

    OCASSERT(state != NULL);
    OCASSERT(container != NULL);
    OCASSERT(field != NULL);

    template = container->ocdata->template;

    if(!iscontainer(template->octype))
	return OCTHROW(OC_EBADTYPE);

    /* Validate index */
    if(index >= container->ocdata->ninstances)
	return OCTHROW(OC_EINDEX);

    field->parent = container;
    field->ocdata = container->ocdata->instances[index];
    field->mode = OCDT_FIELD;

    octrace("ithfield", state, field);

    return OC_NOERR;
}

OCerror
ocinstance_ithelement(OCstate* state, OCinstance* instance,
                      size_t* indices,
		      OCinstance* element
                      )
{
    int stat = OC_NOERR;
    OCnode* template;
    size_t index,rank;

    OCASSERT(state != NULL);
    OCASSERT(instance != NULL);
    OCASSERT(element != NULL);

    template = instance->ocdata->template;
    rank = template->array.rank;

    /* Must be a dimensioned Structure */
    if(template->octype != OC_Structure || rank == 0)
	return OCTHROW(OC_EBADTYPE);

    /* Validate indices */
    if(!ocvalidateindices(rank,template->array.sizes,indices))
	return OCTHROW(OC_EINVALCOORDS);

    /* compute linearized index */
    index = ocarrayoffset(rank,template->array.sizes,indices);

    if(index >= instance->ocdata->ninstances)
	return OCTHROW(OC_EINDEX);

    element->parent = instance;
    element->ocdata = instance->ocdata->instances[index];
    element->mode = OCDT_ELEMENT;

    octrace("ithelement", state, element);

    return OCTHROW(stat);
}

/* Move to the ith sequence record. */
OCerror
ocinstance_ithrecord(OCstate* state, OCinstance* instance,
                      size_t index, /* record number */
		      OCinstance* record
                      )
{
    int stat = OC_NOERR;
    OCnode* template;

    OCASSERT(state != NULL);
    OCASSERT(instance != NULL);
    OCASSERT(record != NULL);

    template = instance->ocdata->template;

    /* Must be a Sequence */
    if(template->octype != OC_Sequence)
	return OCTHROW(OC_EBADTYPE);

    /* Validate index */
    if(index >= instance->ocdata->ninstances)
	return OCTHROW(OC_EINDEX);

    record->parent = instance;
    record->ocdata = instance->ocdata->instances[index];
    record->mode = OCDT_RECORD;

    octrace("ithrecord", state, record);

    return OCTHROW(stat);
}

OCerror
ocinstance_position(OCstate* state, OCinstance* instance, size_t* indices)
{
    OCnode* template;
    OCdata* data;

    OCASSERT(state != NULL);
    OCASSERT(instance != NULL);
    OCASSERT(indices != NULL);

    data = instance->ocdata;
    template = data->template;
    if(data->datamode == OCDT_RECORD)
	indices[0] = data->index;
    else if(data->datamode == OCDT_ELEMENT) {
	/* Convert the linearized array index into a set of indices */
	ocarrayindices(data->index,template->array.rank,template->array.sizes,indices);
    } else
	return OCTHROW(OC_EBADTYPE);
    return OCTHROW(OC_NOERR);
}

OCerror
ocinstance_recordcount(OCstate* state, OCinstance* instance, size_t* countp)
{
    OCdata* data;

    OCASSERT(state != NULL);
    OCASSERT(instance != NULL);
    OCASSERT(countp != NULL);

    data = instance->ocdata;
    if(data->template->octype != OC_Sequence)
	return OCTHROW(OC_EBADTYPE);
    if(data->datamode == OCDT_RECORD) {
	data = data->container;
        OCASSERT(data->template->octype == OC_Sequence
		 && data->datamode == OCDT_OTHER);
    }

    *countp = data->ninstances;		

    return OC_NOERR;
}

/**************************************************/
/*
In order to actually extract data, one must move to the
specific primitive typed field containing the data of
interest by using ocinstance_fieldith().  Then this procedure
is invoked to extract some subsequence of items from the
field.  For scalars, the start and count are ignored.
Note that start and count are linearized from the oc_instance_read
arguments.
*/

int
ocinstance_read(OCstate* state, OCinstance* instance, size_t start, size_t count,
		void* memory, size_t memsize)
             
{
    int stat = OC_NOERR;
    XXDR* xdrs;
    OCtype etype, octype;
    int isscalar;
    size_t elemsize, totalsize;
    OCnode* template;
    OCdata* data;

    octrace("read", state, instance);

    assert(state != NULL);
    assert(instance != NULL);
    assert(memory != NULL);
    assert(memsize > 0);

    data = instance->ocdata;
    template = data->template;
    octype = template->octype;
    assert(octype == OC_Atomic);
    etype = template->etype;

    isscalar = (template->array.rank == 0 ? 1 : 0);

    /* validate memory space*/
    elemsize = octypesize(etype);
    totalsize = elemsize*data->ninstances;
    if(memsize < totalsize)
	return OCTHROW(OC_EINVAL);

    /* Get XXDR* */
    xdrs = template->root->tree->data.xdrs;

    if(isscalar) {
        /* Extract the data */
        stat = ocread(data,xdrs,(char*)memory,memsize,0,1);
    } else {
        /* Validate the start and count */
        if(start >= data->ninstances
           || (start+count) > data->ninstances)
	    return OCTHROW(OC_EINVALCOORDS);
        /* Extract the data */
        stat = ocread(data,xdrs,(char*)memory,memsize,start,count);
    }

    return OCTHROW(stat);
}

/**************************************************/
/*
Extract data from a leaf into memory.
*/

static OCerror
ocread(OCdata* data, XXDR* xdrs, char* memory, size_t memsize, size_t start, size_t count)
{
    int i;
    OCnode* template;
    OCtype etype;
    off_t elemsize, totalsize, xdrtotal, xdrstart;
    int scalar;

    OCASSERT(data != NULL);
    OCASSERT(memory != NULL);
    OCASSERT(memsize > 0);
    OCASSERT(count > 0);
    OCASSERT((start+count) <= data->ninstances);

    template = data->template;
    etype = template->etype;
    scalar = (template->array.rank == 0);

    xdrtotal = count*data->xdrsize; /* amount (in xdr sizes) to read */
    xdrstart = start*data->xdrsize; /* offset from the start of the data */

    elemsize = octypesize(etype); /* wrt memory, not xdrsize */
    totalsize = elemsize*count;

    /* validate memory space*/
    if(memsize < totalsize) return OCTHROW(OC_EINVAL);

    /* copy out with appropriate byte-order conversions */
    switch (etype) {

    case OC_Int32: case OC_UInt32: case OC_Float32:
	xxdr_setpos(xdrs,data->xdroffset+xdrstart);
	if(!xxdr_getbytes(xdrs,memory,xdrtotal)) {OCTHROW(OC_EDATADDS); goto xdrfail;}
	if(!xxdr_network_order) {
	    unsigned int* p;
	    for(p=(unsigned int*)memory,i=0;i<count;i++,p++) {
		swapinline32(p);
	    }
	}
	break;
	
    case OC_Int64: case OC_UInt64:
	xxdr_setpos(xdrs,data->xdroffset+xdrstart);
	if(!xxdr_getbytes(xdrs,memory,xdrtotal)) {OCTHROW(OC_EDATADDS); goto xdrfail;}
	if(!xxdr_network_order) {
	    unsigned long long* llp;
	    for(llp=(unsigned long long*)memory,i=0;i<count;i++,llp++) {
		swapinline64(llp);
	    }
	}
        break;

    case OC_Float64:
	xxdr_setpos(xdrs,data->xdroffset+xdrstart);
	if(!xxdr_getbytes(xdrs,memory,xdrtotal)) {OCTHROW(OC_EDATADDS); goto xdrfail;}
	{
	    double* dp;
	    for(dp=(double*)memory,i=0;i<count;i++,dp++) {
		double swap;
		xxdrntohdouble((char*)dp,&swap);
		*dp = swap;
	    }
	}
	break;

    /* non-packed fixed length, but memory size < xdrsize */
    case OC_Int16: case OC_UInt16: {
	/* In order to avoid allocating a lot of space, we do this one int at a time */
	unsigned int ii;
	unsigned short* sp = (unsigned short*)memory;
	xxdr_setpos(xdrs,data->xdroffset+xdrstart);
	for(i=0;i<count;i++,sp++) {
	    if(!xxdr_uint(xdrs,&ii)) {OCTHROW(OC_EDATADDS); goto xdrfail;}
   	    *sp = (unsigned short)ii;
	}
	} break;

    /* Do the byte types, packed/unpacked */
    case OC_Byte:
    case OC_UByte:
    case OC_Char:
	if(scalar) {
	    xxdr_setpos(xdrs,data->xdroffset+xdrstart);
	    if(!xxdr_getbytes(xdrs,memory,XDRUNIT)) {OCTHROW(OC_EDATADDS); goto xdrfail;}
	} else {
	    /* the xdroffset will always be at the start of the
               packed data, so we need to add the start count to it */
	    xxdr_setpos(xdrs,data->xdroffset+xdrstart);
	    if(!xxdr_getbytes(xdrs,memory,xdrtotal)) {OCTHROW(OC_EDATADDS); goto xdrfail;}
	}
	break;

    /* Hard case, because strings are variable length */
    case OC_String: case OC_URL: {
	/* We have to do this one by one */
	char** sp = (char**)memory;
	if(count > data->nstrings)
	    return OCTHROW(OC_EDATADDS);
        xxdr_setpos(xdrs,data->xdroffset+xdrstart);
	for(i=0;i<count;i++,sp++) {
            /* Move into position */
	    off_t len;
	    if(!xxdr_string(xdrs,sp,&len)) {OCTHROW(OC_EDATADDS); goto xdrfail;}
	}
        } break;

    default: OCPANIC("unexpected etype"); break;
    }

    return OC_NOERR;

xdrfail:
    oc_log(LOGERR,"DAP DATADDS packet is apparently too short");
    return OCTHROW(OC_EDATADDS);
}

