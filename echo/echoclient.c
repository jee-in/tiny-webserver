#include <stdio.h>                      /* function printf, fprintf */
#include <stdlib.h>                     /* function exit */
#include <string.h>                     /* function mset */

#include "wrapper/w_rio.h"              /* function Rio_readinitb, Rio_writen, Rio_readlineb */
#include "wrapper/w_stdio.h"            /* function Fgets, Fputs */
#include "wrapper/w_unixio.h"           /* function Close */
#include "wrapper/w_sockhelper.h"       /* function Open_clientfd */

#define	MAXLINE	 8192  /* Max text line length */

int main (int argc, char **argv) {
  int clientfd;
  char *host, *port, buf[MAXLINE];
  rio_t rio;

  if (argc != 3) {
    fprintf (stderr, "usage: %s <host> <port>\n", argv[0]);
    exit (0);
  }
  host = argv[1];
  port = argv[2];

  clientfd = Open_clientfd (host, port);
  Rio_readinitb (&rio, clientfd);

  while (Fgets (buf, MAXLINE, stdin) != NULL) {
    Rio_writen (clientfd, buf, strlen (buf));
    Rio_readlineb (&rio, buf, MAXLINE);
    Fputs (buf, stdout);
  }
  Close (clientfd);
  exit (0);
}