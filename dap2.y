/* Copyright 2009, UCAR/Unidata and OPeNDAP, Inc.
   See the COPYRIGHT file for more information. */

/*The lines down to DO NOT DELETE ... comment are specific to the C Parser.
  They will be commennted out when building a java parser.
*/
%error-verbose
%define api.pure full
%lex-param {DAP2parsestate* parsestate}
%parse-param {DAP2parsestate* parsestate}

%code top{
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
}

%code requires {
#include "dap2parselex.h"
#define YYSTYPE Object
#include "dap2tab.h"
}

%code {
int dap2debug = 0;
}

/*DO NOT DELETE THIS LINE*/

%token SCAN_ALIAS 
%token SCAN_ARRAY
%token SCAN_ATTR
%token SCAN_BYTE
%token SCAN_CODE
%token SCAN_DATASET
%token SCAN_DATA

%token SCAN_ERROR
%token SCAN_FLOAT32
%token SCAN_FLOAT64
%token SCAN_GRID
%token SCAN_INT16
%token SCAN_INT32
%token SCAN_MAPS 
%token SCAN_MESSAGE
%token SCAN_SEQUENCE
%token SCAN_STRING
%token SCAN_STRUCTURE
%token SCAN_UINT16
%token SCAN_UINT32
%token SCAN_URL 
/* For errorbody */
%token SCAN_PTYPE
%token SCAN_PROG

/* Non-keywords */
%token WORD_WORD WORD_STRING

%start start

%%

start:
	  dataset datasetbody
	| dataset datasetbody SCAN_DATA
	| attr attributebody
	| err errorbody
        | error {dap2_unrecognizedresponse(parsestate); YYABORT;}
	;

dataset:
	SCAN_DATASET
	    {dap2_tagparse(parsestate,SCAN_DATASET);}
	;
attr:
	SCAN_ATTR
	    {dap2_tagparse(parsestate,SCAN_ATTR);}
	;
err:
	SCAN_ERROR
	    {dap2_tagparse(parsestate,SCAN_ERROR);}
	;

datasetbody:
	  '{' declarations '}' datasetname ';'
		{dap2_datasetbody(parsestate,$4,$2);}
	;


declarations:
	  /* empty */ {$$=dap2_declarations(parsestate,null,null);}
        | declarations declaration {$$=dap2_declarations(parsestate,$1,$2);}
	;

/* 01/21/08: James says: no dimensions for grids or sequences */
/* 05/08/09: James says: no duplicate map names */
declaration:
	  base_type var_name array_decls ';'
		{$$=dap2_makebase(parsestate,$2,$1,$3);}
	| SCAN_STRUCTURE '{' declarations '}' var_name array_decls ';'
	    {if(($$=dap2_makestructure(parsestate,$5,$6,$3))==null) {YYABORT;}}
	| SCAN_SEQUENCE '{' declarations '}' var_name ';'
	    {if(($$=dap2_makesequence(parsestate,$5,$3))==null) {YYABORT;}}
	| SCAN_GRID '{' SCAN_ARRAY ':' declaration SCAN_MAPS ':'
          declarations '}' var_name ';'
	    {if(($$=dap2_makegrid(parsestate,$10,$5,$8))==null) {YYABORT;}}
        | error 
            {dap2semanticerror(parsestate,OC_EBADTYPE,"Unrecognized type"); YYABORT;}
	;
 

base_type:
	  SCAN_BYTE {$$=(Object)SCAN_BYTE;}
	| SCAN_INT16 {$$=(Object)SCAN_INT16;}
	| SCAN_UINT16 {$$=(Object)SCAN_UINT16;}
	| SCAN_INT32 {$$=(Object)SCAN_INT32;}
	| SCAN_UINT32 {$$=(Object)SCAN_UINT32;}
	| SCAN_FLOAT32 {$$=(Object)SCAN_FLOAT32;}
	| SCAN_FLOAT64 {$$=(Object)SCAN_FLOAT64;}
	| SCAN_URL {$$=(Object)SCAN_URL;}
	| SCAN_STRING {$$=(Object)SCAN_STRING;}
	;

array_decls:
	  /* empty */ {$$=dap2_arraydecls(parsestate,null,null);}
	| array_decls array_decl {$$=dap2_arraydecls(parsestate,$1,$2);}
	;

array_decl:
	   '[' WORD_WORD ']' {$$=dap2_arraydecl(parsestate,null,$2);}
	 | '[' '=' WORD_WORD ']' {$$=dap2_arraydecl(parsestate,null,$3);}
	 | '[' name '=' WORD_WORD ']' {$$=dap2_arraydecl(parsestate,$2,$4);}
	 | error
	    {dap2semanticerror(parsestate,OC_EDIMSIZE,"Illegal dimension declaration"); YYABORT;}
	;

datasetname:
	  var_name {$$=$1;}
        | error
	    {dap2semanticerror(parsestate,OC_EDDS,"Illegal dataset declaration"); YYABORT;}
	;

var_name: name {$$=$1;};

attributebody:
	  '{' attr_list '}' {dap2_attributebody(parsestate,$2);}
	| error
            {dap2semanticerror(parsestate,OC_EDAS,"Illegal DAS body"); YYABORT;}
	;

attr_list:
	  /* empty */ {$$=dap2_attrlist(parsestate,null,null);}
	| attr_list attribute {$$=dap2_attrlist(parsestate,$1,$2);}
	;

attribute:
	  alias ';' {$$=null;} /* ignored */ 
        | SCAN_BYTE name bytes ';'
	    {$$=dap2_attribute(parsestate,$2,$3,(Object)SCAN_BYTE);}
	| SCAN_INT16 name int16 ';'
	    {$$=dap2_attribute(parsestate,$2,$3,(Object)SCAN_INT16);}
	| SCAN_UINT16 name uint16 ';'
	    {$$=dap2_attribute(parsestate,$2,$3,(Object)SCAN_UINT16);}
	| SCAN_INT32 name int32 ';'
	    {$$=dap2_attribute(parsestate,$2,$3,(Object)SCAN_INT32);}
	| SCAN_UINT32 name uint32 ';'
	    {$$=dap2_attribute(parsestate,$2,$3,(Object)SCAN_UINT32);}
	| SCAN_FLOAT32 name float32 ';'
	    {$$=dap2_attribute(parsestate,$2,$3,(Object)SCAN_FLOAT32);}
	| SCAN_FLOAT64 name float64 ';'
	    {$$=dap2_attribute(parsestate,$2,$3,(Object)SCAN_FLOAT64);}
	| SCAN_STRING name strs ';'
	    {$$=dap2_attribute(parsestate,$2,$3,(Object)SCAN_STRING);}
	| SCAN_URL name urls ';'
	    {$$=dap2_attribute(parsestate,$2,$3,(Object)SCAN_URL);}
	| name '{' attr_list '}' {$$=dap2_attrset(parsestate,$1,$3);}
	| error 
            {dap2semanticerror(parsestate,OC_EDAS,"Illegal attribute"); YYABORT;}
	;

bytes:
	  WORD_WORD {$$=dap2_attrvalue(parsestate,null,$1,(Object)SCAN_BYTE);}
	| bytes ',' WORD_WORD
		{$$=dap2_attrvalue(parsestate,$1,$3,(Object)SCAN_BYTE);}
	;
int16:
	  WORD_WORD {$$=dap2_attrvalue(parsestate,null,$1,(Object)SCAN_INT16);}
	| int16 ',' WORD_WORD
		{$$=dap2_attrvalue(parsestate,$1,$3,(Object)SCAN_INT16);}
	;
uint16:
	  WORD_WORD {$$=dap2_attrvalue(parsestate,null,$1,(Object)SCAN_UINT16);}
	| uint16 ',' WORD_WORD
		{$$=dap2_attrvalue(parsestate,$1,$3,(Object)SCAN_UINT16);}
	;
int32:
	  WORD_WORD {$$=dap2_attrvalue(parsestate,null,$1,(Object)SCAN_INT32);}
	| int32 ',' WORD_WORD
		{$$=dap2_attrvalue(parsestate,$1,$3,(Object)SCAN_INT32);}
	;
uint32:
	  WORD_WORD {$$=dap2_attrvalue(parsestate,null,$1,(Object)SCAN_UINT32);}
	| uint32 ',' WORD_WORD  {$$=dap2_attrvalue(parsestate,$1,$3,(Object)SCAN_UINT32);}
	;
float32:
	  WORD_WORD {$$=dap2_attrvalue(parsestate,null,$1,(Object)SCAN_FLOAT32);}
	| float32 ',' WORD_WORD  {$$=dap2_attrvalue(parsestate,$1,$3,(Object)SCAN_FLOAT32);}
	;
float64:
	  WORD_WORD {$$=dap2_attrvalue(parsestate,null,$1,(Object)SCAN_FLOAT64);}
	| float64 ',' WORD_WORD  {$$=dap2_attrvalue(parsestate,$1,$3,(Object)SCAN_FLOAT64);}
	;
strs:
	  str_or_id {$$=dap2_attrvalue(parsestate,null,$1,(Object)SCAN_STRING);}
	| strs ',' str_or_id {$$=dap2_attrvalue(parsestate,$1,$3,(Object)SCAN_STRING);}
	;

urls:
	  url {$$=dap2_attrvalue(parsestate,null,$1,(Object)SCAN_URL);}
	| urls ',' url {$$=dap2_attrvalue(parsestate,$1,$3,(Object)SCAN_URL);}
	;

url:
	str_or_id {$$=$1;}
	;

str_or_id:
	  name {$$=$1;}
	| WORD_STRING {$$=$1;}
	;

/* Not used
float_or_int:
	  WORD_INT {$$=$1;}
	| WORD_DOUBLE {$$=$1;}        
	;
*/

alias:
	SCAN_ALIAS WORD_WORD WORD_WORD {$$=$2; $$=$3; $$=null;} /* Alias is ignored */
	;

errorbody:
	'{' errorcode errormsg errorptype errorprog '}' ';'
		{dap2_errorbody(parsestate,$2,$3,$4,$5);}
	;

errorcode:  /*empty*/ {$$=null;} | SCAN_CODE    '=' WORD_WORD ';' {$$=$3;}
errormsg:   /*empty*/ {$$=null;} | SCAN_MESSAGE '=' WORD_WORD ';' {$$=$3;}
errorptype: /*empty*/ {$$=null;} | SCAN_PTYPE   '=' WORD_WORD ';' {$$=$3;}
errorprog : /*empty*/ {$$=null;} | SCAN_PROG    '=' WORD_WORD ';' {$$=$3;}

/* Note that variable names like "byte" are legal names
   and are disambiguated by context
*/
name:
          WORD_WORD      {$$=dap2decode(parsestate->lexstate,$1);}
	| SCAN_ALIAS     {$$=strdup("alias");}
	| SCAN_ARRAY     {$$=strdup("array");}
	| SCAN_ATTR      {$$=strdup("attributes");}
	| SCAN_BYTE      {$$=strdup("byte");}
	| SCAN_DATASET   {$$=strdup("dataset");}
	| SCAN_DATA      {$$=strdup("data");}
	| SCAN_ERROR     {$$=strdup("error");}
	| SCAN_FLOAT32   {$$=strdup("float32");}
	| SCAN_FLOAT64   {$$=strdup("float64");}
	| SCAN_GRID      {$$=strdup("grid");}
	| SCAN_INT16     {$$=strdup("int16");}
	| SCAN_INT32     {$$=strdup("int32");}
	| SCAN_MAPS      {$$=strdup("maps");}
	| SCAN_SEQUENCE  {$$=strdup("sequence");}
	| SCAN_STRING    {$$=strdup("string");}
	| SCAN_STRUCTURE {$$=strdup("structure");}
	| SCAN_UINT16    {$$=strdup("uint16");}
	| SCAN_UINT32    {$$=strdup("uint32");}
	| SCAN_URL       {$$=strdup("url");}
	| SCAN_CODE      {$$=strdup("code");}
	| SCAN_MESSAGE   {$$=strdup("message");}
	| SCAN_PROG      {$$=strdup("program");}
	| SCAN_PTYPE     {$$=strdup("program_type");}
	;

%%
