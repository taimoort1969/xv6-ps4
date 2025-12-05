#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define MAXARG 32

char*
fmtname(char *path) {
  static char buf[DIRSIZ+1];
  char *p;

  for(p = path + strlen(path); p >= path && *p != '/'; p--);
  p++;

  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  buf[strlen(p)] = 0;
  return buf;
}

void
find(char *path, char *target, int exec_mode, char **cmd) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch (st.type) {
    case T_FILE:
      if (strcmp(fmtname(path), target) == 0) {
        if (exec_mode) {
          if (fork() == 0) {
            char *argv[MAXARG];
            int i = 0;
            while (cmd[i] != 0) {
              argv[i] = cmd[i];
              i++;
            }
            argv[i++] = path;
            argv[i] = 0;
            exec(argv[0], argv);
            fprintf(2, "exec %s failed\n", argv[0]);
            exit(1);
          } else {
            wait(0);
          }
        } else {
          printf("%s\n", path);
        }
      }
      break;

    case T_DIR:
      if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
        printf("find: path too long\n");
        break;
      }
      strcpy(buf, path);
      p = buf + strlen(buf);
      *p++ = '/';

      while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0)
          continue;
        if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
          continue;

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        find(buf, target, exec_mode, cmd);
      }
      break;
  }
  close(fd);
}

int
main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(2, "Usage: find path target [-exec command args]\n");
    exit(1);
  }

  char *path = argv[1];
  char *target = argv[2];
  char *cmd[MAXARG];
  int exec_mode = 0;

  if (argc > 3 && strcmp(argv[3], "-exec") == 0) {
    exec_mode = 1;
    int i;
    for (i = 0; i < argc - 4; i++) {
      cmd[i] = argv[i + 4];
    }
    cmd[i] = 0;
  }

  find(path, target, exec_mode, cmd);
  exit(0);
}
