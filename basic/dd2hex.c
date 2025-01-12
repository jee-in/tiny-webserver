#include <netinet/in.h>     /* struct in_addr, function ntohl */
#include <arpa/inet.h>      /* function inet_ntop */
#include <stdio.h>
#include <stdlib.h>         /* function exit */

#define MAXBUF 8192 // 8MB

int main (int argc, char **argv) {
  struct in_addr inaddr;

  if (argc != 2) {
    fprintf (stderr, "%s requires one dotted-decimal string for argument\n", argv[0]);
    exit (1);
  }

  int result = inet_pton(AF_INET, argv[1], &inaddr);
  if (result == 0) {
    fprintf (stderr, "Invalid dotted-decimal string: %s\n", argv[1]);
    exit (1);
  }
  else if (result < 0) {
    perror ("inet_pton");
    exit (1);
  }
  printf("Represent a dotted-decimal string IP address %s in hex.\n %x\n", 
        argv[1], inaddr.s_addr);

  printf ("Convert network byte order (Big Endian) %x "
          "into host byte order (Little Endian).\n %x\n", 
          inaddr.s_addr, ntohl (inaddr.s_addr));
}
