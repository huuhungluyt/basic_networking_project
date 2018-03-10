#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>

int OpenServerSocket(int port, unsigned int max_conn);
int ConnectServer(const char *host, int port);