#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void do_cat_conv(FILE *f);

int 
main(int argc, char *argv[]) {
  int i;
  FILE *f;

  if (argc < 2) {
    fprintf(stderr, "file name not given\n");
    exit(0);
  }

  for (i = 1; i < argc; i++) {
    f = fopen(argv[i], "r");
    if (!f) {
      perror(argv[i]);
      exit(1);
    }
    do_cat_conv(f);
    if (fclose(f) != 0) exit(1);
  }
  exit(0);
}

static void
do_cat_conv(FILE *f)
{
  int c;
  while((c = fgetc(f)) != EOF) {
    switch(c) {
      case '\t':
        if (fputs("\\t", stdout) == EOF) exit(1);
        break;
      case '\n':
        if (fputs("$\n", stdout) == EOF) exit(1);
        break;
      default:
        if (putchar(c) < 0) exit(1);
        break;
    }
  }
}
