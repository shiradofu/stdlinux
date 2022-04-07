#include <stdio.h>
#include <regex.h>
#include <sys/types.h>
#include <stdlib.h>

static void do_slice(regex_t *pat, FILE *f);

int
main(int argc, char *argv[])
{
  int re_err;
  regex_t pat;

  if (argc < 2) {
    fputs("no pattern\n", stderr);
    exit(1);
  }

  re_err = regcomp(&pat, argv[1], REG_EXTENDED | REG_NEWLINE);
  if (re_err) {
    char buf[1024];

    regerror(re_err, &pat, argv[1], sizeof buf);
    puts(buf);
    exit(1);
  }

  if (argc == 2) {
    do_slice(&pat, stdin);
  } else {
    int i;
    for (i = 2; i < argc; i++) {
      FILE *f;
      f = fopen(argv[i], "r");
      if (!f) {
        perror(argv[i]);
        exit(1);
      }
      do_slice(&pat, f);
      fclose(f);
    }
  }

  regfree(&pat);
  exit(0);
}

static void
do_slice(regex_t *pat, FILE *f)
{
  char buf[4096];

  while (fgets(buf, sizeof buf, f)) {
    size_t nmatch = 1;
    regmatch_t matches[nmatch];
    if (regexec(pat, buf, nmatch, matches, 0) == 0) {
      int i;
      for (i = matches[0].rm_so; i < matches[0].rm_eo; i++) {
        putchar(buf[i]);
      }
      putchar('\n');
    }
  }
}
