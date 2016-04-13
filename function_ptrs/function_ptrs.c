#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM 7

typedef struct process_t
{
  int pid;
  int arrival_time;
  int priority;
} process_t;

int arrival_time_compare(const void * a, const void * b)
{
    const process_t * process1 = a;
    const process_t * process2 = b;
    return ( process1->arrival_time - process2->arrival_time );
}

int priority_compare(const void * a, const void * b)
{
    const process_t * process1 = a;
    const process_t * process2 = b;
    if ( process1->priority == process2->priority )
        return ( process1->arrival_time - process2->arrival_time );
    else
        return ( process2->priority - process1->priority );
}

void print( process_t process[NUM] )
{
    int i = 0;
    while( i < NUM )
    {
        printf("pid: %2d   arrival_time: %2d   priority: %2d\n", process[i].pid, process[i].arrival_time, process[i].priority );
        ++i;
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    char buffer[1024] ;
    char *line;
    struct process_t process[NUM];
    int i=0;

    FILE *fstream = fopen("process.txt","r");

    if(fstream == NULL)
    {
        printf("\n file opening failed ");
        return -1 ;
    }

    while((line=fgets(buffer,sizeof(buffer),fstream))!=NULL)
    {
        process[i].pid = atoi( strtok(line,",") );
        process[i].arrival_time = atoi( strtok(NULL,",") );
        process[i].priority = atoi( strtok(NULL,",") );
        ++i ;
    }

    printf("\n");

    printf("ORIGINAL\n\n");
    print( process );

    printf("ARRIVAL TIME SORT\n\n");
    qsort( process, NUM, sizeof(process_t), arrival_time_compare );
    print( process );

    printf("PRIORITY SORT\n\n");
    qsort( process, NUM, sizeof(process_t), priority_compare );
    print( process );

    return 0;
}