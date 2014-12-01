#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#define DEBUG

#include "netcdf.h"

#undef NOEMBED
#define NOLOCAL
#define NOHOME
#define NOREDIR

#define KEEPRC

#define RC ".ocrc"
#define SPECRC "./ocrc"

#define URLSERVER "54.172.97.47"
#define USERPWD "dennis.heimbigner:AseyMay0"
#define COOKIEFILE "./cookies"

#define URL1 "https://%s@%s/opendap/data/nc/fnoc1.nc"
#define URL2 "https://%s/opendap/data/nc/fnoc1.nc"

/* Embedded user:pwd */
static char url1[1024];

/* user:pwd from RC*/
static char url2[1024];

static int testrc(const char* prefix, const char* url);
static void fillrc(const char* path);
static void killrc();

#ifdef DEBUG
static void
CHECK(int e, const char* msg)
{
    if(e == NC_NOERR) return;
    if(msg == NULL) msg = "Error";
    fprintf(stderr,"%s: %s\n", msg, nc_strerror(e));
    exit(1);
}
#endif

int
main(int argc, char** argv)
{
    int ncid,retval,pass;
    FILE* rc;
    const char* dfaltsvc;
    char buffer[8192];
    const char* home;

    fprintf(stderr,"Testing: Authorization\n");

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
        retval = nc_open(url1, 0, &ncid);
        if(retval != NC_NOERR) {
            pass = 0;
            fprintf(stderr,"*** FAIL: Testing embedded user:pwd\n");
        } else {
            fprintf(stderr,"*** PASS: Testing embedded user:pwd\n");
	    retval = nc_close(ncid);
	}
        fflush(stderr);
    }
#endif

#ifndef NOLOCAL
    {
        /* Test 1: RC in ./ */
        fprintf(stderr,"Testing: user:pwd in %s/%s: %s\n",".",RC);
	if(!testrc(".",url2)) {
	    fprintf(stderr,"user:pwd in %s/%s failed\n",".",RC);
	    exit(1);
        }
    }
#endif

#ifndef NOHOME
    {
        /* Test 1: RC in HOME  */
	home = getenv("HOME");
        fprintf(stderr,"user:pwd in %s/%s: %s\n",home,RC);
	if(!testrc(home,url2)) {
	    fprintf(stderr,"user:pwd in %s/%s failed\n",home,RC);
	    exit(1);
        }
    }
#endif

    return !pass;
}

static int
testrc(const char* prefix, const char* url)
{
    int pass = 1;
    int retval;
    int ncid;
    char rcpath[8192];
    FILE* rc;

    snprintf(rcpath,sizeof(rcpath),"%s/%s",prefix,RC);
    rc = fopen(rcpath,"w");
    if(rc == NULL) {
        fprintf(stderr,"Cannot create ./%s\n",RC);
        exit(1);
    }    
    fclose(rc);
    fillrc(rcpath);
    retval = nc_open(url, 0, &ncid);
    if(retval != NC_NOERR) {
        pass = 0;
        fprintf(stderr,"*** FAIL: Testing: user:pwd in %s\n",rcpath);
    } else {
	retval = nc_close(ncid);
        fprintf(stderr,"*** PASS: Testing: user:pwd in %s\n",rcpath);
    }
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
#endif
    fprintf(rc,"HTTP.COOKIEJAR=%s\n",COOKIEFILE);
    fprintf(rc,"HTTP.VALIDATE=1\n");
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
