/* Copyright 2014, UCAR/Unidata and OPeNDAP, Inc.
   See the COPYRIGHT dap for more information. */

/*
OC External Interface
Created: 4/4/2009
Last Revised: 12/23/2014
Version: 2.1
*/

#ifndef OC_H
#define OC_H

#include <stdlib.h>
#include <stdio.h>
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

/*!\file oc.h
*/

/* Note that the terms DMR or DDS have in many
   places been replaced with the more generic
   term "metadata" or "meta". The term DATADDS
   has been replaced by the more generic term "data".

   Also note that in DAP2, a Dataset node is treated
   like a Structure. In DAP4, it is treated like a Group.

*/

/*!\defgroup Definitions Constants, types, etc.
@{*/

/*! OC_MAX_RANK is the maximum allowable variable rank.*/
#define OC_MAX_RANK 1024

/*! OC_MAX_DIMENSIONS is a synonym for OC_MAX_RANK. */
#define OC_MAX_DIMENSIONS OC_MAX_RANK

/*! OC_MAX_NESTING is the maximum allowable nesting depth.*/
#define OC_MAX_NESTING 1024

/*!\enum OCdxd
Define the legal kinds of fetch: DAS, META, DATA.
*/
typedef enum OCdxd {
OCMETA=0,
OCDAS=1, /* Left for DAP2 only */
OCDATA=2,
} OCdxd;

/*! Aliases for backward compatibility */
#define OCDDS OCMETA
#define OCDATADDS OCDATA

/*!
\typedef OCflags
Define flags for oc_fetch.
*/
typedef int OCflags;
/*!\def OCONDISK
Cause oc_fetch to store the retrieved data on disk.
*/

#define OCONDISK 1

/**************************************************/
/* OCtype */

/*!\enum OCtype
Define the set of legal types. The set is divided
into two parts. The atomic types define
leaf nodes of the metadata (DDS or DMR). The non-atomic types
are used to tag internal nodes of the metadata tree.
*/
typedef enum OCtype {
/* Atomic Types */
OC_NAT=0,
OC_Char=1,
OC_Int8=2,
OC_UInt8=3,
OC_Int16=4,
OC_UInt16=5,
OC_Int32=6,
OC_UInt32=7,
OC_Int64=8,
OC_UInt64=9,
OC_Float32=10,
OC_Float64=11,
OC_String=12,
OC_URL=13,

/* DAP4 Additions */
OC_Opaque=14,
OC_Enum=15,

/* Non-Atomic DAP2 Types */
OC_Atomic=100,
OC_Dataset=101,
OC_Grid=102,
OC_Structure=103,
OC_Dimension=104,
OC_Attribute=105,
OC_Attributeset=106,

/* Non-Atomic DAP4 Additions*/
OC_Group=130,
OC_Sequence=131,
OC_Map=132,
OC_Enumeration=133, /*=> Enumeration definition, OC_Enum => reference */
OC_XML=134,

/* Change this if you add to the end */
#define OC_TYPE_LAST OC_XML
} OCtype;

/*!\enum OCerror
Define the set of error return codes.
The set consists of oc errors (negative
values) plus the set of system errors, which
are positive.
*/
typedef enum OCerror {
OC_NOERR=0,
OC_EBADID=-1,
OC_ECHAR=-2,
OC_EDIMSIZE=-3,
OC_EEDGE=-4,
OC_EINVAL=-5,
OC_EINVALCOORDS=-6,
OC_ENOMEM=-7,
OC_ENOTVAR=-8,
OC_EPERM=-9,
OC_ESTRIDE=-10,
OC_EDAP=-11,
OC_EXDR=-12,
OC_ECURL=-13,
OC_EBADURL=-14,
OC_EBADVAR=-15,
OC_EOPEN=-16,
OC_EIO=-17,
OC_ENODATA=-18,
OC_EDAPSVC=-19,
OC_ENAMEINUSE=-20,
OC_EDAS=-21,
/* DDS is deprecated in favor of "META" */
OC_EMETA=-22,
#define OC_EDDS OC_EMETA
/* DATADDS deprecated in favor of  DATA generically */
OC_EDATA=-23,
#define OC_EDATADDS OC_EDATA
OC_ERCFILE=-24,
OC_ENOFILE=-25,
OC_EINDEX=-26,
OC_EBADTYPE=-27,
OC_ESCALAR=-28,
OC_EOVERRUN=-29,
OC_ERANK=-30,
OC_EPROTOCOL=-31,
OC_EAUTH=-32,

/* Following are added for DAP4; leave a gap to extend dap2 error set */
OC_ERESPONSE=-50, /* Error response was returned */
} OCerror;

/*!\def OCLOGNOTE
Tag a log entry as a note.
*/
#define OCLOGNOTE 0
/*!\def OCLOGWARN
Tag a log entry as a warning.
*/
#define OCLOGWARN 1
/*!\def OCLOGERR
Tag a log entry as an error.
*/
#define OCLOGERR  2
/*!\def OCLOGDBG
Tag a log entry as a debug note.
*/
#define OCLOGDBG  3

/**************************************************/
/* Define the opaque types */

/*!\typedef OCobject
Define a common opaque type.
*/
typedef void* OCobject;

/*!\typedef OCmetanode
The OCmetanode type provide a reference to a component of a
metadata tree: e.g. Sequence, Grid, Dataset, etc.  These
objects are nested, so most objects reference a container
object and subnode objects. The term metadatanode
also covers DAS nodes where appropriate.
*/

typedef OCobject OCmetanode;

/*!\typedef OCdasnode
The OCdasnode is a alias for OCmetanode.
*/

typedef OCmetanode OCdasnode;

/* Data data type */
/*!\typedef OCdatanode
The OCdatanode type provide a reference
to a specific piece of data in the data
part of a server response.
*/
typedef OCobject OCdatanode;

/*!\typedef OClink
Think of OClink as analogous to the C stdio FILE structure;
it "holds" all the other state info about
a connection to the server, the url request, and the response info.
3/24/210: Renamed from OCconnection because of confusion about
term "connection"
*/
typedef OCobject OClink;

/**@}*/

/**************************************************/
/* External API */

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************/
/* Link management */

extern OCerror oc_open(const char* url, OClink*);
extern OCerror oc_close(OClink);

/**************************************************/
/* Tree Management */

extern OCerror oc_fetch(OClink,
			const char* constraint,
			OCdxd,
			OCflags,
			OCmetanode*);

extern OCerror oc_root_free(OClink, OCmetanode root);
extern const char* oc_tree_text(OClink, OCmetanode root);

/**************************************************/
/* Node Management */

extern OCerror oc_meta_properties(OClink, OCmetanode,
		  char** namep,
		  OCtype* typep,
		  OCtype* atomictypep, /* if octype == OC_Atomic */
		  OCmetanode* containerp,  /* NULL if octype == OC_Dataset */
		  size_t* rankp,       /* 0 if scalar */
		  size_t* nsubnodesp,
		  size_t* nattrp);

/* Define some individual accessors for convenience */

extern OCerror oc_meta_name(OClink,OCmetanode,char**);
extern OCerror oc_meta_class(OClink,OCmetanode,OCtype*);
extern OCerror oc_meta_basetype(OClink,OCmetanode,OCtype*);
extern OCerror oc_meta_nsubnodes(OClink,OCmetanode,size_t*);
extern OCerror oc_meta_rank(OClink,OCmetanode,size_t*);
extern OCerror oc_meta_attr_count(OClink,OCmetanode,size_t*);
extern OCerror oc_meta_root(OClink,OCmetanode,OCmetanode*);
extern OCerror oc_meta_container(OClink,OCmetanode,OCmetanode*);

/* Aliases */
#define oc_meta_octype oc_meta_class
#define oc_meta_type oc_meta_class

/* Get the i'th field of the given (container) node; return OC_EINDEX
   if there is no such node; return OC_EBADTYPE if node is not
   a container
*/
extern OCerror oc_meta_ithfield(OClink, OCmetanode, size_t index, OCmetanode* ithfieldp);

/* Alias for oc_meta_ithfield */
extern OCerror oc_meta_ithsubnode(OClink, OCmetanode, size_t, OCmetanode*);

/* Convenience functions that are just combinations of ithfield with other functions */

/* Return the grid array dds node from the specified grid node*/
extern OCerror oc_meta_gridarray(OClink, OCmetanode grid, OCmetanode* arrayp);

/* Return the i'th grid map dds node from the specified grid dds node.
   NOTE: Map indices start at ZERO.
*/
extern OCerror oc_meta_gridmap(OClink, OCmetanode grid, size_t index, OCmetanode* mapp);

/* Retrieve a contained declaration  by name from a
   structure, or sequence, dataset, or if DAP4, a group.
   return OC_EBADTYPE if parent is not a container.
   return OC_EINDEX if no field by the given name is found.
*/
extern OCerror oc_meta_declbyname(OClink, OCmetanode, const char* name, OCmetanode* declp);

/* Return the dimension nodes, if any, associated with a given meta node */
/* Caller must allocate and free the vector for dimids */
/* If the node is scalar, then return OC_ESCALAR. */
extern OCerror oc_meta_dimensions(OClink, OCmetanode, OCmetanode* dimids);

/* Return the i'th dimension node, if any, associated with a given object */
/* If there is no such dimension, then return OC_EINVAL */
extern OCerror oc_meta_ithdimension(OClink,OCmetanode, size_t, OCmetanode*);

/* Return the size and name associated with a given dimension object */
extern OCerror oc_dimension_properties(OClink,OCmetanode,size_t*,char**);

/* For convenience, return only the dimension sizes */
extern OCerror oc_meta_dimensionsizes(OClink,OCmetanode,size_t* dimsizes);

/* Attribute Management */

/* Obtain the attributes associated with a given OCmetanode.
   One specifies a group or dataset node to get the global attributes.
   The actual attribute strings are returned and the user
   must do any required conversion based on the octype.
   The strings vector must be allocated and freed by caller,
   The contents of the strings vector must also be reclaimed
   using oc_attr_reclaim(see below). 
   Standard practice is to call twice, once with the 'strings'
   argument == NULL to get the number of values,
   then the second time with an allocated char** vector.
*/
extern OCerror oc_meta_attr(OClink,OCmetanode, size_t i,
			    char** name, OCtype* octype,
			    size_t* nvalues, char** strings);

/* DAP2 only: Access ith value string of a DAS OC_Attribute object.
   OCmetanode of the object is assumed to be OC_Attribute.
   Note that this is  different than the above oc_meta_attr
   that works on non-DAS metadata nodes.
   Note also that the return value is always a string.
   Caller must free returned string.
*/

extern OCerror oc_das_attr_count(OClink, OCmetanode, size_t* countp);

extern OCerror oc_das_attr(OClink,OCmetanode, size_t, OCtype*, char**);

/**************************************************/
/* Free up a metadata node that is no longer being used */
extern OCerror oc_meta_free(OClink, OCmetanode);

/**************************************************/
/* Data Procedures */

/* Given the metadata tree root, get the corresponding root data from a data response */
extern OCerror oc_meta_getdataroot(OClink, OCmetanode treeroot, OCdatanode* rootp);

/* Alias for oc_meta_getdataroot */
#define oc_data_getroot oc_meta_getdataroot

/* Return the data of the container (Sequence, Grid, or Structure)
   for the specified data. If it does not exist, then return NULL.
   In effect this procedure allows one to walk up the datatree one level.
   Remember that groups do not themselves have any associated data.
*/
extern OCerror oc_data_container(OClink, OCdatanode data, OCdatanode* containerp);

/* Return the root node of the data tree that contains the specified data node.
   In effect this procedure allows one to walk to the root of the datatree
   containing the specified datanode.
*/
extern OCerror oc_data_root(OClink, OCdatanode data, OCdatanode* rootp);

/*
There are multiple ways to walk down a level in a data tree
depending on what kind of node we are currently visiting.
The possibilities are:
1. current node is the data for a container:
   grid, structure or sequence or (for DAP2) a dataset node:
   => oc_data_ithfield -- get the data node corresponding
                          to the ith field of the container.
2. current node is the data specifically for a grid node:
   => oc_data_gridarray -- get the data node for the grid array
   => oc_data_gridmap -- get the data node for the ith grid map
3. current node is the data for an array of structures or sequences
   => oc_data_ithelement -- get the data node corresponding to
                            the i'th structure or sequence in the array
                            If the structure/sequence is scalar, then the indices
                            are ignored.
4. current node is the data for a sequence
   => oc_data_ithrecord -- get the data node corresponding to
                            the i'th record in the sequence

Note that the above only apply to compound objects. Once
you have the data node for an atomic types object (dimensioned
or scalar), you can read its contents using oc_data_read
or oc_data_readscalar.
*/

/* Return the data node for the i'th field of the specified container data */
extern OCerror oc_data_ithfield(OClink, OCdatanode container, size_t index,
                                OCdatanode* fieldp);

/* Retrieve the data node by name from a container data node */
extern OCerror oc_dat_fieldbyname(OClink, OCdatanode, const char* name, OCdatanode* fieldp);

/* Return the grid array data for the specified grid data */
extern OCerror oc_data_gridarray(OClink, OCdatanode grid, OCdatanode* arrayp);

/* Return the i'th grid map data for the specified grid data.
   NOTE: Map indices start at ZERO.
*/
extern OCerror oc_data_gridmap(OClink, OCdatanode grid, size_t index, OCdatanode* mapp);

/* Return the data of a dimensioned Structure or Sequence corresponding
   to the element specified by the indices.
*/
extern OCerror oc_data_ithelement(OClink, OCdatanode data, size_t* indices, OCdatanode* elementp);

/* Return the i'th record data of a Sequence data. */
extern OCerror oc_data_ithrecord(OClink, OCdatanode data, size_t index, OCdatanode* recordp);

/* Free up data that is no longer being used */
extern OCerror oc_data_free(OClink, OCdatanode data);

/* Get a count of the number of records associated with a sequence */
extern OCerror oc_data_recordcount(OClink, OCdatanode, size_t*);

/* Return the actual data values associated with the specified leaf data.
   The OCdatanode is assumed to be referencing a leaf node that is
   either a atomic valued scalar or array.
   If scalar, then index and count are ignored.
   Caller is responsible for allocating memory(of proper size)
   and free'ing it.
   See also oc_meta_read().
   WARNING: for opaque and string instances, space will be allocated
   dynamically to hold the content for each instance. This must be
   free'd by the caller also.
*/
extern OCerror oc_data_read(OClink, OCdatanode, size_t*, size_t*, size_t, void*);

/* Like oc_data_read, but for reading a scalar.
   Caller is responsible for allocating memory(of proper size)
   and free'ing it.
   See also oc_meta_readscalar().
*/
extern OCerror oc_data_readscalar(OClink, OCdatanode, size_t, void*);

/* Like oc_data_read, but caller provides a starting set of indices
   and count of the number of elements to read.
   Caller is responsible for allocating memory(of proper size)
   and free'ing it.
   See also oc_meta_readn().
*/
extern OCerror oc_data_readn(OClink, OCdatanode, size_t*, size_t, size_t, void*);

/* Return the indices for this data instance; Assumes the data
   was obtained using oc_data_ithelement or oc_data_ithrecord;
   if not, then an error is returned.
*/
extern OCerror oc_data_position(OClink, OCdatanode data, size_t* indices);

/* Return the pattern dds node for an data */
extern OCerror oc_data_ddsnode(OClink, OCdatanode data, OCddsnode*);

/* Return the octype of the data (convenience) */
extern OCerror oc_data_octype(OClink, OCdatanode data, OCtype*);

/* Return 1 if the specified data has a valid index, 0 otherwise.
   Valid index means it was created using
   oc_data_ithelement or oc_data_ithrecord.
*/
extern int oc_data_indexed(OClink link, OCdatanode datanode);

/* Return 1 if the specified data can be indexed
   Indexable means the data is pointing to
   an indexed structure or to a sequence.
*/
extern int oc_data_indexable(OClink, OCdatanode);

/**************************************************/
/* 
For top-level, atomic variables, it is possible to directly
read the associated data without having to use the oc_data_XXX
procedures. Provide special procedures to support this.
*/

/* See oc_data_read for semantics */
extern OCerror oc_meta_read(OClink, OCmetanode, size_t*, size_t*, size_t, void*);

/* See oc_data_readscalar for semantics */
extern OCerror oc_meta_readscalar(OClink, OCmetanode, size_t, void*);

/* See oc_data_readn for semantics */
extern OCerror oc_meta_readn(OClink, OCmetanode, size_t*, size_t, size_t, void*);

/**************************************************/
/* Misc. OCtype-related functions */

/* Return size of the given type(Atomic only) */
extern size_t oc_typesize(OCtype);

/* Return a canonical printable string describing a given type:
   e.g. Byte, Int16, etc.
*/
extern const char* oc_typetostring(OCtype);

/* Given a value of a atomic OC type, provide a canonical
   string representing that value; mostly for debugging.
*/
extern OCerror oc_typeprint(OCtype, void* value, size_t bufsize, char* buf);

/**************************************************/
/* Logging */

extern void oc_loginit(void);
extern int  oc_setlogging(int onoff); /* 1=>start logging 0=>stop */
extern int  oc_logopen(const char* logfilename);
extern void oc_logclose(void);

extern void oc_log(int tag, const char* fmt, ...);

extern void oc_logtext(int tag, const char* text);

/**************************************************/
/* Miscellaneous */

/* Reclaim the strings within a string vector, but not the vector itself.
   This is useful for reclaiming the result of oc_data_read
   or oc_meta_attr when the type is OC_String or OC_URL.
   Note that only the strings are reclaimed, the string vector
   is not reclaimed because it was presumably allocated by the client.
*/
extern void oc_reclaim_strings(size_t n, char** svec);

/* Convert an OCerror to a human readable string */
extern const char* oc_errstring(OCerror err);

/* Get client parameters from the URL
   DO NOT free the result
*/
extern const char* oc_clientparam_get(OClink, const char* param);

/**************************************************/
/* Merging operations */

/* DAP2 only: Merge a specified DAS into a specified DDS or DATADDS */
extern OCerror oc_merge_das(OClink, OCmetanode dasroot, OCmetanode ddsroot);

/**************************************************/
/* Debugging */

/* Get the HTTP return code from the last call;
   note that this may or may not be the same as returned
   by oc_svcerrordata.
 */
extern int oc_httpcode(OClink);

/*
(Re-)initialize the oc library as if nothing had been called.
This is primarily for debugging of rc files.
*/
extern OCerror oc_initialize(void);

/**************************************************/
/* Curl options */
/* This is here because trial and error shows that
   libcurl shows thru too much. So bow to the inevitable.
*/

/*Cause the curl library to be verbose and save error messages*/
extern OCerror oc_trace_curl(OClink link);

/* Allow specification of the rc file */
extern OCerror oc_set_rcfile(const char* filepath);

/* Allow specification of the netrc file */
extern OCerror oc_set_netrc(OClink*, const char* filepath);

/* Set arbitrary curl option */
extern OCerror oc_set_curlopt(OClink link, const char* option, void* value);

/**************************************************/
/* Experimental/Undocumented */

/* Given an arbitrary OCnode, return the connection of which it is a part */
extern OCerror oc_get_connection(OCobject ocnode, OCobject* linkp);

/* Resend a url as a head request to check the Last-Modified time */
extern OCerror oc_update_lastmodified_data(OClink);

/* Get last known modification time; -1 => data unknown */
extern long oc_get_lastmodified_data(OClink);

/* Test if a given url responds to a DAP protocol request */
extern OCerror oc_ping(const char* url);

/* Return the size of the in-memory or on-disk
   data chunk returned by the server for a given tree.
   Zero implies it is not defined.
*/

/* For some reason, the MSVC compiler doesn't like this. */
#ifndef _WIN32
extern OCerror oc_raw_xdrsize(OClink,OCmetanode,off_t*);
#endif

#ifdef __cplusplus
}
#endif

#endif /*OC_H*/
