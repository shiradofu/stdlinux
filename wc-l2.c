#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void do_count(FILE *f);

int
main(int argc, char *argv[])
{
  if (argc == 1) {
    do_count(stdin);
  } else {
    int i;
    FILE *f;

    for (i = 1; i < argc; i++){
      f = fopen(argv[i], "r");
      if (!f) {
        perror(argv[i]);
        exit(1);
      }
      do_count(f);
    }
  }

  exit(0);
}

static void
do_count(FILE *f)
{
  int c;
  unsigned long cnt = 0;
  char prev = '\n';

  while ((c = getc(f)) != EOF) {
    if (c == '\n') cnt++;
    prev = c;
  }
  if (prev != '\n') cnt++;

  printf("%lu", cnt);
}
