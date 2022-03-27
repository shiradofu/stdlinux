#include <stdio.h>
#include <stdlib.h>

static void do_cat(const char *path);
static void die(const char *s);

int main(int argc, char *argv[]) {
  int i;

  if (argc == 1) {
    fprintf(stderr, "%s: file name not given\n", argv[0]);
    exit(1);
  }

  for (i = 1; i < argc; i++) {
    do_cat(argv[i]);
  }

  exit(0);
}

#define BUFFER_SIZE 2048

static void do_cat(const char *path) {
  FILE *f;
  unsigned char buf[BUFFER_SIZE];

  f = fopen(path, "r");
  for (;;) {
    size_t n = fread(buf, 1, sizeof buf, f);
    if (ferror(f)) die(path);
    if (fwrite(buf, 1, n, stdout) < n) die(path);
    if (n < sizeof buf) break;
  }
  if (fclose(f) != 0) die(path);
}

static void die(const char *s) {
  perror(s);
  exit(1);
}
