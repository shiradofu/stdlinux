#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

static void do_tail(FILE *f, long nlines);
static int asc(const void *a, const void *b);
static void help_msg(char *cmd, int code);

#define DEFAULT_N_LINES 10
#define SIZEOF_ARR(array) (sizeof(array)/sizeof(array[0]))

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
  
  qsort(newline_byteno, SIZEOF_ARR(newline_byteno), sizeof(unsigned long), asc);
  fseeko(f, prev == '\n' ? newline_byteno[0] : newline_byteno[1], SEEK_SET);
  while((c = getc(f)) != EOF) {
    if (putchar(c) < 0) exit(1);
  }
}

static int
asc(const void *a, const void *b)
{
  int na = *(int*)a;
  int nb = *(int*)b;
  return na - nb;
}

static void
help_msg(char *cmd, int code)
{
  fprintf(code == 0 ? stdout : stderr, "Usage: %s [-n LINES] [FILE ...]\n", cmd);
  exit(code);
}
