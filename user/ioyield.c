#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void delay(int ticks)
{
  for(int t = 0; t < ticks; t++){
    for(volatile int i = 0; i < 1000000; i++) { }
  }
}

int
main(void)
{
  printf("ioyield starting...\n");
  while(1){
    delay(10); // small delay
    printf("ioyield pulse.\n"); // print slowly
  }
  exit(0);
}
