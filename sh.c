#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 2048
#define MAX_ARG 10

int
main(int argc, char *argv[])
{
  char buf[BUFFER_SIZE];

  if (argc > 1) {
    fprintf(stderr, "%s: argument is not accepted\n", argv[0]);
    exit(1);
  }
  for (;;) {
    fprintf(stdout, "> ");
    fflush(stdout);
    if (fgets(buf, sizeof buf, stdin) == NULL) {
      exit(0);
    }

    /* 末尾の空白文字を除去 */
    char *last = buf + strlen(buf) - 1;
    while (isspace(*last)) {
        *last-- = '\0';
    }
    /* 先頭の空白文字を除去 */
    char *start = buf;
    while (isspace(*start)) {
      start++;
    }

    int i;
    char *args[MAX_ARG + 1] = { NULL }; /* 末尾のNULLのぶん1つ多めに*/
    args[0] = start;
    char *next = start + 1;
    for (i = 1; i < MAX_ARG; i++) {
      /*
       * スペースをNULL文字で塗りつぶし、
       * 最後に塗りつぶしたアドレスの次の
       * アドレスを引数リストに加える
       */
      while (!isspace(*next) && *next != '\0') {
        next++;
      }
      while (isspace(*next) && *next != '\0') {
        *next++ = '\0';
      }
      if (*next == '\0') {
        break;
      }
      args[i] = next;
    }

    if (i == MAX_ARG) {
      while (!isspace(*next) && *next != '\0') {
        next++;
      }
      while (isspace(*next) && *next != '\0') {
        *next++ = '\0';
      }
      if (*next != '\0') {
        fprintf(stderr, "Arguments list too long\n");
        continue;
      }
    }

    if (args[0][0] == '\0') {
      continue;
    }

    pid_t pid = fork();
    if (pid < 0) {
      fprintf(stderr, "fork(2) failed\n");
      continue;
    }
    if (pid == 0) {
      execvp(args[0], args);
      perror(buf);
      exit(99);
    } else {
      int status;

      waitpid(pid, &status, 0);
    }
  }
}
