#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 2048

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
    fgets(buf, sizeof buf, stdin);

    /* fgets で取得した文字列の改行を除去 */
    char *last = buf + strlen(buf) - 1;
    *last = '\0';

    /* 入力をパース */
    int i;
     /* 引数は 10個まで */
    char *args[10] = { NULL };
    char *next;
    args[0] = buf;
    for (i = 1; i < 9; i++) {
      /*
       * スペースをNULL文字で塗りつぶし、
       * 最後に塗りつぶしたアドレスの次の
       * アドレスを引数リストに加える
       */
      next = strrchr(buf, ' ');
      if (next == NULL) {
        break;
      }
      while (*next == ' ') {
        *next++ = '\0';
      }
      if (*next == '\0') {
        break;
      }
      args[i] = next;
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
