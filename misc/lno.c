#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static char digits[] = "0123456789";

static int readline(FILE*);

static char line[4096];

int
main(int argc, char** argv)
{
    FILE* f;
    int counter;
    int inpre;

    if(argc == 1) {
	fprintf(stderr,"no arg\n");
	exit(1);
    } else {
        f = fopen(argv[1],"r");
        if(f == NULL) {
	    perror("fopen:");
	    exit(1);
	}
    }    

    for(counter=0,inpre=0;;) {
	int eof = readline(f);
	if(eof) break;
	/* Is this a <pre> ? */
	if(inpre) {
   	    int hasline = (strchr(digits,line[0]) && strchr(digits,line[1]) ? 1 : 0);
	    if(strncmp(line,"</pre>",strlen("</pre>")) == 0) {inpre = 0;}
	    else if(hasline) { /* make line numbers consecutive */
   	        char lno[8];
		counter++;
		snprintf(lno,sizeof(lno),"%02d",counter);		
	        printf("%s%s\n",lno,&line[2]);
	        continue; /* don't output line twice */
	    }
	} else if(strncmp(line,"<pre>",strlen("<pre>")) == 0) {
	    inpre = 1;
	    counter = 0;
	}
        printf("%s\n",line); /* pass thru */
    }	
    if(f) fclose(f);
    exit(0);
}

static int
readline(FILE* f)
{
    int c;
    int pos = 0;
    line[0] = '\0';
    for(;;) {
	c=fgetc(f);
	if(c == EOF) return 1; /* is EOF */
	if(c == '\r') continue;
	if(c == '\n') break;
	if(pos == sizeof(line)-1) {
	    fprintf(stderr,"line too long\n");
	    exit(1);
	}
	line[pos++] = (char)c;
    }
    line[pos] = '\0';
    return 0; /* !EOF */
}
