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

int copy (const char *src, const char *dst, 
          char *userbuf, size_t bufsize) {
  FILE *srcfile, *dstfile;
  size_t n;

  /* Open file */
  if ((srcfile = fopen (src, O_RDONLY)))

  if ((dstfile = fopen (dst, O_RDONLY)))

  /* Copy file */
  while (1) {
    n = fgets ())
    if (n)
    
    printf ("read %ld bytes\n", n);

  }

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
