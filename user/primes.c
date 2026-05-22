#include "kernel/types.h"
#include "user/user.h"

void child_proc(int *left, int depth) {
  // have to limit depth to get GCC not to complain
  if (depth > 50) {
    return;
  }

  int right_pipe[2];
  int p, n;

  close(left[1]);

  pipe(right_pipe);

  if (read(left[0], &p, sizeof(int)) == 0) {
    exit(0);
  } else if (fork() != 0) {
    printf("prime %d\n", p);

    while (read(left[0], &n, sizeof(int)) > 0) {
      if (n % p != 0) {
        write(right_pipe[1], &n, sizeof(int));
      }
    }

    close(right_pipe[1]);
    wait((int *)0);
    exit(0);
  }
  child_proc(right_pipe, depth + 1);
  exit(0);
}

int main(void) {
  int leftmost_pipe[2];

  pipe(leftmost_pipe);

  if (fork() != 0) {         // parent process
    close(leftmost_pipe[0]); // close read

    for (int i = 2; i < 36; i++) {
      write(leftmost_pipe[1], &i, sizeof(int));
    }
    close(leftmost_pipe[1]); // close write

    wait((int *)0);
    exit(0);
  } else {
    child_proc(leftmost_pipe, 0);
  }
  exit(0);
}
