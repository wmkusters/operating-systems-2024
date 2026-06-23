#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void
main(int argc, char *argv[]){
  char *argv_cmd[MAXARG];
  int i;
  for(i=1; i < argc; i++){
    argv_cmd[i-1] = argv[i];
  }
  int args_used = argc-1; // didn't use argv[0]
  char buf[1024];
  char *p = buf;
  char *buf_idx;
  buf_idx = buf;
  char *argv_place;
  int arg = 0;

  while(read(0, buf_idx, 1) == 1){
    // printf("buf_idx: %p\n", buf_idx);
    // printf("item at buf_idx: %c\n", *(buf_idx));
    if(*buf_idx == ' ' ){
      // printf("inside\n");
      if(args_used == MAXARG){
        exit(1);
      }
      *buf_idx = '\0';
      argv_place = malloc(strlen(buf));
      // printf("Moving %s into argv\n", p);
      memmove(argv_place, p, strlen(buf));
      argv_cmd[args_used] = argv_place;
      // reset vars
      memset(p, 0, sizeof(buf));
      buf_idx = p;
      args_used++;
      continue;
      // for(i=0; i < args_used; i++){
      //   printf("Argv[%d]: %s\n", i, argv_cmd[i]);
      // }
    }

    if(*buf_idx == *"\n"){
      // printf("outside\n");
      // printf("think moving %s into argv\n", buf);
      if(args_used == MAXARG){
        exit(1);
      }
      *buf_idx = '\0';
      argv_place = malloc(strlen(buf));
      // printf("Moving %s into argv\n", buf);
      memmove(argv_place, buf, strlen(buf));
      argv_cmd[args_used] = argv_place;
      // reset vars
      memmove(buf, "", 1);
      buf_idx = buf;
      args_used++;

      if (fork() != 0){ // parent
        wait((int *) 0);
      } else { // child
        // char* tmp_cmd = "hello from child\n";
        // printf("Tmp cmd: %s\n", tmp_cmd);
        exec(argv_cmd[0], argv_cmd);
        exit(0);
      }
      arg++;
    }
    buf_idx = buf_idx + 1;
  }
  exit(0);
}

