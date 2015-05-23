/* Copyright 2009, UCAR/Unidata and OPeNDAP, Inc.
   See the COPYRIGHT file for more information. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "oclist.h"
#include "dap4tab.h"
#include "yax.h"

#undef TRACE

struct dap4_keyword {
	char *name;
	int opentag;
	int closetag;
	int attrtag;
};

/**************************************************/

/**************************************************/
int
dap4lex(YYSTYPE* lvalp, DAP4parser* parser)
{
    yax_err err = YAX_OK;
    yax_token yaxtoken_instance;
    yax_token* yaxtoken = &yaxtoken_instance;
    DAP4lexer* lexer = parser->lexstate;
    yax_lexer* yax = lexer->yax;
    const struct dap4_keyword* keyword;
    int yytoken = UNKNOWN;

    while(yytoken == UNKNOWN) {
	err = yax_nexttoken(yax,yaxtoken);
	if(err) goto done;

#ifdef TRACE
	{
	char* trace = yax_trace(yax,yaxtoken);
	printf("yylex: charno=%d token: %s\n",yaxtoken->charno,trace);
	fflush(stdout);
	if(trace) free(trace);
	}
#endif

	if(yaxtoken->name != NULL)
	    keyword = dap4_keyword_lookup(yaxtoken->name, strlen(yaxtoken->name));

	/* If the keyword is unknown and not an attribute, then fail */
	if(keyword == NULL && yaxtoken->tokentype != YAX_ATTRIBUTE) {
	    char msg[1024];
	    strcpy(msg,"unknown token: |");
	    strncat(msg,yaxtoken->name,16);
	    strcat(msg,"...|\n");
	    dap4error(parser,msg);
	    yytoken = UNEXPECTED;
	    goto done;
	}

	switch(yaxtoken->tokentype) {

	case YAX_OPEN: 
	    /* Since keyword is defined, this means the opentag is legal */
	    yytoken = keyword->opentag;
	    /* Check for the special case of <Value> */
	    if(keyword->opentag == VALUE_)
		lexer->textok = 1;
	    break;

	case YAX_EMPTYCLOSE:
	case YAX_CLOSE:
	    /* Since keyword is defined, this means the closetag is legal */
	    yytoken = keyword->closetag;
	    lexer->textok = 0;
	    break;

	case YAX_ATTRIBUTE:
	    if(keyword == NULL)
		yytoken = UNKNOWN_ATTR;
	    else
	        /* Since keyword is defined,
                   this means the attribute is legal */
	        yytoken = keyword->attrtag;
	    break;

	case YAX_TEXT:
	    if(lexer->textok)
		yytoken = TEXT;
	    break;

	case YAX_COMMENT:
	    break;

	case YAX_EOF:
	    yytoken = 0;
	    break;

	default: {
	    char msg[1024];
	    snprintf(msg,sizeof(msg),"unknown token type: %d\n",yaxtoken->tokentype);
	    dap4error(parser,msg);
	    yytoken = ERROR;
	    } break;
	}
    }

done:
    if(err) {
	dap4error(parser,yax_errstring(err));
	return ERROR;
    }
    if(yaxtoken != NULL)
        lexer->lasttoken = *yaxtoken;
    return yytoken;
}

void
dap4lexinit(char* input, DAP4lexer** lexstatep)
{
    DAP4lexer* lexstate;
    if(lexstatep == NULL) return; /* no point in building it */
    lexstate = (DAP4lexer*)malloc(sizeof(DAP4lexer));
    if(lexstate == NULL) return;
    memset((void*)lexstate,0,sizeof(DAP4lexer));
    if(!yax_create(input,YXFLAG_NOCR,&lexstate->yax))
	goto fail;
    *lexstatep = lexstate;
    return;

fail:
    if(lexstate != NULL)
	dap4lexcleanup(&lexstate);
    *lexstatep = NULL;
}

void
dap4lexcleanup(DAP4lexer** lexstatep)
{
    DAP4lexer* lexstate = *lexstatep;
    if(lexstate->yax != NULL)
	yax_free(lexstate->yax);
    free(lexstate);
    *lexstatep = NULL;
}

