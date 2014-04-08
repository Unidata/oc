#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static char digits[] = "0123456789";

static int readline(FILE*);

static char line[4096];

static int exno = 0;

int
main(int argc, char** argv)
{
    FILE* f;
    int excounter;

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
    if(argc == 3) {
	exno = atol(argv[2]);
    } else
	exno = 1;

    for(;;) {
	int hasline;
	int eof = readline(f);
	if(eof) break;
	/* Are we starting a new example? */
	if(strcmp(line,"<pre>/*Example 1*/") ==0) {
	    excounter = 1;
	} else if(strcmp(line,"<pre>/*Example 2*/") ==0) {
	    excounter = 2;	
	}
	if(exno != excounter) continue;
	/* Does this line start with 2 digits ? */
        hasline = (strchr(digits,line[0]) && strchr(digits,line[1]) ? 1 : 0);
	if(hasline) {
	    printf("%s\n",&line[2]); /* without the leading line number */
	} /* else ignore this line */
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
