#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
int main()
{ 
  extern const char * const sys_errlist[];
  extern int sys_nerr;  
  const char *path = "abracodabra.txt";
  int fd = open(path, O_RDONLY);
  if (fd == -1)
  {
    perror("open"); // 1 способ
    fprintf(stderr, "%s\n", strerror(errno)); // 2 способ
    if (errno >= 0 && errno < sys_nerr) {
      fprintf(stderr, "%s\n", sys_errlist[errno]);
    } // 3 способ
  return 1;  
  }
  close(fd);
  return 0;
}
