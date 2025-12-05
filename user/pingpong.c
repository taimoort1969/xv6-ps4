#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void){
  int p2c[2];
  int c2p[2];

  if (pipe(p2c) < 0 || pipe(c2p) < 0) {
   fprintf(2,"pipe failed\n");
    exit(1);}

  int pid = fork();
  if (pid < 0) {
   fprintf(2,"fork failed\n");
   exit(1);
  }

  if (pid == 0) {
   close(p2c[1]);
    close(c2p[0]);

    char buf = 'c';
    for (int i = 0; i < 10; i++) {
      if (read(p2c[0], &buf, 1) != 1) {
        fprintf(2,"read failed\n");
        exit(1);
      }
      printf("received ping %d\n", i+1);

      if (write(c2p[1], &buf, 1) != 1) {
        fprintf(2, "write failed\n");
        exit(1);
      }
    }
    close(p2c[0]);
    close(c2p[1]);
    exit(0);

  } else {
    close(p2c[0]);
    close(c2p[1]);

    char buf = 'P';
    for (int i = 0; i < 10; i++) {
      if (write(p2c[1], &buf, 1) != 1) {
        fprintf(2, "write failed\n");
        exit(1);
      }
      printf("sent ping %d\n", i+1);
      if (read(c2p[0], &buf, 1) != 1) {
        fprintf(2, "read failed\n");
        exit(1);
      }
      printf("received pong %d\n", i+1);
    }
    close(p2c[1]);
    close(c2p[0]);
    wait(0);
    exit(0);
  }
}
