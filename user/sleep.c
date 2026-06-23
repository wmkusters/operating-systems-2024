#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int sleeptime;
  sleeptime = atoi(argv[1]);
  sleep(sleeptime);
  exit(0);
}
