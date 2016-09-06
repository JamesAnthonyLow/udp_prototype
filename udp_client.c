#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/uio.h>

int main()
{
  const char* hostname=0; /* localhost */
  const char* portname="daytime";
  struct addrinfo hints, * res;
  ssize_t count;
  int err, fd;

  struct sockaddr_storage src_addr;
  socklen_t src_addr_len=sizeof(src_addr);

  memset(&hints,0,sizeof(hints));

  hints.ai_family=AF_UNSPEC;
  hints.ai_socktype=SOCK_DGRAM;
  hints.ai_protocol=0;
  hints.ai_flags=AI_ADDRCONFIG;

  res=0;

  err=getaddrinfo(hostname,portname,&hints,&res);
  if (err!=0) {
    fprintf(stderr, "failed to resolve remote socket address (err=%d)",err);
    exit(EXIT_FAILURE);
  }

  fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
  if (fd==-1) {
    fprintf(stderr, "%s",strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (bind(fd,res->ai_addr,res->ai_addrlen)==-1) {
    fprintf(stderr, "%s",strerror(errno));
    exit(EXIT_FAILURE);
  }

  freeaddrinfo(res);

  while(1){
    char buffer[549];
    count=recvfrom(fd,buffer,sizeof(buffer),0,(struct sockaddr*)&src_addr,&src_addr_len);
    printf("%s\n", buffer);
  }
}
