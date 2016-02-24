#include <stdio.h>     /* standard I/O functions                         */
#include <stdlib.h>    /* exit                                           */
#include <unistd.h>    /* standard unix functions, like getpid()         */
#include <signal.h>    /* signal name macros, and the signal() prototype */

/* first, define the Ctrl-C counter, initialize it with zero. */
int ctrl_c_count = 0;
int got_response = 0;
#define CTRL_C_THRESHOLD  5 

/* the Ctrl-C signal handler */
void catch_int(int sig_num)
{
  /* increase count, and check if threshold was reached */
  ctrl_c_count++;
  if (ctrl_c_count >= CTRL_C_THRESHOLD) {
    char answer[30];

    /* prompt the user to tell us if to really
     * exit or not */
    printf("\nReally exit? [Y/n]: ");
    alarm(10);
    fflush(stdout);
    
    
    fgets(answer, sizeof(answer), stdin);
    
    if (answer[0] == 'n' || answer[0] == 'N') {
      printf("\nContinuing\n");
      alarm(0);
      fflush(stdout);
      /* 
       * Reset Ctrl-C counter
       */
      ctrl_c_count = 0;
    }
    else {
      printf("\nExiting...\n");
      fflush(stdout);
      exit(0);
    }
  }
}

/* the Ctrl-Z signal handler */
void catch_tstp(int sig_num)
{
  /* print the current Ctrl-C counter */
  printf("\n\nSo far, '%d' Ctrl-C presses were counted\n\n", ctrl_c_count);
  fflush(stdout);
}

/* the SIGALRM signal handler */
void catch_timeout()
{
  printf("\nUser taking too long to respond. Exiting...\n\n");
  fflush(stdout);
  exit(0);
}

int main(int argc, char* argv[])
{
  struct sigaction sa;
  sigset_t mask_set;  /* used to set a signal masking set. */

  /* setup mask_set */

  /* set signal handlers */

  struct sigaction c_act;
  c_act.sa_handler = catch_int;
  sigemptyset(&c_act.sa_mask);

  sigdelset(&mask_set, SIGALRM);

  c_act.sa_flags = 0;
  sigaction(SIGINT, &c_act, 0);

  struct sigaction z_act;
  z_act.sa_handler = catch_tstp;
  sigemptyset(&z_act.sa_mask);
  z_act.sa_flags = 0;
  sigaction(SIGTSTP, &z_act, 0);

  struct sigaction t_act;
  t_act.sa_handler = catch_timeout;
  sigemptyset(&t_act.sa_mask);
  t_act.sa_flags = 0;
  sigaction(SIGALRM, &t_act, 0);

  while(1)
  {
    pause();
  }

  return 0;
}

