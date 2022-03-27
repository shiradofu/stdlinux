#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

static void do_tail(FILE *f, long nlines);
static unsigned long get_start_byteno(unsigned long *newline_byteno, long nlines, char prev);
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
  unsigned long newline_byteno[nlines + 1];
  int i;
  int c;
  char prev = '\n';

  if (nlines <= 0) return;

  for (i = 0; i < nlines + 1; i++) newline_byteno[i] = 0;

  while((c = getc(f)) != EOF) {
    if (c == '\n') {
      cnt++;
      newline_byteno[cnt % (nlines + 1)] = ftello(f);
    }
    prev = c;
  }
  
  fseeko(f, get_start_byteno(newline_byteno, nlines, prev), SEEK_SET);
  while((c = getc(f)) != EOF) {
    if (putchar(c) < 0) exit(1);
  }
}

static unsigned long
get_start_byteno(unsigned long *newline_byteno, long nlines, char prev)
{
  long i;

  unsigned long max_idx;

  for (i = 0; i < nlines; i++) {
    if (newline_byteno[i] < newline_byteno[i + 1]) {
      max_idx = i + 1;
    } else {
      max_idx = i;
      break;
    }
  }
  if (prev == '\n') {
    return newline_byteno[(max_idx + 1) % (nlines + 1)];
  } else {
    return newline_byteno[(max_idx + 2) % (nlines + 1)];
  }
}

static void
help_msg(char *cmd, int code)
{
  fprintf(code == 0 ? stdout : stderr, "Usage: %s [-n LINES] [FILE ...]\n", cmd);
  exit(code);
}
