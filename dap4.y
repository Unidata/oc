/* Copyright 2009, UCAR/Unidata and OPeNDAP, Inc.
   See the COPYRIGHT file for more information. */

/*
This parser has a lot of C specific code.
Grammar changes must be synchronized with
Java version.
*/

%error-verbose
%define api.pure full

%lex-param {DAP4parser* state}
%parse-param {DAP4parser* state}

%code top {
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
}
   
%code requires {
#include "dap4parselex.h"
#include "dap4tab.h"
}
     
%code {
int dap4debug = 0;
}

%union {
OClist* List;
yax_token* Token;
OCnode* Node;
OCDOM* Dom;
}

%token <Token> DATASET_ _DATASET
%token <Token> GROUP_ _GROUP
%token <Token> ENUMERATION_ _ENUMERATION
%token <Token> ENUMCONST_ _ENUMCONST
%token <Token> NAMESPACE_ _NAMESPACE
%token <Token> DIMENSION_ _DIMENSION
%token <Token> DIM_ _DIM
%token <Token> ENUM_ _ENUM
%token <Token> MAP_ _MAP
%token <Token> STRUCTURE_ _STRUCTURE
%token <Token> SEQUENCE_ _SEQUENCE
%token <Token> VALUE_ _VALUE
%token <Token> ATTRIBUTE_ _ATTRIBUTE
%token <Token> OTHERXML_ _OTHERXML
%token <Token> ERROR_ _ERROR
%token <Token> MESSAGE_ _MESSAGE
%token <Token> CONTEXT_ _CONTEXT
%token <Token> OTHERINFO_ _OTHERINFO

/* atomictype lexemes */
%token <Token> CHAR_ _CHAR
%token <Token> BYTE_ _BYTE
%token <Token> INT8_ _INT8
%token <Token> UINT8_ _UINT8
%token <Token> INT16_ _INT16
%token <Token> UINT16_ _UINT16
%token <Token> INT32_ _INT32
%token <Token> UINT32_ _UINT32
%token <Token> INT64_ _INT64
%token <Token> UINT64_ _UINT64
%token <Token> FLOAT32_ _FLOAT32
%token <Token> FLOAT64_ _FLOAT64
%token <Token> STRING_ _STRING
%token <Token> URL_ _URL
%token <Token> OPAQUE_ _OPAQUE

/* Standard attributes */
%token <Token> ATTR_BASE ATTR_BASETYPE ATTR_DAPVERSION ATTR_DMRVERSION
%token <Token> ATTR_ENUM ATTR_HREF ATTR_NAME ATTR_NAMESPACE
%token <Token> ATTR_NS ATTR_SIZE ATTR_TYPE ATTR_VALUE ATTR_XMLNS

/* XML Attribute List*/
%token <Token> TEXT

/* Unexpected elements or attributes */
%token <Token> UNKNOWN_ATTR UNKNOWN_ELEMENT_ _UNKNOWN_ELEMENT

/* Error cases */
%token <Token>  ERROR UNKNOWN UNEXPECTED


%type <Token> xml_open xml_close xml_attribute atomictype_ _atomictype namespace
%type <List> xml_attribute_map xml_body  namespace_list
%type <Node> otherxml
%type <Dom> element_or_text





%start response

%%

response:
	  dataset
	| error_response
	;

dataset:
	DATASET_
	xml_attribute_map
		{if(dap4_enterdataset(state,$2)) YYABORT;}
	groupbody
	_DATASET
		{if(dap4_leavedataset(state)) YYABORT;}
	;

group:
	GROUP_
	ATTR_NAME
		{if(dap4_entergroup(state,$2)) YYABORT;}
	groupbody
	_GROUP
		{if(dap4_leavegroup(state)) YYABORT;}
	;

/* The decls are directly inserted into the current group,
   so no actions required here
*/
groupbody:
	  /*empty*/
	| groupbody enumdef
	| groupbody dimdef
	| groupbody variable
	| groupbody metadata
	| groupbody group
	;

enumdef:
	ENUMERATION_
	xml_attribute_map
		{if(dap4_enterenumdef(state,$2)) YYABORT;}
	enumconst_list
	_ENUMERATION
		{if(dap4_leaveenumdef(state)) YYABORT;}
	;

enumconst_list:
	  enumconst
	| enumconst_list enumconst
	;

enumconst:
	  ENUMCONST_ ATTR_NAME ATTR_VALUE _ENUMCONST
		{if(dap4_enumconst(state,$2,$3)) YYABORT;}
	| ENUMCONST_ ATTR_VALUE ATTR_NAME _ENUMCONST
		{if(dap4_enumconst(state,$3,$2)) YYABORT;}
	;

dimdef:
	DIMENSION_
	xml_attribute_map
		{if(dap4_enterdimdef(state,$2)) YYABORT;}
	metadatalist
	_DIMENSION
		{if(dap4_leavedimdef(state)) YYABORT;}
	;

dimref:
	  DIM_ ATTR_NAME _DIM
		{if(dap4_dimref(state,$2)) YYABORT;}
	| DIM_ ATTR_SIZE _DIM
		{if(dap4_dimref(state,$2)) YYABORT;}
	;

variable:
	  atomicvariable
	| enumvariable
	| structurevariable
	| sequencevariable
	;

/* Use atomic type to avoid rule explosion */
atomicvariable:
	atomictype_
	ATTR_NAME
		{if(dap4_enteratomicvariable(state,$1,$2)) YYABORT;}
	varbody
	_atomictype
		{if(dap4_leaveatomicvariable(state,$5)) YYABORT;}

	;

enumvariable:
	ENUM_
	xml_attribute_map
		{if(dap4_enterenumvariable(state,$2)) YYABORT;}
	varbody
	_ENUM
		{if(dap4_leaveenumvariable(state)) YYABORT;}

	;

/* Does not include enum */
atomictype_:
	  CHAR_ {$$=($1);}
	| BYTE_ {$$=($1);}
	| INT8_ {$$=($1);}
	| UINT8_ {$$=($1);}
	| INT16_ {$$=($1);}
	| UINT16_ {$$=($1);}
	| INT32_ {$$=($1);}
	| UINT32_ {$$=($1);}
	| INT64_ {$$=($1);}
	| UINT64_ {$$=($1);}
	| FLOAT32_ {$$=($1);}
	| FLOAT64_ {$$=($1);}
	| STRING_ {$$=($1);}
	| URL_ {$$=($1);}
	| OPAQUE_ {$$=($1);}
	;

_atomictype:
	  _CHAR {$$=($1);}
	| _BYTE {$$=($1);}
	| _INT8 {$$=($1);}
	| _UINT8 {$$=($1);}
	| _INT16 {$$=($1);}
	| _UINT16 {$$=($1);}
	| _INT32 {$$=($1);}
	| _UINT32 {$$=($1);}
	| _INT64 {$$=($1);}
	| _UINT64 {$$=($1);}
	| _FLOAT32 {$$=($1);}
	| _FLOAT64 {$$=($1);}
	| _STRING {$$=($1);}
	| _URL {$$=($1);}
	| _OPAQUE {$$=($1);}
	| _ENUM {$$=($1);}
	;

varbody:
	  /*empty*/
	| varbody dimref
	| varbody mapref
	| varbody metadata
	;

mapref:
	MAP_
	ATTR_NAME
		{if(dap4_entermap(state,$2)) YYABORT;}
	metadatalist
	_MAP
		{if(dap4_leavemap(state)) YYABORT;}
	;

structurevariable:
	STRUCTURE_
	ATTR_NAME
		{if(dap4_enterstructurevariable(state,$2)) YYABORT;}
	structbody
	_STRUCTURE
		{if(dap4_leavestructurevariable(state)) YYABORT;}
	;

structbody:
	  /*empty*/
	| structbody dimref
	| structbody variable
	| structbody mapref
	| structbody metadata
	;

sequencevariable:
	SEQUENCE_
	ATTR_NAME
		{if(dap4_entersequencevariable(state,$2)) YYABORT;}
	sequencebody
	_SEQUENCE
		{if(dap4_leavesequencevariable(state)) YYABORT;}
	;

sequencebody:
	  /*empty*/
	| sequencebody dimref
	| sequencebody variable
	| sequencebody mapref
	| sequencebody metadata
	;

metadatalist:
	  /*empty*/
	| metadatalist metadata
	;

metadata:
	  attribute
	| otherxml
	;

attribute:
	  atomicattribute
	| containerattribute
	;


/* We have to case this out to avoid empty list followed by empty list */
atomicattribute:
	  ATTRIBUTE_
	  xml_attribute_map
	  namespace_list
		{if(dap4_enteratomicattribute(state,$2,$3)) YYABORT;}
	  valuelist
	  _ATTRIBUTE
		{if(dap4_leaveatomicattribute(state)) YYABORT;}
	|
	  ATTRIBUTE_
	  xml_attribute_map
	  namespace_list
		{if(dap4_enteratomicattribute(state,$2,$3)) YYABORT;}
	  _ATTRIBUTE
		{if(dap4_leaveatomicattribute(state)) YYABORT;}
	;

namespace_list:
	  /*empty*/
		{$$=dap4_namespace_list(state,NULL,NULL);}
	| namespace_list namespace
		{$$=dap4_namespace_list(state,$1,$2);}
	;

namespace:
	NAMESPACE_
	ATTR_HREF
	_NAMESPACE
		{$$=($2);}
	;

containerattribute:
	  ATTRIBUTE_
	  xml_attribute_map
	  namespace_list
		{if(dap4_entercontainerattribute(state,$2,$3)) YYABORT;}
	  attributelist
	  _ATTRIBUTE
		{if(dap4_leavecontainerattribute(state)) YYABORT;}
	;

/* Cannot be empty */
attributelist:
	  attribute
	| attributelist attribute
	;

/* Cannot be empty (see atomicattribute above) */
valuelist:
	  value
	| valuelist value
	;

value:
	  VALUE_ TEXT _VALUE /* text can contain multiple values */
		{if(dap4_value(state,$2)) YYABORT;}
	| VALUE_ ATTR_VALUE _VALUE /* Single Value */
		{if(dap4_value(state,$2)) YYABORT;}
	;

otherxml:
	OTHERXML_
	xml_attribute_map
	xml_body
	_OTHERXML
	    {if(dap4_otherxml(state,$2,$3)) YYABORT;}
	;

xml_body:
	  element_or_text
		{$$=dap4_xmlbody(state,NULL,$1);}
	| xml_body element_or_text
		{$$=dap4_xmlbody(state,$1,$2);}
	;

element_or_text:
	  xml_open
	  xml_attribute_map
	  xml_body
	  xml_close
		{$$=dap4_xmlelement(state,$1,$2,$3,$4);}
	| TEXT
		{$$=dap4_xmltext(state,$1);}
	;

/* Use a generic map of xml attributes; action
   is responsible for checking correctness.
*/
xml_attribute_map:
	  /*empty*/
		{$$=dap4_xml_attributes(state,NULL,NULL);}
	| xml_attribute_map xml_attribute
		{$$=dap4_xml_attributes(state,$1,$2);}
	;

/* Collect the set of all known attributes */
/* Assume default action: $$=$1 */
xml_attribute:
	  ATTR_BASE
	| ATTR_BASETYPE
	| ATTR_DAPVERSION
	| ATTR_DMRVERSION
	| ATTR_ENUM
	| ATTR_HREF
	| ATTR_NAME
	| ATTR_NAMESPACE
	| ATTR_NS
	| ATTR_SIZE
	| ATTR_TYPE
	| ATTR_VALUE
	| UNKNOWN_ATTR
	;

/* Collect the set of all known elements */
/* Assume default action: $$=$1 */
xml_open:
	  DATASET_
	| GROUP_
	| ENUMERATION_
	| ENUMCONST_
	| NAMESPACE_
	| DIMENSION_
	| DIM_
	| ENUM_
	| MAP_
	| STRUCTURE_
	| SEQUENCE_
	| VALUE_
	| ATTRIBUTE_
	| OTHERXML_
	| CHAR_
	| BYTE_
	| INT8_
	| UINT8_
	| INT16_
	| UINT16_
	| INT32_
	| UINT32_
	| INT64_
	| UINT64_
	| FLOAT32_
	| FLOAT64_
	| STRING_
	| URL_
	| OPAQUE_
	| UNKNOWN_ELEMENT_
	;

xml_close:
	  _DATASET
	| _GROUP
	| _ENUMERATION
	| _ENUMCONST
	| _NAMESPACE
	| _DIMENSION
	| _DIM
	| _ENUM
	| _MAP
	| _STRUCTURE
	| _SEQUENCE
	| _VALUE
	| _ATTRIBUTE
	| _OTHERXML
	| _CHAR
	| _BYTE
	| _INT8
	| _UINT8
	| _INT16
	| _UINT16
	| _INT32
	| _UINT32
	| _INT64
	| _UINT64
	| _FLOAT32
	| _FLOAT64
	| _STRING
	| _URL
	| _OPAQUE
	| _UNKNOWN_ELEMENT
	;

error_response:
	ERROR_
	xml_attribute_map
	    /* optional attribute name="httpcode" data type="dap4_integer" */
	    {if(dap4_entererror(state,$2)) YYABORT;}

	error_body
	_ERROR
	    {if(dap4_leaveerror(state)) YYABORT;}
	;

error_body:
	  /*empty*/
	| error_body error_element
	;

error_element:
	  MESSAGE_ TEXT _MESSAGE
		{if(dap4_errormessage(state,$2)) YYABORT;}
	| CONTEXT_ TEXT _CONTEXT
		{if(dap4_errorcontext(state,$2)) YYABORT;}
	| OTHERINFO_ TEXT _OTHERINFO
		{if(dap4_errorotherinfo(state,$2)) YYABORT;}
	
	;
