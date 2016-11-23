#include "client_socket.h"

ClientSocket::ClientSocket(){}

int ClientSocket::connet(char *_host, char *service)
{
  struct sockaddr_in direction;
  struct servent *port;
  struct hostent *host;
  int descriptor;

  port = getservbyname(service, "tcp");
  if (port == NULL)
    return -1;

  host = gethostbyname(_host);
  if (host == NULL)
    return -1;

  direction.sin_family = AF_INET;
  direction.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr;
  direction.sin_port = port->s_port;

  descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (descriptor == -1)
    return -1;

  if (connect(
	  descriptor,
	  (struct sockaddr *)&direction,
	  sizeof(direction)) == -1)
  {
    return -1;
  }
  fd = descriptor;
  return descriptor;
}

int ClientSocket::readSocket(char *buf, int size)
{
	int reading = 0;
  int aux = 0;
  if ((fd == -1) || (buf == NULL) || (size < 1))
    return -1;

  while (reading < size)
  {
    aux = read(fd, buf + reading, size - reading);
    if (aux > 0)
    {
      reading = reading + aux;
    }
    else
    {
      if (aux == 0)
      {
	return reading;
      }
      if (aux == -1)
      {
	switch (errno)
	{
	case EINTR:
	case EAGAIN:
	  usleep(100);
	  break;
	default:
	  return -1;
	}
      }
    }
  }
  return reading;
}

int ClientSocket::writeSocket(char *buf, int size)
{
  int writing = 0;
  int aux = 0;

  if ((fd == -1) || (buf == NULL) || (size < 1))
    return -1;

  while (writing < size)
  {
    aux = write(fd, buf + writing, size - writing);
    if (aux > 0)
    {
      writing = writing + aux;
    }
    else
    {
      if (aux == 0)
	return writing;
      else
	return -1;
    }
  }
  return writing;
}

  void ClientSocket::closeSocket(){
    close(fd);
  }

  ClientSocket::~ClientSocket(){
    closeSocket();
  }