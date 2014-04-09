/* Copyright 2009, UCAR/Unidata and OPeNDAP, Inc.
   See the COPYRIGHT file for more information. */

#ifndef OCUTIL_H
#define OCUTIL_H 1

/* Forward */
struct OCstate;

#define ocmax(x,y) ((x) > (y) ? (x) : (y))

extern char* ocstrndup(const char* s, size_t len);
extern int ocstrncmp(const char* s1, const char* s2, size_t len);

extern int occopycat(char* dst, size_t size, size_t n, ...);
extern int occoncat(char* dst, size_t size, size_t n, ...);

extern size_t octypesize(OCtype etype);
extern char*  octypename(OCtype octype);
extern char*  octypetostring(OCtype octype);
extern char* ocerrstring(int err);
extern OCerror ocsvcerrordata(OCstate*, char** codep,
                              char** msgp, char** contextp, char** otherinfop,
                              long* httpcodep);

extern OCerror octypeprint(OCtype etype, void* value, size_t bufsize, char* buf);
extern size_t xxdrsize(OCtype etype);

extern int oc_ispacked(OCnode* node);

extern size_t octotaldimsize(size_t,size_t*);

extern size_t ocarrayoffset(size_t rank, size_t*, size_t*);
extern void ocarrayindices(size_t index, size_t rank, size_t*, size_t*);
extern size_t ocedgeoffset(size_t rank, size_t*, size_t*);

extern int ocvalidateindices(size_t rank, size_t*, size_t*);

extern void ocmakedimlist(OClist* path, OClist* dims);

extern int ocfindbod(OCbytes* buffer, size_t*, size_t*);

/* Reclaimers*/
extern void ocfreeprojectionclause(OCprojectionclause* clause);

/* Misc. */

extern int ocmktmp(const char* base, char** tmpnamep, int* fdp);

extern void ocdataddsmsg(struct OCstate*, struct OCtree*);

extern const char* ocdtmodestring(OCDT mode,int compact);

/* Define some classifiers */
#define iscontainer(t) ((t) == OC_Dataset || (t) == OC_Structure || (t) == OC_Sequence || (t) == OC_Grid || (t) == OC_Attributeset)

#define isatomic(t) ((t) == OC_Atomic)

/* DAP4 */
extern OCnode* ocfindbyname(OCnode* node, const char* name);
extern OCnode* ocfindbyfqn(OCnode* root, const char* fqn0);

#endif /*UTIL_H*/
