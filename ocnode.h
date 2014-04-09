/* Copyright 2009, UCAR/Unidata and OPeNDAP, Inc.
   See the COPYRIGHT file for more information. */

#ifndef OCNODE_H
#define OCNODE_H

/*! Specifies the Diminfo. */

/* Track info purely about declared dimensions.
   More information is included in the Dimdata structure (dim.h)
*/
typedef struct OCdiminfo {
    struct OCnode* array;   /* defining array node (if known) DAP2*/
    size_t arrayindex;	    /* rank position ofthis dimension in the array DAP2*/
    long long declsize; /* from DDS/DMR */
    int anonymous; /* => it is has no name */
} OCdiminfo;

/*! Specifies the Arrayinfo.*/
typedef struct OCarrayinfo {
    /* The complete set of dimension info applicable to this node*/
    OClist*  dimensions; /* List<OCnode*> */
    /* cached (because they are computed so often*/
    size_t rank; /* == |dimensions|*/
    size_t* sizes;
    size_t  totalsize; /* product of the sizes */
} OCarrayinfo;

/*! Specifies Attribute info */
typedef struct OCattribute {
    char*   name;
    OCtype etype; /* type of the attribute */
    size_t  nvalues;
    char**  values;  /* |values| = nvalues*sizeof(char**)*/
} OCattribute;

/*! Specifies the Attinfo.*/
/* For DAP2, This is the form as it comes out of the DAS parser; For Dap4,
   only values and nslist are used
 */
typedef struct OCattinfo {
    OClist* values; /* oclist<char*>*/
    int isglobal; /* is this supposed to be a global attribute set?*/
    int isdods;   /* is this a global DODS_XXX  attribute set */
    OClist* nslist;
} OCattinfo;

/*! Specifies an Enum Constant (DAp4 only).*/
typedef struct OCeconst {
    char* name;
    unsigned long long value;
} OCeconst;

/*! Specifies a Map (DAP4 only).*/
typedef struct OCmap {
    OCnode*    variable; /* to which this map refers */
    OCnode*    parent; /* Parent containing this map reference */
} OCmap;

/* Indicate the kind of OCDOM node */
#define DOMFLAG_ELEMENT 1 
#define DOMFLAG_ATTR    2 
#define DOMFLAG_TEXT    4

/**
The OCDOM node represents xml in a pseudo-dom format.
*/

typedef struct OCDOM {
    int flags;
    char* name; /* element name or attribute name */
    char* text; /* attribute value or text text */
    OClist* subnodes; /* OClist<OCDOM*> */
    OClist* attrs; /* OClist<OCDOM*> associated attributes if this is an element */
} OCDOM;

/*! Specifies an OtherXML Node.*/
typedef struct OCxmlinfo {
    OCDOM* document;    
} OCxmlinfo;

/*! Specifies the OCnode.*/

/**

The OCnode structure is a bit of a catchall.  In a properly
object oriented language, there would be a number of
subclasses based on the octype.

This can be approximated in C using a union type,
but saving the space is not particularly an issue
since the sizeof(OCnode) is not large.
Instead, and where useful, OCtype specific
data is pushed off to structs (like OCdiminfo).
Also, some fields are specific to the protocol: DAP2 or DAP4.

I suppose that this should be cleaned up so one
can tell easily which fields are used by which
OCtype.

*/

struct OCnode {
    /* Common to all OCnode instances */
    OCheader header; /* class=OC_Node */
    OCtype	    octype;
    char*           name;
    char*           fullname;
    struct OCnode*  container; /* this node is subnode of container */
    struct OCnode*  root;      /* root node of tree containing this node */
    int             order;     /* Impose a linear order on all nodes in the tree */

    /* All octypes that can have attributes; note that this differs
       between DAP2 and DAP4 */
    OClist*         attributes; /* oclist<OCattribute*>*/

    /* OC_Dataset */
    struct OCtree*  tree;      /* !NULL iff this is a root node */
    struct OCnode*  datadds;   /* correlated datadds node, if any (DAP2)*/

    /* OC_Dimension */
    OCdiminfo       dim;       /* octype == OC_Dimension*/

    /* OC_Atomic, OC_Structure */
    /* also OC_Sequence (DAP4 only) */
    OCarrayinfo     array;     /* octype == {OC_Structure, OC_Primitive}*/

    /* OC_Atomic, OC_Enumeration */
    OCtype          basetype;

    /* DAP4: OC_Atomic when basetype is OC_Enum */
    OCnode*         enumdef; /* The defining enumeration declaration */

    /* For "container types" loosely defined. */
    /* DAP2 and DAP4: OC_Structure, OC_Sequence, OC_Dataset, OC_Attributeset */
    /* DAP4: OtherXML */
    OClist*         subnodes; /*OClist<OCnode*>*/

    /* OC_Attribute */
    OCattinfo       att;
    OClist* attributes; /* oclist<OCattribute*> DAP2*/

    /* OC_Atomic */
    OCdata*         data; /* Defined only if this node is a top-level atomic variable*/

    /* OC_Enumeration */
    OClist*         econsts; /* OClist<OCeconst*> */

    /* DAP4: OC_Atomic, OC_Structure, OC_Sequence */
    OClist*         maps; /* List<OCmap> */

    /* DAP4: OC_XML */
    OCxmlinfo           xml;
};

#if SIZEOF_SIZE_T == 4
#define OCINDETERMINATE  ((size_t)0xffffffff)
#endif
#if SIZEOF_SIZE_T == 8
#define OCINDETERMINATE  ((size_t)0xffffffffffffffff)
#endif

extern OCnode* ocnode_new(char* name, OCtype ptype, OCnode* root);
extern void occollectpathtonode(OCnode* node, OClist* path);
extern void occomputefullnames(OCnode* root);
extern void occomputesemantics(OClist*);
//fix extern void ocaddattribute(OCattribute* attr, OCnode* parent);
//fix extern OCattribute* ocmakeattribute(char* name, OCtype ptype, OClist* values);
extern size_t ocsetsize(OCnode* node);
extern OCerror occorrelate(OCnode*,OCnode*);
extern void ocmarkcacheable(OCstate* state, OCnode* ddsroot);

extern void octree_free(struct OCtree* tree);
extern void ocroot_free(OCnode* root);
extern void ocnodes_free(OClist*);
extern void ocnode_free(OCnode*);
extern void ocdata_free(OCstate* state, OCdata* data);

/* Merge DAS with DDS or DATADDS*/
extern OCerror ocddsdasmerge(struct OCstate*, OCnode* das, OCnode* dds);

#endif /*OCNODE_H*/
