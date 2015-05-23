/* Copyright 2009, UCAR/Unidata and OPeNDAP, Inc.
   See the COPYRIGHT file for more information. */

#ifndef DAP4PARSELEX_H
#define DAP4PARSELEX_H 1

#include "ocinternal.h"
#include "ocdebug.h"
#include "yax.h"

/* For consistency with Java parser */
#define null NULL

//typedef void* Object;

#define DEFAULTENUMBASETYPE OC_Int32

#define MAX_TOKEN_LENGTH 1024

/*! Specifies the DAP4 lexer state. */
typedef struct DAP4lexer {
    yax_lexer* yax;
    int textok;
    yax_token lasttoken;
} DAP4lexer;

/*! Specifies the DAP4 parser state. */
typedef struct DAP4parser {
    struct OCnode* root;
    DAP4lexer* lexstate;
    OClist* ocnodes;
    struct OCstate* conn;
    OClist* scopestack;
    /* Provide a flag for semantic failures during parse */
    OCerror error; /* OC_EDAPSVC=> we had a server failure;
                      else we had a semantic error */
    struct {
	char* code;
	char* message;
    } errormsg; 
    int datadmr; /* 1 => we are parsing a data dmr */
} DAP4parser;

#define DAPVERSION 4.0
#define DMRVERSION 1.0

enum RULE {
    RULENULL = 0,
    RULEDIMREF = 1,
    RULEMAPREF = 2,
    RULEVAR = 3,
    RULEMETADATA = 4,
};

/* Lexer entry points */
union YYSTYPE;
extern int dap4lex(union YYSTYPE*, DAP4parser*);
extern void dap4lexinit(char* input, DAP4lexer** lexstatep);
extern void dap4lexcleanup(DAP4lexer** lexstatep);

/* DAP4 Parser entry point */
extern int dap4debug; /* global state */
extern OCerror DAP4parse(OCstate*, struct OCtree*, char*);
extern int dap4parse(DAP4parser*);

/* DAP4 Error reporting*/
extern int dap4error(DAP4parser* state, const char* msg);

/* DAP4 Actions */

extern int dap4_enterdataset(DAP4parser*, OClist* alist);
extern int dap4_leavedataset(DAP4parser*);
extern int dap4_entergroup(DAP4parser*, yax_token* name);
extern int dap4_leavegroup(DAP4parser*);
extern int dap4_enterenumdef(DAP4parser*, OClist* attrs);
extern int dap4_leaveenumdef(DAP4parser*);
extern int dap4_enumconst(DAP4parser*, yax_token* name, yax_token* value);
extern int dap4_enterdimdef(DAP4parser*, OClist* attrs);
extern int dap4_leavedimdef(DAP4parser*);
extern int dap4_dimref(DAP4parser*, yax_token* nameorsize);
extern int dap4_enteratomicvariable(DAP4parser*, yax_token* open, yax_token* nameattr);
extern int dap4_leaveatomicvariable(DAP4parser*, yax_token* close);
extern int dap4_enterenumvariable(DAP4parser*, OClist* attrs);
extern int dap4_leaveenumvariable(DAP4parser*);
extern int dap4_entermap(DAP4parser*, yax_token* name);
extern int dap4_leavemap(DAP4parser*);
extern int dap4_enterstructurevariable(DAP4parser*, yax_token* name);
extern int dap4_leavestructurevariable(DAP4parser*);
extern int dap4_entersequencevariable(DAP4parser*, yax_token* name);
extern int dap4_leavesequencevariable(DAP4parser*);
extern int dap4_enteratomicattribute(DAP4parser*, OClist* attrs, OClist* nslist);
extern int dap4_leaveatomicattribute(DAP4parser*);
extern int dap4_entercontainerattribute(DAP4parser*, OClist* attrs, OClist* nslist);
extern int dap4_leavecontainerattribute(DAP4parser*);
extern int dap4_value(DAP4parser*, yax_token* value);
extern int dap4_entererror(DAP4parser*, OClist* attrs);
extern int dap4_leaveerror(DAP4parser*) ;
extern int dap4_errormessage(DAP4parser*, yax_token* value);
extern int dap4_errorcontext(DAP4parser*, yax_token* value);
extern int dap4_errorotherinfo(DAP4parser*, yax_token* value);
extern const struct dap4_keyword* dap4_keyword_lookup(const char*, unsigned int);

/* List support */
extern OClist* dap4_xml_attributes(DAP4parser*,OClist*,yax_token*);
extern OClist* dap4_namespace_list(DAP4parser*,OClist*,yax_token*);

extern int dap4_otherxml(DAP4parser* state, OClist* attrs, OClist* body);
extern OClist* dap4_otherxml_attributes(DAP4parser* state, OClist* list, OCDOM* elem);
extern OClist* dap4_xmlbody(DAP4parser* state, OClist* list, OCDOM* elem);
extern OCDOM* dap4_xmlelement(DAP4parser* state, yax_token* open, OClist* attrs, OClist* body, yax_token* close) ;
extern OCDOM* dap4_xmltext(DAP4parser* state, yax_token* text);
#endif /*DAP4PARSELEX_H*/
