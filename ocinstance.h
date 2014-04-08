/* Copyright 2009, UCAR/Unidata and OPeNDAP, Inc.
   See the COPYRIGHT file for more information. */

#ifndef OCINSTANCE_H
#define OCINSTANCE_H

/*! Specifies internal OCinstance structure */
struct OCinstance {
    OCheader     header;   /* class=OC_Instance */
    OCinstance*  next;     /* chain of known instances */
    OCDT         mode;
    OCinstance*  parent;
    OCdata*      ocdata;
};

extern OCinstance* ocinstance_new(OCstate*,OCinstance*);
extern void ocinstance_free(OCstate*, OCinstance*);

extern OCerror ocinstance_ithfield(OCstate*, OCinstance* container, size_t index, OCinstance* field);
extern OCerror ocinstance_container(OCstate*, OCinstance* instance, OCinstance* container);
extern OCerror ocinstance_root(OCstate*, OCinstance* instance, OCinstance* root);

extern OCerror ocinstance_ithelement(OCstate*, OCinstance* instance, size_t* indices, OCinstance* element);
extern OCerror ocinstance_ithrecord(OCstate*, OCinstance* instance, size_t index, OCinstance* record);

extern OCerror ocinstance_position(OCstate*, OCinstance* instance, size_t* indices);
extern OCerror ocinstance_recordcount(OCstate*, OCinstance*, size_t*);

extern OCerror ocinstance_attach(OCstate*, OCnode*, OCinstance*);

/* Atomic leaf reading */
extern int ocinstance_read(OCstate*,OCinstance*,size_t,size_t,void*,size_t);

#endif /*OCINSTANCE_H*/
