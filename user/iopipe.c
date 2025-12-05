#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
delay(long ticks)
{
  for (long t = 0; t < ticks; t++) {
    for (volatile int i = 0; i < 1000000; i++) {
      // busy wait
    }
  }
}

int
main(void)
{
  int p[2];
  char buf[1];

  if (pipe(p) < 0) {
    printf("iopipe: pipe failed\n");
    exit(1);
  }

  int pid = fork();
  if (pid < 0) {
    printf("iopipe: fork failed\n");
    exit(1);
  }

  if (pid == 0) {
    // Child: mostly I/O-bound, blocking on read()
    close(p[1]); // close write end
    printf("iopipe child: starting, mostly waiting on pipe...\n");
    while (1) {
      int n = read(p[0], buf, 1);   // will block when no data
      if (n <= 0) {
        // if writer ever closes, just exit
        printf("iopipe child: pipe closed, exiting\n");
        break;
      }
      printf("iopipe child: got a byte\n");
    }
    exit(0);
  } else {
    // Parent: occasional writer
    close(p[0]); // close read end
    printf("iopipe parent: writing occasionally...\n");
    while (1) {
      // Long delay between writes → child sleeps a lot
      delay(5);
      write(p[1], "x", 1);
    }
  }
}
