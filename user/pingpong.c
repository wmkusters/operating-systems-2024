#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  char* send_byte[1];
  int p2c_p[2], c2p_p[2];

  pipe(p2c_p);
  pipe(c2p_p);
  send_byte[0] = "B";


  if (fork() == 0) {
    // Child
    char* rx_buf[1];
    close(p2c_p[1]);
    close(c2p_p[0]);

    if (read(p2c_p[0], rx_buf, 1) > 0) {
      fprintf(1, "%d: received ping\n", getpid());
    }

    write(c2p_p[1], rx_buf, 1);
    exit(0);
  }
  else {
    // Parent
    close(p2c_p[0]);
    close(c2p_p[1]);
    char* rx_buf[1];

    write(p2c_p[1], send_byte, 1);
    wait((int *) 0);
    if (read(c2p_p[0], rx_buf, 1) > 0) {
      fprintf(1, "%d: received pong\n", getpid());
    }
  }
  exit(0);
}