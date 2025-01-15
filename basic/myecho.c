#include <stdio.h>              /* fprintf */
#include <stdlib.h>             /* exit */
#include <unistd.h>             /* STDIN_FILENO, STDOUT_FILENO */
#include <fcntl.h>              /* O_RDONLY */
#include "wrapper/w_unixio.h"   /* Read, Write, Close */

#define MAXBUF 1024

int main (int argc, char **argv) {
  ssize_t nread;
  char buf[MAXBUF];

  if (argc != 1) {
    fprintf (stderr, "usage: %s\n", argv[0]);
    exit (0);
  }

  nread = Read (STDIN_FILENO, buf, MAXBUF);
  Write (STDOUT_FILENO, buf, nread);
}
