/* Copyright 2009, UCAR/Unidata and OPeNDAP, Inc.
   See the COPYRIGHT file for more information. */

#define VALIDATE

#define ALLATONCE
#undef TRACK

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <assert.h>
#include <string.h>
#include <strings.h>

#include <oc.h>
#include <ocx.h>

/* Utilities */
#include "ocuri.h"
#include "oclist.h"
#include "oclog.h"
#include "ocbytes.h"

#ifdef WIN32
/*#include <windows.h>*/
#define snprintf _snprintf
#define strcasecmp stricmp
#endif

#ifndef nulldup
#define nulldup(s) (s==NULL?NULL:strdup(s))
#endif

#define CHECK(x) check_err((ocstat=(x)),0)
#define FAIL(x) check_err((x),1)

/* Define some classifiers */
#define iscontainer(t) ((t) == OC_Dataset || (t) == OC_Structure || (t) == OC_Sequence || (t) == OC_Grid)

#define isatomic(t) ((t) == OC_Atomic)

#define LBRACE "{"
#define RBRACE "}"

/*Mnemonic*/
#define TOPLEVEL 1

extern int ocdebug;

static OCerror ocstat;
static OClink glink;

/* define a large stack of DUMPPATH datanodes */
struct DUMPPATH {
    OCdatanode datanode;
    OCddsnode   node;
    OCtype      octype;
    size_t      rank;
    size_t      dimsizes[OC_MAX_DIMENSIONS];
    int         indexed; /* 1 => indices is valid */
    size_t      indices[OC_MAX_DIMENSIONS];
} stack[2048];

static size_t stacknext;

/* Define the debug options */
struct OCD {
    int debug;          /* -D<integer:1..> */
    int debuglevel;
    int dumpdds;        /* -DN */
    int dumpdatadds;    /* -DX<level 0..> */
    int dumpdatatree;   /* -DD */
    int dumplevel;
    int curl;           /* -DC - make curl be verbose */
} debug;

static char blanks[2048];
#define BLANKSPERDENT 2

#ifdef ENABLE_XGETOPT
extern int optind;
extern int opterr;
extern char* optarg;
#endif

/* Forward*/
static void usage(char*);
static int fail(char*);
static void check_err(int stat, int dofail);
static void dumpflags(void);

static OCerror processdata(int);
static OCerror printdata(OClink, OCdatanode);

static OCerror printdata_indices(OClink, OCdatanode, OCbytes*,int);
static OCerror printdata_container(OClink, OCdatanode, OCbytes*,int);
static OCerror printdata_leaf(OClink, OCdatanode, OCbytes*,int);

static off_t odom_init(size_t rank, size_t* indices, size_t* dimsizes);
static int odom_more(size_t rank, size_t* indices, size_t* dimsizes);
static void odom_next(size_t rank, size_t* indices, size_t* dimsizes);

static OCerror dumpdatanode(OClink, OCdatanode, size_t count, void* memory, OCbytes*);
static OCerror generatedds(OClink, OCddsnode, OCbytes*, int depth);
static char* generatedas(OClink,OCddsnode);
static OCerror generatedasr(OClink, OCddsnode, OCbytes*, int depth);
static OCerror generateddsattributes(OClink, OCddsnode node, OCbytes*, int);

static OCerror printdims(OClink link, OCddsnode node, OCbytes* buffer);
static char* stringescape(char*);
static char* idescape(char*, char*, size_t);
static int needsescapes(const char* s);
static size_t totaldimsize(size_t,size_t*);
static char* indent(int n);
static void pushstack(OCdatanode datanode);
static void popstack() {stacknext--;}
#ifdef TRACK
static void printstack(char* msg);
#endif

static char* optionmsg =
" [-A]"
" [-C <constraint>]"
" [-D <debugarg>]"
" [-L]"
" [-T]"
" [-U <userparam>]"
" [-g]"
" [-p das|dds|datadds]"
" [-v]"
" <url>";

/* Command line arguments */
int     showattributes;	/* -A */
int     logging;	/* -L */
int     octest; 	/* -T */ /* match original octest output */
OClist* userparams;	/* -U */
char*   constraint;	/* -C */
int     generate;	/* -g */
int     optdas;		/* -p */
int     optdatadds;	/* -p */
int     optdds;		/* -p */
int     verbose;	/* -v */
OCflags ocflags;
char*   urlsrc;  	/* <url> */

static void
init()
{
    showattributes = 0;       /* -A */
    logging = 0;              /* -L */
    octest = 0;               /* -T */
    userparams = oclistnew(); /* -U */
    constraint = NULL;        /* -C */
    generate = 1;             /* -g */
    optdas = 0;               /* -p */
    optdatadds = 0;           /* -p */
    optdds = 0;               /* -p */
    verbose = 0;              /* -v */
    ocflags = 0;
    urlsrc = NULL;            /* <url> */
    memset(&debug,0,sizeof(debug));
}

int
main(int argc, char **argv)
{
    int c;
    OCURI* tmpurl;
    char* suffix;

    init();

    opterr = 1;

    while ((c = getopt(argc, argv, "ABC:D:LTU:ghp:v")) != EOF) {
        switch (c) {
	case 'A': showattributes = 1; break;
	case 'C': constraint = nulldup(optarg); break;
        case 'L': logging = 1; break;
        case 'T': octest = 1; break;
	case 'U': oclistpush(userparams,(void*)nulldup(optarg)); break;
        case 'D': {
	    int c0;
	    if(strlen(optarg) == 0) usage("missing -D argument");
	    c0 = optarg[0];
	    if(c0 >= '0' && c0 <= '9') {/* debug level */
		debug.debuglevel = atoi(optarg); break;
	    } else switch (c0) {
	           case 'D': debug.dumpdatatree = 1; break;
	           case 'N': debug.dumpdds = 1; break;
	           case 'X': debug.dumpdatadds = 1;
			     debug.dumplevel = atoi(optarg+1);
			     break;
	           case 'C': debug.curl = 1; break;
		   default: usage("unknown -D option");
		   }
	    } break;

        case 'g': generate = 1; break;
	case 'p':
	    if(strcasecmp(optarg,"das")==0) optdas=1;
	    else if(strcasecmp(optarg,"dds")==0) optdds=1;
	    else if(strcasecmp(optarg,"data")==0) optdatadds=1;
	    else if(strcasecmp(optarg,"datadds")==0) optdatadds=1;
	    else if(strcasecmp(optarg,"all")==0) {
		optdas = 1; optdds = 1; optdatadds = 1;
	    } else usage("unknown -p option");
	    break;
        case 'v': verbose = 1; break;

	case 'h': usage(""); exit(0);

        default: usage("unknown option");
        }
    }

    if (debug.debuglevel > 0) {
        ocdebug = debug.debuglevel;
    }

    if(logging) {
	ocloginit();
	ocsetlogging(1);
	oclogopen(NULL);
    }

    argc -= optind;
    argv += optind;

    if (argc > 0 && urlsrc == NULL) {
        urlsrc = nulldup(argv[argc - 1]);
    }

    if (urlsrc == NULL)
        urlsrc = getenv("URLSRC");

    if (urlsrc == NULL) {
        usage("no source url specified\n");
    }

    /* For convenience, see if the url has a trailing .dds, .das, or .dods
       and if so, use it
    */
    if(!ocuriparse(urlsrc,&tmpurl)) {
	fprintf(stderr,"malformed source url: %s\n",urlsrc);
	exit(1);
    }
    suffix = strrchr(tmpurl->file,'.');
    if(suffix != NULL) {
	int match = 0;
	if(strcmp(suffix,".das")==0) {
	    optdas = 1;
	    optdds = 0;
	    optdatadds = 0;
	    match = 1;
	} else if(strcmp(suffix,".dds")==0) {
	    optdas = 0;
	    optdds = 1;
	    optdatadds = 0;
	    match = 1;
	} else if(strcmp(suffix,".dods")==0) {
	    optdas = 0;
	    optdds = 0;
	    optdatadds = 1;
	    match = 1;
	}
	/* Reassemble minus the suffix */
	if(match) {
	    *suffix = '\0';
	    urlsrc = ocuribuild(tmpurl,NULL,NULL,
					OCURICONSTRAINTS|OCURIUSERPWD|OCURIENCODE);
	}
    }
    ocurifree(tmpurl);

    if (verbose)
        dumpflags();

    processdata(ocflags);

    return 0;
}

static void
dumpflags(void)
{
    if(showattributes) fprintf(stderr," -A");
    if(constraint) fprintf(stderr," -C %s",constraint);
    if(debug.debug) fprintf(stderr," -D%d",debug.debuglevel);
    if(debug.dumpdds) fprintf(stderr," -DN");
    if(debug.dumpdatatree) fprintf(stderr," -DD");
    if(debug.dumpdatadds) fprintf(stderr," -DX%d",debug.dumplevel);
    if(logging) fprintf(stderr," -L");
    if(oclistlength(userparams) > 0) {
	unsigned int i;
	fprintf(stderr," -U");
	for(i=0;i<oclistlength(userparams);i++) {
	    fprintf(stderr," %s",(char*)oclistget(userparams,i));
	}
    }
    if(generate) fprintf(stderr," -g");
    if(optdas || optdds || optdatadds) {
	fprintf(stderr," -p");
	if(optdas) fprintf(stderr," das");
	if(optdds) fprintf(stderr," dds");
	if(optdatadds) fprintf(stderr," datadds");
    }
    if(verbose) fprintf(stderr," -v");
    fprintf(stderr,"-f %s\n",urlsrc);
}

static void
usage(char* msg)
{
    if(msg) fprintf(stderr,"error: %s\n",msg);
    fprintf(stderr,"usage: octest %s\n",optionmsg);
    fail(NULL);
}

static int
fail(char* msg)
{
    if(msg) fprintf(stderr,"fatalerror: %s\n",msg);
    fflush(stdout); fflush(stderr);
    exit(1);
}

/******************************************/
static void
check_err(OCerror ocstat, int dofail)
{
    if(ocstat == OC_NOERR) return;
    fprintf(stderr,"error status returned: (%d) %s\n",ocstat,oc_errstring(ocstat));
    if(dofail) fail(NULL);
}

static OCerror
processdata(OCflags flags)
{
    char* totalurl;
    OClink link;
    OCddsnode dasroot, ddsroot, dataddsroot;
    OCdatanode rootdatanode;

    totalurl = nulldup(urlsrc);

    FAIL(oc_open(totalurl,&link));
    free(totalurl);
    glink = link;

    if(debug.curl)
	oc_trace_curl(link);

    if(optdas) {
        ocstat = oc_fetch(link,constraint,OCDAS,0,&dasroot);
        if(ocstat != OC_NOERR) {
            fprintf(stderr,"error status returned: (%d) %s\n",ocstat,oc_errstring(ocstat));
            fprintf(stderr,"Could not read DAS; continuing.\n");
            optdas = 0;
            showattributes = 0;
        } else if(generate) {
            char* das = generatedas(link,dasroot);
            fprintf(stdout,"%s",das);
            free(das);
        } else {
	    const char* text = oc_tree_text(link,dasroot);
            fprintf(stdout,"%s",(text?text:"null"));
        }
    }
    fflush(stdout);

    if(optdds) {
        ocstat = oc_fetch(link,constraint,OCDDS,flags,&ddsroot);
        if(ocstat != OC_NOERR) {
            fprintf(stderr,"error status returned: (%d) %s\n",ocstat,oc_errstring(ocstat));
            fprintf(stderr,"Could not read DDS; continuing.\n");
            optdds = 0;
        } else {
            if(showattributes && !optdas) {
                FAIL(oc_fetch(link,constraint,OCDAS,flags,&dasroot));
            }
            if(showattributes || optdas) {
                FAIL(oc_merge_das(link,dasroot,ddsroot));
            }
            if(generate) {
	        OCbytes* buffer = ocbytesnew();
                FAIL(generatedds(link,ddsroot,buffer,0));
                fprintf(stdout,"%s",ocbytescontents(buffer));
		ocbytesfree(buffer);
            } else {
                const char* text = oc_tree_text(link,ddsroot);
                fprintf(stdout,"%s",(text?text:"null"));
            }
        }
        if(debug.dumpdds)
            oc_dds_ddnode(link,ddsroot);
    }
    fflush(stdout);

    if(optdatadds) {
        ocstat = oc_fetch(link,constraint,OCDATADDS,flags,&dataddsroot);
        if(ocstat) {
            if(constraint)
                fprintf(stderr,"Cannot read DATADDS: %s?%s\n",urlsrc,constraint);
            else
                fprintf(stderr,"Cannot read DATADDS: %s\n",urlsrc);
            exit(1);
        }
        if(debug.dumpdds)
            oc_dds_ddnode(link,dataddsroot);
        if(debug.dumpdatadds)
            oc_dds_dd(link,dataddsroot,debug.dumplevel);

        FAIL(oc_dds_getdataroot(link,dataddsroot,&rootdatanode));
        if(debug.dumpdatatree)
	    oc_data_ddtree(link,rootdatanode);
        stacknext = 0;
        printdata(link,rootdatanode);
    }
    fflush(stdout);

    oc_close(link);
    return OC_NOERR;
}

/**
This is the main procedure for
printing a data tree.
*/
static OCerror
printdata(OClink link, OCdatanode datanode)
{
    OCbytes* buffer;
    OCtype octype;

    buffer = ocbytesnew();

    /* verify that datanode is a Dataset datanode */
    FAIL(oc_data_octype(link,datanode,&octype));
    assert(octype == OC_Dataset);

    printdata_container(link,datanode,buffer,TOPLEVEL);

    fprintf(stdout,"%s",ocbytescontents(buffer));

    ocbytesfree(buffer);
    return OC_NOERR;
}


/* Print a single container datanode */
static OCerror
printdata_container(OClink link, OCdatanode datanode, OCbytes* buffer, int istoplevel)
{
    OCerror stat = OC_NOERR;
    int i;
    OCddsnode node;
    OCtype octype;
    size_t nsubnodes;

    /* Obtain some information about the node */     
    FAIL(oc_data_ddsnode(link,datanode,&node));
    FAIL(oc_dds_nsubnodes(link,node,&nsubnodes));
    FAIL(oc_data_octype(link,datanode,&octype));

    /* If this is not a single instance container, then
       defer to the appropriate function */
    if(isatomic(octype))
	return printdata_leaf(link,datanode,buffer,istoplevel);
    if(oc_data_indexable(link,datanode))
	return printdata_indices(link,datanode,buffer,!TOPLEVEL);

    /* Must be a container instance or a record */
    for(i=0;i<nsubnodes;i++) {
        OCdatanode field;
	FAIL(oc_data_ithfield(link,datanode,i,&field));
	pushstack(field);
        FAIL(printdata_indices(link,field,buffer,istoplevel));
	popstack();
	oc_data_free(link,field);
	if(stat != OC_NOERR) break;
    }
    return stat;
}

static OCerror
printdata_indices(OClink link, OCdatanode datanode, OCbytes* buffer, int istoplevel)
{
    OCerror stat = OC_NOERR;
    int i;
    OCddsnode node;
    size_t rank;
    OCtype octype;
    size_t dimsizes[OC_MAX_DIMENSIONS];
    size_t indices[OC_MAX_DIMENSIONS];

    /* Obtain some information about the node */     
    FAIL(oc_data_ddsnode(link,datanode,&node));
    FAIL(oc_dds_octype(link,node,&octype));
    FAIL(oc_dds_rank(link,node,&rank));

    /* If this is not an indexed structure or a sequence then
       defer to the appropriate function */
    if(isatomic(octype))
	return printdata_leaf(link,datanode,buffer,istoplevel);
    if(iscontainer(octype) && !oc_data_indexable(link,datanode))
	return printdata_container(link,datanode,buffer,istoplevel);

    /* Iterate over the datanodes */
    if(octype == OC_Structure) {
	/* Get dimension sizes */
        FAIL(oc_dds_dimensionsizes(link,node,dimsizes));
        
	/* init odometer and get cross-product */
	odom_init(rank,indices,dimsizes);
        while(odom_more(rank,indices,dimsizes)) {
	    OCdatanode element;
	    FAIL(oc_data_ithelement(link,datanode,indices,&element));
            pushstack(element);
	    /* walk the container */	
            printdata_container(link,element,buffer,!TOPLEVEL);
            popstack();
	    oc_data_free(link,element);
	    odom_next(rank,indices,dimsizes);
        }
    } else if(octype == OC_Sequence) {
        /* Dump each element */
        for(i=0;;i++) {
	    OCdatanode record;
	    stat = oc_data_ithrecord(link,datanode,i,&record);
	    if(stat != OC_NOERR) {
	        if(stat == OC_EINDEX) break;
	        return stat;
	    }
            pushstack(record);
            printdata_container(link,record,buffer,!TOPLEVEL); /* print current record */
            popstack();
	    oc_data_free(link,record);
        }
#ifdef VALIDATE
	{
	    size_t count;
	    FAIL(oc_data_recordcount(link,datanode,&count));
	    assert(count == i);
        }
#endif
    } else
        abort();

    return OC_NOERR;
}

static OCerror
printdata_leaf(OClink link, OCdatanode datanode, OCbytes* buffer, int istoplevel)
{
    OCddsnode node;
    OCtype octype,atomtype;
    size_t elemsize;
    size_t memsize;
    char* memory;
    size_t count,rank;

    /* Obtain some information about the node */     
    FAIL(oc_data_ddsnode(link,datanode,&node));
    FAIL(oc_dds_octype(link,node,&octype));
    FAIL(oc_dds_atomictype(link,node,&atomtype));
    FAIL(oc_dds_rank(link,node,&rank));

    assert(octype == OC_Atomic);

    /* If this variable is top-level then
       use the oc_dds_read functions
       in order to test them
    */

    elemsize = oc_typesize(atomtype);
    
    if(rank == 0) {/* Scalar case */
	memory = calloc(elemsize,1); /* reading only one value */
        /* read the scalar */
	if(istoplevel) {
	    FAIL(oc_dds_read(link,node,NULL,NULL,elemsize,memory));
	} else {
	    FAIL(oc_data_read(link,datanode,NULL,NULL,elemsize,memory));
	}
        count = 1;
    } else { 
	size_t dimsizes[OC_MAX_DIMENSIONS];
	size_t indices[OC_MAX_DIMENSIONS];
        FAIL(oc_dds_dimensionsizes(link,node,dimsizes));
	/* init odometer and get cross-product */
	count = odom_init(rank,indices,dimsizes);
        memsize = elemsize*count;
        memory = calloc(memsize,1);
    
#ifdef ALLATONCE /* read all at once */
        /* indices should be all zeros at this point */
	if(istoplevel) {
	    FAIL(oc_dds_read(link,node,indices,dimsizes,memsize,memory));
	} else {
	    FAIL(oc_data_read(link,datanode,indices,dimsizes,memsize,memory));
	}
#else /* BYITEM */
        {
  	    size_t offset;
	    size_t one[OC_MAX_DIMENSIONS]; 
            /* Initialize the read-by-one counts */ 
	    for(i=0;i<rank;i++) one[i]=0; 
	    one[rank-1] = 1; 
            /* Read whole atomic array item by item using an odometer */
	    for(offset=0,i=0;i<count;i++,offset+=elemsize) {
		if(!odom_more(rank,indices,dimsizes))
		    abort();
		if(istoplevel) {
		    FAIL(oc_dds_read(link,node,
                                      indices,one,
				      elemsize,memory+offset));
		} else {
	            FAIL(oc_data_read(link,datanode,
				       indices,one,
                                       elemsize,memory+offset));
		}
		odom_next(rank,indices,dimsizes);
	    }
        }
#endif
    }
    dumpdatanode(link,datanode,count,memory,buffer);
    if(atomtype == OC_String || atomtype == OC_URL)
	oc_reclaim_strings(count,(char**)memory);
    free(memory);
    return OC_NOERR;
}

static OCerror
generatedds(OClink link, OCddsnode node, OCbytes* buffer, int depth)
{
    size_t i,rank,nattr,nsubnodes;
    OCtype octype, atomtype;
    OCddsnode container,field;
    char id1[1024];
    char* name;

    ocbytescat(buffer,indent(depth));

    /* get all info about the node */
    FAIL(oc_dds_properties(link,node,&name,&octype,&atomtype,&container,
                             &rank,&nsubnodes,&nattr));

    if(octype == OC_Atomic) {
        ocbytescat(buffer,oc_typetostring(atomtype));
	ocbytescat(buffer," ");
	ocbytescat(buffer,idescape(name,id1,sizeof(id1)));
        /* dump dim info (if any)*/
	printdims(link,node,buffer);
        ocbytescat(buffer,";\n");
        generateddsattributes(link,node,buffer,depth+1);
    } else { /*must be container*/
	const char* typename = oc_typetostring(octype);
        ocbytescat(buffer,typename);
        ocbytescat(buffer," ");
        ocbytescat(buffer,LBRACE);
        ocbytescat(buffer,"\n");
        for(i=0;i<nsubnodes;i++) {
	    FAIL(oc_dds_ithfield(link,node,i,&field));
	    if(octype == OC_Grid) {
                ocbytescat(buffer,indent(depth));
		switch (i) {
		case 0: ocbytescat(buffer,"Array:\n"); break;
		case 1: ocbytescat(buffer,"Maps:\n"); break;
		default: break;
		}
 	    }
	    generatedds(link,field,buffer,depth+1);
        }
        ocbytescat(buffer,indent(depth));
        ocbytescat(buffer,RBRACE);
        ocbytescat(buffer," ");
        ocbytescat(buffer,idescape(name,id1,sizeof(id1)));
	printdims(link,node,buffer);
        ocbytescat(buffer,";\n");
        generateddsattributes(link,node,buffer,depth+1);
    }
    if(name) free(name);
    return OC_NOERR;
}

static OCerror
printdims(OClink link, OCddsnode node, OCbytes* buffer)
{
    int i;
    size_t rank,size;
    OCddsnode dimids[OC_MAX_DIMENSIONS];
    char tmp[1024];
    char id1[1024];

    FAIL(oc_dds_rank(link,node,&rank));
    if(rank == 0) return OC_NOERR;

    FAIL(oc_dds_dimensions(link,node,dimids));
    for(i=0;i<rank;i++) {
	OCddsnode dim = dimids[i];
	char* dimname = NULL;
	FAIL(oc_dimension_properties(link,dim,&size,&dimname));
	if(dimname == NULL)
	    snprintf(tmp,sizeof(tmp),"[%lu]",(unsigned long)size);
        else
	    snprintf(tmp,sizeof(tmp),"[%s=%lu]",idescape(dimname,id1,sizeof(id1)),(unsigned long)size);
	ocbytescat(buffer,tmp);
        if(dimname) free(dimname);
    }
    return OC_NOERR;
}

static OCerror
generateddsattributes(OClink link, OCddsnode node, OCbytes* buffer, int depth)
{
    size_t i,j;
    char tmp[128];
    size_t nattrs;
    char* aname = NULL;
    char* name = NULL;
    OCtype atomtype;
    size_t nvalues;
    char** values = NULL;
    char id1[1024];

    FAIL(oc_dds_attr_count(link,node,&nattrs));
    FAIL(oc_dds_name(link,node,&name));

    if(showattributes && nattrs > 0) {
        for(i=0;i<nattrs;i++) {
            FAIL(oc_dds_attr(link,node,i,NULL,NULL,&nvalues,NULL));
   	    values = (char**)malloc(nvalues*sizeof(char*));
            FAIL(oc_dds_attr(link,node,i,&aname,&atomtype,NULL,values));
            snprintf(tmp,sizeof(tmp),"%s%s %s:%s = ",indent(depth),
                        oc_typetostring(atomtype),idescape(name,id1,sizeof(id1)),aname);
            ocbytescat(buffer,tmp);
            for(j=0;j<nvalues;j++) {
                if(j > 0) ocbytescat(buffer,", ");
		if(needsescapes(values[j])) {
		    char* escaped = stringescape(values[j]);
                    ocbytescat(buffer,"\"");
                    ocbytescat(buffer,escaped);
                    ocbytescat(buffer,"\"");
		    if(escaped) free(escaped);
		} else
                    ocbytescat(buffer,values[j]);
            }
            ocbytescat(buffer,";\n");
	    oc_reclaim_strings(nvalues,values);
	    if(values) free(values);
            if(aname) free(aname);
	    values = NULL;
	    aname = NULL;
        }
    }
    if(name) free(name);
    return OC_NOERR;
}

static char*
generatedas(OClink link, OCddsnode root)
{
    size_t i, nsubnodes;
    char* result;
    OCbytes* buffer = ocbytesnew();

    FAIL(oc_dds_nsubnodes(link,root,&nsubnodes));
    ocbytescat(buffer,"Attributes {\n");
    for(i=0;i<nsubnodes;i++) {
	OCddsnode attr;
	FAIL(oc_dds_ithfield(link,root,i,&attr));
        generatedasr(link,attr,buffer,1);
    }
    ocbytescat(buffer,"}\n");
    result = ocbytesdup(buffer);
    ocbytesfree(buffer);
    return result;
}

static OCerror
generatedasr(OClink link, OCddsnode ddsnode, OCbytes* buffer, int depth)
{
    size_t i,nsubnodes;
    char tmp[256];
    OCtype octype, atomtype;
    char* name = NULL;
    char id1[1024];

    /* get some node info */
    FAIL(oc_dds_name(link,ddsnode,&name));
    FAIL(oc_dds_octype(link,ddsnode,&octype));
    FAIL(oc_dds_atomictype(link,ddsnode,&atomtype));

    if(octype == OC_Attributeset) {
        /* get node subcount */
        FAIL(oc_dds_nsubnodes(link,ddsnode,&nsubnodes));
        snprintf(tmp,sizeof(tmp),"%s%s {\n",indent(depth),idescape(name,id1,sizeof(id1)));
        ocbytescat(buffer,tmp);
        for(i=0;i<nsubnodes;i++) {
	    OCddsnode attr;
	    FAIL(oc_dds_ithfield(link,ddsnode,i,&attr));
            generatedasr(link,attr,buffer,depth+1);
        }
        ocbytescat(buffer,indent(depth));
        ocbytescat(buffer,"}\n");
    } else if(octype == OC_Attribute) {
        /* get some info about the node */
	size_t nvalues;
	FAIL(oc_das_attr_count(link,ddsnode,&nvalues));
        snprintf(tmp,sizeof(tmp),"%s%s %s",indent(depth),
                oc_typetostring(atomtype),idescape(name,id1,sizeof(id1)));
        ocbytescat(buffer,tmp);
        for(i=0;i<nvalues;i++) {
            char* value;
            OCtype ptype;
            FAIL(oc_das_attr(link,ddsnode,i,&ptype,&value));
            if(i > 0) ocbytescat(buffer,",");
            if(ptype == OC_String || ptype == OC_URL) {
                char* se = stringescape(value);
                snprintf(tmp,sizeof(tmp)," \"%s\"",se);
                free(se);
            } else
                snprintf(tmp,sizeof(tmp)," %s",value);
            ocbytescat(buffer,tmp);
            free(value);
        }
        ocbytescat(buffer,";\n");
    } else {
        snprintf(tmp,sizeof(tmp),"ocget DAS: unexpected type: %d",(int)octype);
        ocbytescat(buffer,tmp);
    }
    if(name) free(name);
    return OC_NOERR;
}

static char hexdigits[] = "0123456789abcdef";

/* Add escape characters to a string */
static char*
stringescape(char* s)
{
    size_t len;
    char* p;
    int c;
    char* escapedstring;

    if(s == NULL) return NULL;
    len = strlen(s);
    escapedstring = (char*)malloc(4*len);
    p = escapedstring;
    while((c=*s++)) {
        if(c == '"' || c == '\\') {*p++ = '\\'; *p++ = c;}
        else if (c < ' ' || c >= 127) {
            *p++ = '\\'; *p++ = 'x';
            *p++ = hexdigits[(c & 0xf0)>>4];
            *p++ = hexdigits[(c & 0xf)];
        } else
            *p++ = c;
    }
    *p = '\0';
    return escapedstring;
}

static char idchars[] = "_%";

/* Add escape characters to an identifier */
static char*
idescape(char* id, char* escapeid, size_t esize)
{
    char* p;
    int c;

    if(id == NULL) return NULL;
    p = escapeid;
    *p = '\0';
    esize--; /* leave room for null */
    while(esize-- > 0 && (c=*id++)) {
        if(c >= '0' && c <= '9') {*p++ = c;}
        else if(c >= 'a' && c <= 'z') {*p++ = c;}
        else if(c >= 'A' && c <= 'Z') {*p++ = c;}
        else if(strchr(idchars,c) != NULL) {*p++ = c;}
        else {
            *p++ = '%';
            *p++ = hexdigits[(c & 0xf0)>>4];
            *p++ = hexdigits[(c & 0xf)];
        }
    }
    *p = '\0';
    return escapeid;
}

static char valuechars[] = " \\\"";

/**
Return 1 if the given string, used as a value, should be escaped.
*/
static int
needsescapes(const char* s)
{
    const char* p = s;
    int c;
    while((c=*p++)) {
	if(strchr(valuechars,c) != NULL)
	    return 1; /* needs to be escaped */
    }
    return 0;    
}


static OCerror
dumpdatanode(OClink link, OCdatanode datanode, size_t count, void* memory, OCbytes* buffer)
{
    size_t i;
    size_t delta;
    OCddsnode node;
    OCtype atomtype;
    OCtype octype;
    OCbytes* path = NULL;
    char* name;
    char id[1024];
    char tmp[1024];
    struct DUMPPATH* entry = NULL;

    FAIL(oc_data_ddsnode(link,datanode,&node));
    FAIL(oc_dds_octype(link,node,&octype));
    FAIL(oc_dds_atomictype(link,node,&atomtype));
    delta = oc_typesize(atomtype);

#ifdef TRACK
    printstack("dumpdatanode");
#endif

    /* construct the fully qualified name from the stack; watch out for duplicates
       from e.g. sequence versus record */
    path = ocbytesnew();
    for(i=0;i<stacknext;i++) {
	entry = stack + i;
	/* check for duplicate */
	if(i<(stacknext-1) && entry->node == stack[i+1].node) continue;

	/* Get various pieces of additional node information */
        FAIL(oc_dds_name(glink,entry->node,&name));
        (void)idescape(name,id,sizeof(id));
	if(name) free(name);

        switch (entry->octype) {

        case OC_Dataset:
            break;

        case OC_Structure:
            ocbytescat(path,"/");
            ocbytescat(path,id);
            if(entry->rank > 0) {
		for(i=0;i<entry->rank;i++) {
                    sprintf(tmp,"[%lu]",(unsigned long)entry->indices[i]);
                    ocbytescat(path,tmp);
		}
            }
	    break;

        case OC_Grid:
            ocbytescat(path,"/");
            ocbytescat(path,id);
	    break;

        case OC_Sequence:
            ocbytescat(path,"/");
            ocbytescat(path,id);
            sprintf(tmp,"[%lu]",(unsigned long)entry->indices[0]);
            ocbytescat(path,tmp);
            break;

        case OC_Atomic:
            ocbytescat(path,"/");
            ocbytescat(path,id);
            break; /* deal with below */

        default: ocbytescat(path,"?"); break;
        }
    }
    /* work with the final entry */
    assert(entry == (stack + (stacknext - 1)));
    assert(datanode == entry->datanode);
    snprintf(tmp,sizeof(tmp),"%s %s",
                oc_typetostring(atomtype),
                ocbytescontents(path));
    ocbytescat(buffer,tmp);
    if(entry->rank > 0) {
	if(octest) { /* Match the octest output */
	    off_t xproduct;
	    xproduct = totaldimsize(entry->rank,entry->dimsizes);
            snprintf(tmp,sizeof(tmp),"[0..%lu]",(unsigned long)xproduct-1);
            ocbytescat(buffer,tmp);
	} else {
	    for(i=0;i<entry->rank;i++) {
                snprintf(tmp,sizeof(tmp),"[0..%lu]",(unsigned long)entry->dimsizes[i]-1);
                ocbytescat(buffer,tmp);
	    }
	}
    }
    count = totaldimsize(entry->rank,entry->dimsizes);
    for(i=0;i<count;i++) {
        char *memory_local = memory;
        ocbytescat(buffer," ");
        oc_typeprint(atomtype,memory_local+(i*delta),sizeof(tmp),tmp);
        ocbytescat(buffer,tmp);
    }
    ocbytescat(buffer,"\n");
    ocbytesfree(path);
    return OC_NOERR;
}

static off_t
odom_init(size_t rank, size_t* indices, size_t* dimsizes) 
{ 
    int i; 
    off_t count;
    for(count=1,i=0;i<rank;i++) {
        indices[i] = 0;
	count *= dimsizes[i];
    }
    return count;
} 

static void
odom_next(size_t rank, size_t* indices, size_t* dimsizes) 
{ 
    int i; 
    for(i=rank-1;i>=0;i--) { 
	indices[i]++; 
	if(indices[i] < dimsizes[i]) break; 
	if(i > 0) indices[i] = 0; 
    } 
}

/* Return 0 if we have exhausted the indices, 1 otherwise */
static int
odom_more(size_t rank, size_t* indices, size_t* dimsizes) 
{
    if(indices[0] >= dimsizes[0]) return 0;
    return 1;
} 

/* Compute total # of elements if dimensioned */
static size_t
totaldimsize(size_t rank, size_t* sizes)
{
    unsigned int i;
    size_t count = 1;
    for(i=0;i<rank;i++) {
        count *= sizes[i];
    }
    return count;
}

static char*
indent(int n)
{
    int nblanks = BLANKSPERDENT * n;
    memset(blanks,' ',nblanks);
    blanks[nblanks] = '\0';
    return blanks;
}

static void
pushstack(OCdatanode datanode)
{
    struct DUMPPATH* entry = stack+stacknext;
    entry->datanode = datanode;
    FAIL(oc_data_ddsnode(glink,entry->datanode,&entry->node));
    FAIL(oc_dds_octype(glink,entry->node,&entry->octype));
    FAIL(oc_dds_rank(glink,entry->node,&entry->rank));
    if(entry->rank > 0) {
        FAIL(oc_dds_dimensionsizes(glink,entry->node,entry->dimsizes));
    }
    entry->indexed = oc_data_indexed(glink,entry->datanode);
    if(entry->indexed) {
	FAIL(oc_data_position(glink,entry->datanode,entry->indices));
    }
    stacknext++;
}


#ifdef TRACK
static void printstack(char* msg)
{
    size_t i,j;
    struct DUMPPATH* entry;
    fprintf(stderr,"\n%s@stack: %u\n",msg,stacknext);
    for(entry=stack,i=0;i<stacknext;i++,entry++) {
	OCdatanode datanode = entry->datanode;
	OCddsnode node;
	size_t rank;
	size_t edges[OC_MAX_DIMENSIONS];
	char* name;
        FAIL(oc_dds_rank(glink,entry->node,&rank));
        if(entry->rank > 0)
            FAIL(oc_dds_dimensionsizes(glink,entry->node,edges));
        FAIL(oc_dds_name(glink,node,&name));
        fprintf(stderr,"    [%d] (%s)",(int)i,name)
	for(j=0;j<rank;j++) 
            fprintf(stderr,"[%u]",(unsigned int)edges[j]);
        fprintf(stderr,"\n");
    }
}
#endif
