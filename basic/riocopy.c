#include <stdio.h>              /* macro stderr,
                                   function fprintf, printf, close */
#include <stdlib.h>             /* function exit */
#include <string.h>             /* function strerror */
#include <unistd.h>             /* macro STDIN_FILENO, STDOUT_FILENO, 
                                   function close */
#include <fcntl.h>              /* macro O_RDONLY, 
                                   function open (in fcntl2.h) */
#include <errno.h>              /* errno */
#include "rio.h"                /* function rio_readn, rio_writen */

#define MAXBUF 8192

/* Print error message */
void error_msg (const char *msg) {
  fprintf (stderr, "%s: %s\n", msg, strerror(errno));
}

/* Close opened file descriptors */
void close_fd (const int srcfd, const int dstfd) {
  if ((close (srcfd)) < 0)
    error_msg ("Close source file error");

  if ((close (dstfd)) < 0)
    error_msg ("Close destination file error");
}

ssize_t Rio_readn (int srcfd, void *userbufp, size_t nbytes) {
  ssize_t n;

  if ((n = rio_readn (srcfd, userbufp, nbytes)) < 0) {
    error_msg ("Read error");
    return -1;
  }
  return n;
}

int Rio_writen (int dstfd, void *userbufp, size_t nbytes) {
  ssize_t n;

  if ((rio_writen (dstfd, userbufp, nbytes)) != n) {
    error_msg ("Write error");
    return -1;
  }
}

int copy (const char *src, const char *dst, 
          char *userbuf, size_t bufsize) {
  int srcfd, dstfd;
  size_t n;

  /* Open file */
  if ((srcfd = open (src, O_RDONLY, 0)) < 0) {
    error_msg ("Open source file error");
    return -1;
  }
  
  if ((dstfd = open (dst, O_CREAT|O_TRUNC|O_RDWR, 0664)) < 0) {
    error_msg ("Open destination file error");
    return -1;
  }

  /* Copy file */
  while ((n = Rio_readn (srcfd, userbuf, bufsize)) > 0) {
    printf ("read %ld bytes\n", n);

    if (Rio_writen (dstfd, userbuf, n) < 0)
      break;
  }

  /* Close file */
  close_fd (srcfd, dstfd);
}

int main (int argc, char **argv) {
  size_t readsize;
  char buf[MAXBUF];

  if (argc != 3) {
    fprintf (stderr, "usage: %s <source file> <destination file>\n", argv[0]);
    exit (1);
  }

  copy (argv[1], argv[2], buf, MAXBUF);
}
