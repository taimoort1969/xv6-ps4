#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  printf("CPU-bound process starting...\n");
  // Busy loop forever
  for(;;){
    // do some useless work
    volatile int x = 0;
    for(int i = 0; i < 1000000; i++){
      x += i;
    }
  }
  exit(0);
}
