#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <errno.h>
#include <sys/wait.h>

#define BSIZE 256

#define BASH_EXEC  "/bin/bash"
#define FIND_EXEC  "/usr/bin/find"
#define XARGS_EXEC "/usr/bin/xargs"
#define GREP_EXEC  "/bin/grep"
#define SORT_EXEC  "/usr/bin/sort"
#define HEAD_EXEC  "/usr/bin/head"

int main(int argc, char *argv[])
{
  char buf[BSIZE];
  int status, rsize;
  pid_t pid_find, pid_grep, pid_sort, pid_head;

  if (argc != 4) {
    printf("usage: finder DIR STR NUM_FILES\n");
    exit(0);
  }

  // make 4 pipes (find to grep, grep to sort, sort to head); each has 2 fds

  int find2grep[2];
  int grep2sort[2];
  int sort2head[2];
  int head2parent[2];

  pipe(find2grep); // sets up pipe between find and grep
  pipe(grep2sort); // sets up pipe between grep and sort
  pipe(sort2head); // sets up pipe between sort and head
  pipe(head2parent); // sets up pipe between head and parent

  pid_find = fork();
  if (pid_find == 0) 
  {
    /* First Child */
    char cmdbuf[BSIZE];
    bzero(cmdbuf, BSIZE);

    dup2(find2grep[1], 1);
    
    sprintf(cmdbuf, "%s %s -name \'*\'.[ch]", FIND_EXEC, argv[1]);

    /*close all pipes*/
    close(find2grep[0]);
    close(find2grep[1]);
    close(grep2sort[0]);
    close(grep2sort[1]);
    close(sort2head[0]);
    close(sort2head[1]);
    close(head2parent[0]);
    close(head2parent[1]);

    if( execl( BASH_EXEC, BASH_EXEC, "-c", cmdbuf, (char *) 0 ) < 0 )
    {
      fprintf(stderr, "FIND encountered an error. ERROR%d", errno);
      return EXIT_FAILURE;
    }
    exit(0);
  }

  pid_grep = fork();
  if (pid_grep == 0) 
  {
    /* Second Child */
    char cmdbuf[BSIZE];
    bzero(cmdbuf, BSIZE);

    dup2(find2grep[0], 0);
    dup2(grep2sort[1], 1);
    
    sprintf(cmdbuf, "%s %s -c %s", XARGS_EXEC, GREP_EXEC, argv[2]);

    /*close all pipes*/
    close(find2grep[0]);
    close(find2grep[1]);
    close(grep2sort[0]);
    close(grep2sort[1]);
    close(sort2head[0]);
    close(sort2head[1]);
    close(head2parent[0]);
    close(head2parent[1]);

    if( execl( BASH_EXEC, BASH_EXEC, "-c", cmdbuf, (char *) 0 ) < 0 )
    {
      fprintf(stderr, "GREP encountered an error. ERROR%d", errno);
      return EXIT_FAILURE;
    }
    exit(0);
  }

  pid_sort = fork();
  if (pid_sort == 0) {
    /* Third Child */
    char cmdbuf[BSIZE];
    bzero(cmdbuf, BSIZE);

    dup2(grep2sort[0], 0);
    dup2(sort2head[1], 1);
    
    //sort -t : +1.0 -2.0 --numeric --reverse
    sprintf(cmdbuf, "%s -t : +1.0 -2.0 --numeric --reverse", SORT_EXEC);

    /*close all pipes*/
    close(find2grep[0]);
    close(find2grep[1]);
    close(grep2sort[0]);
    close(grep2sort[1]);
    close(sort2head[0]);
    close(sort2head[1]);
    close(head2parent[0]);
    close(head2parent[1]);

    if( execl( BASH_EXEC, BASH_EXEC, "-c", cmdbuf, (char *) 0 ) < 0 )
    {
      fprintf(stderr, "GREP encountered an error. ERROR%d", errno);
      return EXIT_FAILURE;
    }
    exit(0);
  }

  pid_head = fork();
  if (pid_head == 0) {
    /* Fourth Child */
    char cmdbuf[BSIZE];
    bzero(cmdbuf, BSIZE);

    dup2(sort2head[0], 0);
    dup2(head2parent[1], 1);
    
    //head --lines=$3
    sprintf(cmdbuf, "%s --lines=%s", HEAD_EXEC, argv[3]);

    /*close all pipes*/
    close(find2grep[0]);
    close(find2grep[1]);
    close(grep2sort[0]);
    close(grep2sort[1]);
    close(sort2head[0]);
    close(sort2head[1]);
    close(head2parent[0]);
    close(head2parent[1]);

    if( execl( BASH_EXEC, BASH_EXEC, "-c", cmdbuf, (char *) 0 ) < 0 )
    {
      fprintf(stderr, "GREP encountered an error. ERROR%d", errno);
      return EXIT_FAILURE;
    }
    exit(0);
  }

  /*close all pipes*/
  close(find2grep[0]);
  close(find2grep[1]);
  close(grep2sort[0]);
  close(grep2sort[1]);
  close(sort2head[0]);
  close(sort2head[1]);
  close(head2parent[1]);

  /* read contents of file and write it out to a pipe */
  while ((rsize = read(head2parent[0], buf, BSIZE)) > 0) {
    /* XXX - this should write to a pipe - not to stdout */
    write(STDOUT_FILENO, buf, rsize);
  }

  close(head2parent[0]);

  if ((waitpid(pid_find, &status, 0)) == -1) {
    fprintf(stderr, "Process 1 encountered an error. ERROR%d", errno);
    return EXIT_FAILURE;
  }
  if ((waitpid(pid_grep, &status, 0)) == -1) {
    fprintf(stderr, "Process 2 encountered an error. ERROR%d", errno);
    return EXIT_FAILURE;
  }
  if ((waitpid(pid_sort, &status, 0)) == -1) {
    fprintf(stderr, "Process 3 encountered an error. ERROR%d", errno);
    return EXIT_FAILURE;
  }
  if ((waitpid(pid_head, &status, 0)) == -1) {
    fprintf(stderr, "Process 4 encountered an error. ERROR%d", errno);
    return EXIT_FAILURE;
  }

  return 0;
}
