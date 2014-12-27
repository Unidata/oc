#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include "ocuri.h"

#define DEBUG

/*
#define NOP
#define NOS
#define NOB
*/

typedef enum Case {
PREFIX=1,
SUFFIX=2,
BOTH=3,
} Case;

#define nulllen(x) ((x)==NULL?0:strlen(x))

#if 0
#define SET(flags,flag) (((flags) & (flag) != 0)?1:0)
#else
static int SET(Case flags, Case flag)
{
    int amp =  flags & flag;
    if(amp)
	return 1;
    return 0;
}
#endif

static char* testurl = "http://t.edu/";

typedef struct Test {
    Case which;
    char* key;
    char* value;
} Test;

static Test testparms[] = {
{PREFIX,"w",""},
{PREFIX,"x","y"},
{SUFFIX,"a",NULL},
{SUFFIX,"b","c"},
{0,NULL,NULL}
};

static int nparms;
static size_t plen;

static char* paramlist(Case which);
static int test(char* surl, Case which);

int
main(int argc, char** argv)
{
    int i;
    int pass = 1;
    size_t ulen;    
    Test* t;
    char* url;
    char* sprefix;
    char* ssuffix;

    /* Count Tests and total length of the params */
    for(plen=0,nparms=0,t=testparms;t->which;t++) {
	nparms++;
	plen += nulllen(t->key) + nulllen(t->value) + 1; /* 1 for '=' */
    }

    /* Construct and test prefix, infix, and both */
    sprefix = paramlist(PREFIX);
    ssuffix = paramlist(SUFFIX);
   
    ulen = nulllen(testurl) + nulllen(ssuffix) + nulllen(sprefix);
    ulen += 1; /* for null term */

    url = (char*)malloc(ulen);
    if(url == NULL) abort();

#ifndef NOP
    /* prefix only */
    strncpy(url,sprefix,ulen-1);
    strncat(url,testurl,ulen-1);
    if(!test(url,PREFIX)) pass = 0;
#endif

#ifndef NOS
    /* suffix only */
    strncpy(url,testurl,ulen-1);
    strncat(url,ssuffix,ulen-1);
    if(!test(url,SUFFIX)) pass = 0;
#endif

#ifndef NOB
    /* both */
    strncpy(url,sprefix,ulen-1);
    strncat(url,testurl,ulen-1);
    strncat(url,ssuffix,ulen-1);
    if(!test(url,BOTH)) pass = 0;
#endif

    if(pass)
	fprintf(stderr,"***PASS\n");
    return (pass?0:1);

}

static int
test(char* surl, Case which)
{
    Test* t;
    OCURI* url;

    fprintf(stderr,"***Testing URL: %s\n",surl);
    if(!ocuriparse(surl,&url)) {
	fprintf(stderr,"***FAIL: parse\n");
	return 0;
    }
    if(!ocuridecodeparams(url)) {
	fprintf(stderr,"***FAIL: decode params\n");
	return 0;
    }
    for(t=testparms;t->which;t++) {
        const char* value = NULL;
	if(SET(which,t->which)) {
            if(!ocurilookup(url,t->key, &value)) {
	        fprintf(stderr,"***FAIL: lookup: %s\n",t->key);
	        return 0;
	    }
	} else
	    continue;
#ifdef DEBUG
        fprintf(stderr,"Lookup: %s=|%s|\n",t->key,(value==NULL?"null":value));
#endif
	if(t->value != NULL && value != NULL
	   && strcmp(t->value,value) != 0)
            fprintf(stderr,"Mismatch: %s->%s::%s\n",t->key,t->value,value);
    }
    ocurifree(url);
    return 1;
}

static char*
paramlist(Case which)
{
    size_t len = plen+(nparms*2)+1;
    int first;
    Test* t;
    char* list;

    list = (char*)malloc(len);
    if(list == NULL) abort();
    list[0] = '\0';
    for(first=1,t=testparms;t->which;t++) {
	if(which == PREFIX && t->which == PREFIX) {
	    strncat(list,"[",len-1);
	    strncat(list,t->key,len-1);
	    if(t->value != NULL) {
	        strncat(list,"=",len-1);
	        strncat(list,t->value,len-1);
	    }
	    strncat(list,"]",len-1);
	} else if(which == SUFFIX && t->which == SUFFIX) {
            strncat(list,(first?"#":"&"),len-1);
	    strncat(list,t->key,len-1);
	    if(t->value != NULL) {
		strncat(list,"=",len-1);
	        strncat(list,t->value,len-1);
	    }
	    first=0;
	} /* else ignore */
    }
    return list;
}

