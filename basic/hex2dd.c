#include <netinet/in.h>     /* struct in_addr, function htonl */
#include <arpa/inet.h>      /* function inet_ntop */
#include <stdio.h>
#include <stdlib.h>         /* function exit */

#define MAXBUF 8192 // 8MB

int main (int argc, char **argv) {
  struct in_addr inaddr;
  uint32_t addr;
  char buf[MAXBUF];

  if (argc != 2) {
    fprintf (stderr, "%s requires one hex address for argument\n", argv[0]);
    exit (1);
  }
  sscanf (argv[1], "%x", &addr);

  inaddr.s_addr = htonl (addr); 
  printf ("Convert host byte order (Little Endian) %s "
          "into network byte order (Big Endian).\n %x.\n", 
          argv[1], inaddr.s_addr);

  if (!inet_ntop(AF_INET, &inaddr, buf, MAXBUF)) {
    fprintf(stderr, "inet_ntop Failed\n");
    exit (1);
  }
  printf("Represent an IP address %x in dotted-decimal notation.\n %s\n", 
          inaddr.s_addr, buf);

  exit (0);
}
