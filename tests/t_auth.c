#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#define DEBUG

#include "oc.h"

#undef NOEMBED
#define NOLOCAL
#define NOHOME
#define NOSPEC
#define NOREDIR

#define KEEPRC

#define RC ".ocrc"

#define URLSERVER "remotetest.unidata.ucar.edu"
#define USERPWD "tiggeUser:tigge"
#define COOKIEFILE "./cookies"

#define URL1 "https://%s@%s/thredds/dodsC/restrict/testData.nc"
#define URL2 "https://%s/thredds/dodsC/restrict/testData.nc"
#define URL3 "https://%s@thredds-test.ucar.edu/thredds/dodsC/restrict/testData.nc"

/* Embedded user:pwd */
static char url1[1024];

/* user:pwd from RC*/
static char url2[1024];

/* Test redirect from different machine*/
static char url3[1024];


static OCerror createrc(const char*);
static OCerror testrc(const char* url);
static void killrc();

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
    char rcpath[1024];
    
    fprintf(stderr,"Testing: Authorization\n");

    snprintf(url1,sizeof(url1),URL1,USERPWD,URLSERVER); /* embedded */
    snprintf(url2,sizeof(url2),URL2,URLSERVER); /* using rc file */

#ifdef DEBUG
fprintf(stderr,"url1: %s\n",url1);
fprintf(stderr,"url2: %s\n",url2);
fflush(stderr);
#endif

    pass = 1; /* assume success */

    fprintf(stderr,"Testing: Http Basic Authorization\n\n");
#ifndef NOEMBED
    {
	oc_initialize(); /* reset liboc */
        killrc();
        fprintf(stderr,"Testing: Embedded user:pwd: %s\n",url1);
        retval = oc_open(url1, &link);
	if(retval == OC_NOERR)
	    retval = oc_fetch(link,NULL,OCDDS,0,&root);
	if(retval == OC_NOERR)
	    ocdumpnode(root);
        if(retval != OC_NOERR) {
            fprintf(stderr,"*** FAIL: Testing embedded user:pwd\n");
            pass = 0;
        } else {
            fprintf(stderr,"*** PASS: Testing embedded user:pwd\n");
	}
	if(link != NULL)
	    retval = oc_close(link);
        fflush(stderr);
    }
#endif

#ifndef NOLOCAL
    {
	oc_initialize(); /* reset liboc */
        killrc();
        /* Test 1: RC in ./ */
        snprintf(rcpath,sizeof(rcpath),"%s/%s",".",RC);
        fprintf(stderr,"Testing: user:pwd in %s\n",rcpath);
	if(createrc(rcpath) != OC_NOERR) {
	    fprintf(stderr,"user:pwd in %s; could not create\n",rcpath);
	    exit(1);
	}
	if(testrc(url2) != OC_NOERR) {
            fprintf(stderr,"*** FAIL: Testing: user:pwd in %s\n",rcpath);
	    pass = 0;
        } else {
            fprintf(stderr,"*** PASS: Testing: user:pwd in %s\n",rcpath);
        }
    }
#endif

#ifndef NOHOME
    {
	oc_initialize(); /* reset liboc */
        /* Test 1: RC in $HOME */
	home = getenv("HOME");
        snprintf(rcpath,sizeof(rcpath),"%s/%s",home,RC);
        fprintf(stderr,"Testing: user:pwd in %s\n",rcpath);
        killrc();
	if(createrc(rcpath) != OC_NOERR) {
	    fprintf(stderr,"user:pwd in %s; could not create\n",rcpath);
	    exit(1);
	}
	if(testrc(url2) != OC_NOERR) {
            fprintf(stderr,"*** FAIL: Testing: user:pwd in %s\n",rcpath);
	    pass = 0;
        } else {
            fprintf(stderr,"*** PASS: Testing: user:pwd in %s\n",rcpath);
        }
    }
#endif

#ifndef NOSPEC
    {
	const char* tmp = getenv("TEMP");
	if(tmp == NULL)
	    tmp = "/tmp";
	oc_initialize(); /* reset liboc */
        snprintf(rcpath,sizeof(rcpath),"%s/%s",tmp,RC);
	fprintf(stderr,"Testing rc file in specified directory %s\n",rcpath);
        /* Test: Create the rc file in specified dir */
        fprintf(stderr,"Testing: user:pwd in %s\n",rcpath);
        killrc();
	if(createrc(rcpath) != OC_NOERR) {
	    fprintf(stderr,"user:pwd in %s; could not create\n",rcpath);
	    exit(1);
	}
	if(testrc(url2) != OC_NOERR) {
            fprintf(stderr,"*** FAIL: Testing: user:pwd in %s\n",rcpath);
	    pass = 0;
        } else {
            fprintf(stderr,"*** PASS: Testing: user:pwd in %s\n",rcpath);
        }
    }
#endif

#ifndef NOREDIR
   {
	oc_initialize(); /* reset liboc */
	link = NULL;
        fprintf(stderr,"Testing: Http Basic Redirect\n\n");
	snprintf(url3,sizeof(url3),URL3,USERPWD);
        fprintf(stderr,"Basic redirect: %s\n",url3);
        retval = oc_open(url3, &link);
	if(retval == OC_NOERR)
	    retval = oc_fetch(link,NULL,OCDDS,0,&root);
	if(retval == OC_NOERR)
	    ocdumpnode(root);
        if(retval != OC_NOERR) {
            fprintf(stderr,"*** XFAIL: Basic redirect\n");
        } else {
            fprintf(stderr,"*** PASS: Basic redirect\n");
	}
	if(link != NULL)
	    retval = oc_close(link);
        fflush(stderr);
    }
#endif

    return !pass;
}

static OCerror
createrc(const char* rcpath)
{
    FILE* rc;

    killrc();
    rc = fopen(rcpath,"w");
    if(rc == NULL) {
        fprintf(stderr,"Cannot create %s\n",rcpath);
        exit(1);
    }    
#ifdef DEBUG
    fprintf(rc,"HTTP.VERBOSE=1\n");
#endif
    fprintf(rc,"HTTP.COOKIEJAR=%s\n",COOKIEFILE);
    fprintf(rc,"HTTP.VALIDATE=1\n");
    fprintf(rc,"HTTP.CREDENTIALS.USERPASSWORD=%s\n",USERPWD);
    fclose(rc);
    return OC_NOERR;
}

static OCerror
testrc(const char* url)
{
    OCerror retval = OC_NOERR;
    OClink link = NULL;
    OCddsnode root = NULL;

    retval = oc_open(url, &link);
    if(retval == OC_NOERR)
	retval = oc_fetch(link,NULL,OCDDS,0,&root);
    if(retval == OC_NOERR && root != NULL)
	ocdumpnode(root);
    if(link != NULL)
	retval = oc_close(link);
    fflush(stderr);
#ifndef KEEPRC
    unlink(rcpath); /* delete the file */
#endif
    return retval;
}

static void
killrc()
{
    const char* home;
    const char* tmp;
    char path[1024];
    snprintf(path,sizeof(path),"%s/%s",".",RC);
    unlink(path); /* delete the file */
    home = getenv("HOME");
    snprintf(path,sizeof(path),"%s/%s",home,RC);
    unlink(path);
    tmp = getenv("TEMP");
    if(tmp == NULL)
	tmp = "/tmp";
    snprintf(path,sizeof(path),"%s/%s",tmp,RC);
    unlink(path);
    
}
