#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void)
{
  printf("initial syscall count: %d\n", getsyscallcount());
  int pid = getpid();
  printf("my pid is %d\n", pid);
  getpid();
  printf("after doing some work\n");
  printf("final syscall count: %d\n", getsyscallcount());

  exit(0);
}
