#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

static void do_count(const char *path);
static void die(const char *s);

int
main(int argc, char *argv[])
{
  if (argc == 1) {
    fprintf(stderr, "%s: file name not given\n", argv[0]);
    exit(1);
  }
  if (argc > 2) {
    fprintf(stderr, "%s: too many arguments\n", argv[0]);
    exit(1);
  }

  do_count(argv[1]);
  exit(0);
}

#define BUFFER_SIZE 2048

static void
do_count(const char *path)
{
  unsigned char buf[BUFFER_SIZE];
  int fd;
  int n;
  int cnt = 0;

  fd = open(path, O_RDONLY);
  if (fd < 0) die(path);
  for (;;) {
    n = read(fd, buf, sizeof buf);
    if (n < 0) die(path);
    if (n == 0) break;
    unsigned long i;
    for (i = 0; i < n; i++) {
      if (buf[i] == '\n') cnt++;
    }
  }
  fprintf(stdout, "%d\n", cnt);
  if (close(fd) < 0) die(path);
}

static void
die(const char *s)
{
  perror(s);
  exit(1);
}
