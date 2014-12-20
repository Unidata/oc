#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#define DEBUG

/* pick one */
#if defined DEBUG
#define SHOW(x) show(x)
#else
#define SHOW(x) 
#endif

#include "oc.h"

#undef NOEMBED
#define NOLOCAL
#define NOHOME
#define NOREDIR

#define KEEPRC

#define RC ".ocrc"
#define COOKIEFILE "./cookies"

typedef struct Test {
    char* protocol;
    char* svc;
    char* path;
    char* userpwd;
} Test;

Test tests[] = {
{"https","remotetest.unidata.ucar.edu","thredds/dodsC/restrict/testData.nc","tiggeUser:tigge"},
{"https","54.86.135.31","/opendap/data/nc/fnoc1.nc",NULL},
{NULL,NULL,NULL,NULL}
};

/* Temporary spaces to avoid malloc*/
static char url[8192];

static int testrc(Test* test);
static void fillrc(Test* test);
static void killrc();
static void show(OCddsnode root);

#ifdef DEBUG
static void
CHECK(int e, const char* msg)
{
    if(e == OC_NOERR) return;
    if(msg == NULL) msg = "Error";
    fprintf(stderr,"%s: %s\n", msg, oc_errstring(e));
    exit(1);
}
#endif

static void
fail(int pass)
{
    fprintf(stderr,"***XFAIL : t_auth1\n");
    exit(0); 
}

int
main(int argc, char** argv)
{
    OCerror retval;
    FILE* rc;
    char buffer[8192];
    const char* home;
    OClink link = NULL;
    OCddsnode root;
    int pass = 0;
    TEST* test;

    fprintf(stderr,"Testing: Authorization\n");
    for(test=tests;test->svc != NULL;test++) {
	if(test->userpwd == NULL) {/* get from elsewhere */
        if(argc > 1)
	    test->userpwd = argv[1];
	else if(getenv("UAT") != NULL) {
	    test->userpwd = getenv("UAT");
	else {
	    fprintf(stderr,"***XFAIL : %s: cannot locate user:pwd argument\n",argv[0]);
	    goto done;
	}
	/* construct the url */

    snprintf(url1,sizeof(url1),URL1,USERPWD,URLSERVER); /* embedded */
    snprintf(url2,sizeof(url2),URL2,URLSERVER); /* using rc file */

#ifdef DEBUG
fprintf(stderr,"url1: %s\n",url1);
fprintf(stderr,"url2: %s\n",url2);
fflush(stderr);
#endif

    pass = 1; /* assume success */
    killrc();

    fprintf(stderr,"Testing: Http Basic Authorization\n\n");
#ifndef NOEMBED
    {
        fprintf(stderr,"Testing: Embedded user:pwd: %s\n",url1);
        retval = oc_open(url1, &link);
	if(retval == OC_NOERR) {
	    retval = oc_fetch(link,NULL,OCDDS,0,&root);
	}
        if(retval != OC_NOERR) {
            pass = 0;
            fprintf(stderr,"*** FAIL: Testing embedded user:pwd\n");
        } else {
	    SHOW(root);
            fprintf(stderr,"*** PASS: Testing embedded user:pwd\n");
	}
	if(link != NULL)
	    retval = oc_close(link);
        fflush(stderr);
    }
#endif

#ifndef NOLOCAL
    {
        /* Test 1: RC in ./ */
        fprintf(stderr,"Testing: user:pwd in %s/%s: %s\n",".",RC,USERPWD);
	if(!testrc(".",url2)) {
	    fprintf(stderr,"user:pwd in %s/%s failed\n",".",RC);
	    goto done;
        }
    }
#endif

#ifndef NOHOME
    {
        /* Test 1: RC in HOME  */
	home = getenv("HOME");
        fprintf(stderr,"user:pwd in %s/%s: %s\n",home,RC,USERPWD);
	if(!testrc(home,url2)) {
	    fprintf(stderr,"user:pwd in %s/%s failed\n",home,RC);
	    goto done;
        }
    }
#endif

done:
    if(!pass)
	fail(pass);	
    return 0;
}

static int
testrc(const char* prefix, const char* url)
{
    int pass = 1;
    OCerror retval;
    int ncid;
    char rcpath[8192];
    FILE* rc;
    OClink link = NULL;
    OCddsnode root;

    snprintf(rcpath,sizeof(rcpath),"%s/%s",prefix,RC);
    rc = fopen(rcpath,"w");
    if(rc == NULL) {
        fprintf(stderr,"Cannot create ./%s\n",RC);
	return 0;
    }    
    fclose(rc);
    fillrc(rcpath);
    retval = oc_open(url1, &link);
    if(retval == OC_NOERR)
	retval = oc_fetch(link,NULL,OCDDS,0,&root);
    if(retval != OC_NOERR) {
        pass = 0;
        fprintf(stderr,"*** FAIL: Testing: user:pwd in %s\n",rcpath);
    } else {
        SHOW(root);
	retval = oc_close(link);
        fprintf(stderr,"*** PASS: Testing: user:pwd in %s\n",rcpath);
    }
    if(link != NULL)
	retval = oc_close(link);
    fflush(stderr);
#ifndef KEEPRC
    unlink(rcpath); /* delete the file */
#endif
    return pass;
}

static void
fillrc(const char* path)
{
    FILE* rc;
    killrc();

    rc = fopen(path,"w");
    if(rc == NULL) {
	fprintf(stderr,"cannot create rc file: %s\n",path);
	exit(1);
    }
#ifdef DEBUG
    fprintf(rc,"HTTP.VERBOSE=1\n");
    fprintf(rc,"HTTP.VALIDATE=1\n");
#endif
    fprintf(rc,"HTTP.COOKIEJAR=%s\n",COOKIEFILE);
    fprintf(rc,"HTTP.CREDENTIALS.USERPASSWORD=%s\n",USERPWD);
    fclose(rc);
}

static void
killrc()
{
    const char* home;
    char path[1024];
#ifdef KEEPRC
    fprintf(stderr,"kill: ./%s\n",RC);
#else
    snprintf(path,sizeof(path),"%s/%s",".",RC);
    unlink(path); /* delete the file */
#endif
    home = getenv("HOME");
#ifdef KEEPRC
    fprintf(stderr,"kill: %s/%s\n",home,RC);
#else
    snprintf(path,sizeof(path),"%s/%s",home,RC);
    unlink(path);
#endif
}

static void
show(OCddsnode root)
{
    ocdumpnode(root);
}
