#include <stdio.h>              /* fprintf */
#include <stdlib.h>             /* exit */
#include <unistd.h>             /* STDIN_FILENO, STDOUT_FILENO */
#include <fcntl.h>              /* O_RDONLY */
#include "wrapper/w_unixio.h"   /* Open, Read, Write, Close */

#define MAXBUF 1024

int main (int argc, char **argv) {
  ssize_t nread;
  char buf[MAXBUF];
  int fd;

  if (argc != 2) {
    fprintf (stderr, "usage: %s <filename>\n", argv[0]);
    exit (0);
  }

  fd = Open (argv[1], O_RDONLY, 0);
  nread = Read (fd, buf, MAXBUF);
  Write (STDOUT_FILENO, buf, nread);
  Close (fd);
}
