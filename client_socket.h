#ifndef _CLIENT_SOCKET_H
#define _CLIENT_SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

class ClientSocket
{
private:
  int fd;

public:
  ClientSocket();
  int connet(char *_host, char *service);
  int readSocket(char *buf, int size);
  int writeSocket(char *buf, int size);
  void closeSocket();
  virtual ~ClientSocket();
};

#endif
