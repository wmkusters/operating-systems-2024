#include "kernel/types.h"

#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

void find(char *path, char *name, int depth) {
  char buf[512], *p;

  int fd;
  struct dirent de;
  struct stat st;

  struct stat current_file;

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
    printf("find: path too long\n");
    close(fd);
    return;
  }

  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';

  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    char joined[512];
    // current_fd = open(path, 0);
    stat(de.name, &current_file);
    if (de.inum == 0)
      continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if (stat(buf, &st) < 0) {
      printf("find: cannot stat %s\n", buf);
      continue;
    }
    if (strcmp(de.name, name) == 0) {
      printf("%s/%s\n", path, de.name);
      continue;
    }
    // if the file we're considering is not . or .. and is a dir
    if ((current_file.type == T_DIR) && (strcmp(de.name, ".") != 0) &&
        (strcmp(de.name, "..") != 0)) {
      // construct the path
      memmove(path + strlen(path), "/", 1);
      // if (path[strlen(path) - 1] != atoi("/")) {
      //  memmove(path + strlen(path), "/", 1);
      //}
      memmove(joined, path, strlen(path));
      memmove(joined + strlen(path), de.name, DIRSIZ);
      struct stat should_recurse;
      stat(joined, &should_recurse);
      if (should_recurse.type != T_DIR) {
        continue;
      }
      find(joined, name, depth + 1);
      memmove(path + strlen(path) - 1, "\0", 1);
      continue;
    }
  }
  close(fd);
}

int main(int argc, char *argv[]) {
  char *dir, *name;
  dir = argv[1];
  name = argv[2];
  find(dir, name, 0);

  exit(0);
}
