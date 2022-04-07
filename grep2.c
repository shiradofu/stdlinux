#include <sys/types.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

static void do_regcomp(regex_t *pat, char *src);
static void do_grep(regex_t *pat, FILE *src);

static struct option longopts[] = {
  {"ignore-case",  no_argument, NULL, 'i'},
  {"invert-match", no_argument, NULL, 'v'},
  {0,0,0,0}
};

static int i_flag = 0;
static int v_flag = 0;

int
main(int argc, char *argv[])
{
  int opt;
  regex_t pat;
  int i;

  while ((opt = getopt_long(argc, argv, "iv", longopts, NULL)) != -1) {
    switch(opt) {
      case 'i':
        i_flag = 1;
        break;
      case 'v':
        v_flag = 1;
        break;
      case '?':
        fprintf(stderr, "Usage: %s [-i] [-v] pattern [FILE ...]\n", argv[0]);
        exit(1);
    }
  }

  if (argc == optind) {
    fputs("no pattern\n", stderr);
    exit(1);
  }
  
  do_regcomp(&pat, argv[optind]);
  optind++;

  if (argc == optind) {
    do_grep(&pat, stdin);
  } else {
    for (i = optind; i < argc; i++) {
      FILE *f;

      f = fopen(argv[i], "r");
      if (!f) {
        perror(argv[i]);
        exit(1);
      }
      do_grep(&pat, f);
      fclose(f);
    }
  }
  regfree(&pat);
  exit(0);
}

static void
do_regcomp(regex_t *pat, char *src)
{
  int err;

  int re_mode = REG_EXTENDED | REG_NOSUB | REG_NEWLINE;
  if (i_flag == 1) {
    re_mode |= REG_ICASE;
  }
  err = regcomp(pat, src, re_mode);

  if (err != 0) {
    char buf[1024];

    regerror(err, pat, buf, sizeof buf);
    puts(buf);
    exit(1);
  }
}

static void
do_grep(regex_t *pat, FILE *src)
{
  char buf[4096];

  while (fgets(buf, sizeof buf, src)) {
    if (!!regexec(pat, buf, 0, NULL, 0) == v_flag) {
      fputs(buf, stdout);
    }
  }
}
