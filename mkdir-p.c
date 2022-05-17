#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

static void do_mkdir_p(char *path);

int
main(int argc, char *argv[])
{
  int i;

  if (argc < 2) {
    fprintf(stderr, "%s: no argumens\n", argv[0]);
    exit(1);
  }
  for (i = 1; i < argc; i++) {
    do_mkdir_p(argv[i]);
  }
}

void
do_mkdir_p(char *path)
{
  char *p;
  char delim[] = "/";
  char *token = strtok(path, delim);

  p = malloc(strlen(path));
  if (path[0] == '/') {
    p[0] = '/';
  }
  while (token != NULL) {
    sprintf(p, "%s%s", p, token);
    if (mkdir(p, 0777) < 0) {
      if (errno == EEXIST) {
        struct stat st;
        if (lstat(p, &st) < 0) {
          perror(p);
          exit(1);
        }
        if (!S_ISDIR(st.st_mode)) {
          fprintf(stderr, "%s already exists and is not directory\n", p);
          exit(1);
        }
      }
    }
    sprintf(p, "%s%s", p, delim);
    token = strtok(NULL, delim);
  }
  free(p);
}
