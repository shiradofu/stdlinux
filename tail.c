/* head with getopt */
#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <getopt.h>

static void do_tail(FILE *f, long nlines);
static void help_msg(char *cmd, int code);

#define DEFAULT_N_LINES 10

static struct option longopts[] = {
  {"lines", required_argument, NULL, 'n'},
  {"help",  no_argument,       NULL, 'h'},
  {0, 0, 0, 0}
};

int
main(int argc, char *argv[])
{
  int opt;
  long nlines = DEFAULT_N_LINES;
  int i;

  while ((opt = getopt_long(argc, argv, "hn:", longopts, NULL)) != -1) {
    switch(opt) {
    case 'n':
      nlines = atol(optarg);
      break;
    case 'h':
      help_msg(argv[0], 0);
    case '?':
      help_msg(argv[0], 1);
    }
  }

  /* fseeko を使用しているため stdin からの入力では動作しない */
  if (argc == optind) help_msg(argv[0], 1);

  for (i = optind; i < argc; i++) {
    FILE *f;
    f = fopen(argv[i], "r");
    if (!f) {
      perror(argv[i]);
      exit(1);
    }
    do_tail(f, nlines);
    fclose(f);
  }
  exit(0);
}

static void
do_tail(FILE *f, long nlines)
{
  long cnt = 0;
  unsigned long newline_byteno[nlines];
  int c;

  if (nlines <= 0) return;

  while((c = getc(f)) != EOF) {
    if (c == '\n') {
      cnt++;
      if (nlines == 1) {
        newline_byteno[0] = ftello(f);
      } else if (cnt <= nlines) {
        newline_byteno[cnt - 1] = ftello(f);
      } else {
        int i;
        for (i = 0; i < nlines - 1; i++) {
          newline_byteno[i] = newline_byteno[i + 1];
        }
        newline_byteno[nlines - 1] = ftello(f);
      }
    }
  }
  
  fseeko(f, newline_byteno[0] + 1, SEEK_SET);

  while((c = getc(f)) != EOF) {
    if (putchar(c) < 0) exit(1);
  }
}

static void
help_msg(char *cmd, int code)
{
  fprintf(code == 0 ? stdout : stderr, "Usage: %s [-n LINES] [FILE ...]\n", cmd);
  exit(code);
}
