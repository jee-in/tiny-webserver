#include <stdio.h>            /* macro stderr, function fprintf */
#include <stdlib.h>           /* function exit */
#include <string.h>           /* function strerror */
#include <unistd.h>           /* macro STDIN_FILENO */
#include <sys/select.h>       /* macro FD_ZERO, FD_SET, FD_ISSET,
                                 struct fd_Set, function select */
#include <sys/socket.h>       /* struct sockaddr_storage,
                                 macro socklen_t, function accept */
#include <errno.h>            /* errno */


#include "sockhelper.h"       /* function open_listenfd */
#include "rio.h"              /* struct rio_t,
                                 function rio_readinitb, rio_readlineb
                                 rio_writen  */

#define MAXLINE 8192

void echo (int connfd);
void command (void);

/* Print error message */
void error_msg (const char *msg) {
  fprintf (stderr, "%s: %s\n", msg, strerror(errno));
}

/* Wrapper for rio_readlineb */
ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) {
  ssize_t rc;

  if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0)
	  error_msg ("Rio_readlineb error");
  return rc;
} 

void echo (int connfd) {
  size_t n;
  char buf[MAXLINE];
  rio_t rio;

  rio_readinitb (&rio, connfd);
  while ((n = Rio_readlineb (&rio, buf, MAXLINE)) != 0) {
    printf("server received %d bytes\n", (int)n);
    if (rio_writen(connfd, buf, n) != n)
	    error_msg ("Rio_writen error");
  }
}

void command (void) {
  char buf[MAXLINE];
  if (fgets (buf, MAXLINE, stdin) == NULL) {
    if (ferror (stdin)) {
      fprintf (stderr, "Fgets error");
      exit (1);
    } else {
      exit (0);       /* encounter EOF */
    }
    printf ("%s", buf);
  }
}

int main (int argc, char **argv) {
  int listenfd, connfd;
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;
  fd_set read_set, ready_set;

  if (argc != 2) {
    fprintf (stderr, "usage: %s <port>\n", argv[0]);
    exit (1);
  }

  /* Create a socket descriptor, 
     bind it to the address and make it a listening socket */
  if ((listenfd = open_listenfd(argv[1])) < 0) {
    error_msg ("Open_listenfd error");
    exit (1);
  }

  FD_ZERO (&read_set);               /* Clear read set */
  FD_SET (STDIN_FILENO, &read_set);  /* Add stdin to read set */
  FD_SET (listenfd, &read_set);      /* Add listenfd to read set */

  while (1) {
    ready_set = read_set;
    if (select (listenfd + 1, &ready_set, NULL, NULL, NULL) < 0) {
      error_msg ("Select error");
      exit (1);
    }
    if (FD_ISSET (STDIN_FILENO, &ready_set))
      command ();   /* Read command line from stdin */
    if (FD_ISSET (listenfd, &ready_set)) {
      clientlen = sizeof (struct sockaddr_storage);
      if ((connfd = 
       accept (listenfd, (struct sockaddr *)&clientaddr, &clientlen)) < 0) {
        error_msg ("Accept error");
        exit (1);
      }
      echo (connfd);
      if (close (connfd) < 0) {
        error_msg ("Close error");
        exit (1);
      }
    }
  }
}