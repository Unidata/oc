/* Copyright 2009, UCAR/Unidata and OPeNDAP, Inc.
   See the COPYRIGHT file for more information. */

#ifndef DAP2PARSELEX_H
#define DAP2PARSELEX_H 1

#include "ocinternal.h"
#include "ocdebug.h"

/* For consistency with Java parser */
#define null NULL

typedef void* Object;

#define MAX_TOKEN_LENGTH 1024

/*! Specifies the Lexstate. */
typedef struct DAP2lexstate {
    char* input;
    char* next; /* next char in uri.query*/
    OCbytes* yytext;
    int lineno;
    /*! Specifies the Lasttoken. */
    int lasttoken;
    char lasttokentext[MAX_TOKEN_LENGTH+1];
    char* wordchars1;
    char* wordcharsn;
    char* worddelims;
    OClist* reclaim; /* reclaim WORD_WORD instances */
} DAP2lexstate;

/*! Specifies the DAP2parsestate. */
typedef struct DAP2parsestate {
    struct OCnode* root;
    DAP2lexstate* lexstate;
    OClist* ocnodes;
    struct OCstate* conn;
    /* Provide a flag for semantic failures during parse */
    OCerror error; /* OC_EDAPSVC=> we had a server failure; else we had a semantic error */
    char* code;
    char* message;
    char* progtype;
    char* progname;
    /* State for constraint expressions */
    struct CEstate* cestate;
} DAP2parsestate;

extern int dap2debug; /* global state */

extern int dap2error(DAP2parsestate*, const char*);
extern int dap2semanticerror(DAP2parsestate* state, OCerror, const char* msg);
extern void dap2_parse_error(DAP2parsestate*,const char *fmt, ...);
/* bison parse entry point */
extern int dap2parse(DAP2parsestate*);

/* Actions */
extern Object dap2_datasetbody(DAP2parsestate*,Object decls, Object name);
extern Object dap2_declarations(DAP2parsestate*,Object decls, Object decl);
extern Object dap2_arraydecls(DAP2parsestate*,Object arraydecls, Object arraydecl);
extern Object dap2_arraydecl(DAP2parsestate*,Object name, Object size);

extern void dap2_dassetup(DAP2parsestate*);
extern Object dap2_attributebody(DAP2parsestate*,Object attrlist);
extern Object dap2_attrlist(DAP2parsestate*,Object attrlist, Object attrtuple);
extern Object dap2_attribute(DAP2parsestate*,Object name, Object value, Object etype);
extern Object dap2_attrset(DAP2parsestate*,Object name, Object attributes);
extern Object dap2_attrvalue(DAP2parsestate*,Object valuelist, Object value, Object etype);

extern Object dap2_makebase(DAP2parsestate*,Object name, Object etype, Object dimensions);
extern Object dap2_makestructure(DAP2parsestate*,Object name, Object dimensions, Object fields);
extern Object dap2_makesequence(DAP2parsestate*,Object name, Object members);
extern Object dap2_makegrid(DAP2parsestate*,Object name, Object arraydecl, Object mapdecls);

extern void dap2_errorbody(DAP2parsestate*, Object, Object, Object, Object);
extern void dap2_unrecognizedresponse(DAP2parsestate*);

extern void dap2_tagparse(DAP2parsestate*,int);

extern char* dap2_dimnameanon(char* basename, unsigned int index);

/* Lexer entry points */
extern int dap2lex(Object*, DAP2parsestate*);
extern void dap2lexinit(char* input, DAP2lexstate** lexstatep);
extern void dap2lexcleanup(DAP2lexstate** lexstatep);
extern void dap2setwordchars(DAP2lexstate* lexstate, int kind);
extern char* dap2decode(DAP2lexstate*,char*);

extern OCerror DAP2parse(OCstate*, struct OCtree*, char*);

#endif /*DAP2PARSELEX_H*/
