#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

static void do_cat_conv(FILE *f);

int n = 0;
int t = 0;

int 
main(int argc, char *argv[]) {
  int opt;
  int i;
  FILE *f;

  while ((opt = getopt(argc, argv, "tn")) != -1) {
    switch(opt) {
      case 't':
        t = 1;
        break;
      case 'n': 
        n = 1;
        break;
      case '?':
        fprintf(stderr, "Usage: %s [-t] [-n] FILE [FILE ...]\n", argv[0]);
        exit(1);
    }
  }

  if (argc == optind) {
    fprintf(stderr, "file name not given\n");
    exit(0);
  }

  for (i = optind; i < argc; i++) {
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
        if (fputs(t ? "\\t" : "\t", stdout) == EOF) exit(1);
        break;
      case '\n':
        if (fputs(n ? "$\n" : "\n", stdout) == EOF) exit(1);
        break;
      default:
        if (putchar(c) < 0) exit(1);
        break;
    }
  }
}
