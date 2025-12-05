// user/boosttest.c

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Explicitly declare the delay() function
void delay(int ticks);

int
main(int argc, char *argv[])
{
  printf("Running boosttest...\n");

  int pid1 = fork();
  if(pid1 == 0){
    // CPU-bound child
    for(;;)
      ; // spin
  }

  int pid2 = fork();
  if(pid2 == 0){
    // I/O-bound child
    for(;;){
      printf("IO child running\n");
      delay(10);      // use delay() here
    }
  }

  // parent: let them run and get demoted
  delay(200);        // also delay in parent for the test

  printf("Calling boostproc()...\n");
  boostproc();        // your syscall

  delay(100);
  kill(pid1);
  kill(pid2);
  wait(0);
  wait(0);

  printf("boosttest done\n");
  exit(0);
}

// Your delay function to simulate a small time delay
void delay(int ticks)
{
  for(int t = 0; t < ticks; t++){
    for(volatile int i = 0; i < 1000000; i++) { }  // some busy work
  }
}
