#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define RANK 3

/*
sizes=[2][5][3]
x-product = 2*5*3 = 30

i = x
i = i * 5 + y
i = i * 3 + z

*/

static size_t
linearize(int rank, size_t* dimsizes, size_t* indices)
{
    int i;
    size_t count = 0;
    for(i=0;i<rank;i++) {
        count *= dimsizes[i];
        count += indices[i];
    }
    return count;
}

/*
sizes = [2][5][3]

i = ((x*5)+y)*3+z

z = i % 3
i = (i - z)/3
y = i % 5
i = (i - y)/5
x = i

29
z = i%3 = 2
i = (i-2)/3 = 9
y = i%5 = 4
i = (i-4)/5 = 1
x = i = 1

30
z = i%3 = 0
i = (i-0)/3 = 10
y = i%5 = 0
i = (i-0)/5 = 2
x = i = 2

*/

static void
delinearize(size_t index, int rank, size_t* sizes, size_t* indices)
{
    int i;
    for(i=rank-1;i>0;i--) {
	indices[i] = index % sizes[i];
	index = (index - indices[i]) / sizes[i];
    }
    indices[0] = index;
}

static size_t
edgeoffset(int rank, size_t* dimsizes, size_t* edges)
{
    int i;
    size_t count = 0;
    for(i=0;i<rank;i++) {
        count *= dimsizes[i];
        count += (edges[i]-1);
    }
    return count+1;
}


static int
more(size_t rank, size_t* sizes, size_t* indices)
{
  return (indices[0] < sizes[0] ? 1 : 0);
}

static void
next(size_t rank, size_t* sizes, size_t* indices) 
{ 
  int i; 
  for(i=rank-1;i>=0;i--) { 
      indices[i]++; 
      if(indices[i] < sizes[i]) break; 
      if(i > 0) indices[i] = 0;
  }
} 

static void
dump(char* msg, int rank, size_t* indices)
{
    int j;
    printf("%s = ",msg);
    for(j=0;j<RANK;j++)
	printf("[%u]",indices[j]);
    printf("\n");
}

int
main()
{
    int k;
    size_t i,j;
    size_t sizes[RANK]={3,2,2};
    size_t indices[RANK];
    size_t xp;

    /* Test linearize */
    printf("test linearize:\n");
    dump("sizes",RANK,sizes);
    memset(indices,0,sizeof(indices));
    while(more(RANK,sizes,indices)) {
	char tmp[64];
	size_t linear = linearize(RANK,sizes,indices);
	sprintf(tmp,"%02u",linear);
	dump(tmp,RANK,indices);
        next(RANK,sizes,indices);
    }

    /* Test delinearize */
    printf("\ntest de-linearize:\n");
    for(xp=1,i=0;i<RANK;i++) xp *= sizes[i];

    printf("x-product = %u\n",xp);
    for(k=(int)xp-1;k>=0;k--) {
	char tmp[64];
	size_t test;
	size_t linear = (size_t)k;
	delinearize(linear,RANK,sizes,indices);
	sprintf(tmp,"%02u",linear);
	dump(tmp,RANK,indices);
	/* Validate */
	test = linearize(RANK,sizes,indices);
	if(test != linear)
	    printf("failure: %d\n",k);
    }
    
    printf("\ntest endpoint:\n");
    {
	size_t count;
	size_t edges[RANK];
	size_t size1[RANK];

	count = edgeoffset(RANK,sizes,sizes);

	printf("|edge count| = %lu\n",(unsigned long)count);

	goto done;

	count = linearize(RANK,sizes,sizes);
	printf("|sizes| = %lu\n",(unsigned long)count);
	memset(edges,0,sizeof(edges));	
	delinearize(count,RANK,sizes,edges);
	dump("invert",RANK,edges);


        for(j=0;j<RANK;j++) edges[j]=sizes[j];
        for(j=0;j<RANK;j++) size1[j]=sizes[j]-1;
	size1[RANK-1]++;
    }	
done:
    exit(0);
}
