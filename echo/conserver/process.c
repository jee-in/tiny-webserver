#include <stdio.h>                /* macro stderr, function fprintf */
#include <stdlib.h>               /* function exit */
#include <string.h>               /* function strerror */
#include <signal.h>               /* macro SIGCHLD,SA_RESTART,
                                     function sigemptyset, sigaction */
#include <sys/socket.h>           /* function accept, close */
#include <unistd.h>               /* function fork, close */
#include <sys/wait.h>             /* function waitpid */
#include <errno.h>                /* errno */

#include "sockhelper.h"           /* function open_listenfd */
#include "rio.h"                  /* function rio_readinitb, rio_readlineb,
                                     rio_writen */
#define	MAXLINE	 8192
typedef void (*sighandler_t) (void);

/* Print error message */
void error_msg (const char *msg) {
  fprintf (stderr, "%s: %s\n", msg, strerror(errno));
}

/* User defined signal handler */
void sigchld_handler (int sig) {
  /* Parent process wait until every child process in wait set
     terminate */
  while (waitpid (-1, 0, WNOHANG) > 0)
    ;
  return;
}

/* Wrapper for rio_readlineb */
ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) {
  ssize_t rc;

  if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0)
	  error_msg ("Rio_readlineb error");
  return rc;
} 

/* Service that server provides to client */
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

int main (int argc, char **argv) {
  char *port;
  int listenfd, connfd;
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;
  struct sigaction action, old_action;
  pid_t pid;

  if (argc != 2) {
    fprintf (stderr, "usage: %s <port>\n", argv[0]);
    exit (1);
  }
  port = argv[1];

  /* Make interrupted system call restart when signal handler
     returns by setting signal action flag (Portable code) */
  action.sa_handler = sigchld_handler;  
  sigemptyset (&action.sa_mask); /* Initialize blocked list */
  action.sa_flags = SA_RESTART;  /* Restart syscalls if possible */

  if (sigaction (SIGCHLD, &action, &old_action) < 0) {
    error_msg ("Signal error");
    exit (1);
  }

  /* Create a socket descriptor, 
     bind it to the address and make it a listening socket */
  if ((listenfd = open_listenfd (port)) < 0) {
    error_msg ("Open_listenfd error");
    exit (1);
  }

  while (1) {
    /* Wait for connection request from client */
    clientlen = sizeof (struct sockaddr_storage);
    if ((connfd = accept (listenfd, (struct sockaddr *) &clientaddr, &clientlen)) < 0) {
      error_msg ("Accept error");
      exit (1);
    }

    /* Create child process to service for connected client */
    if ((pid = fork ()) < 0) {
      error_msg ("Fork error");
      exit (1);
    } else if (pid == 0) {
      /* Child closes its listening socket which is not
         needed for it */
      if ((close (listenfd)) < 0) {
        error_msg ("Close error");
        exit (1);
      }

      /* Child services connected client*/
      echo (connfd);
      
      /* Child closes connection with client */
      if ((close (connfd)) < 0) {
        error_msg ("Close error");
        exit (1);
      }
      
      /* Child exits */
      exit (0);
    }

    /* Parent must close connected socket after fork succeeds
       so that connected descriptor can be released */
    if ((close (connfd)) < 0) {
      error_msg ("Close error");
      exit (1);
    }
  }
}