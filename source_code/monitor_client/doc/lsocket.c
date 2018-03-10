#include "lsocket.h"

int OpenServerSocket(int port, unsigned int max_connect){
    //create socket
    int ser_soc;
    struct sockaddr_in ser_addr;
    if((ser_soc= socket(AF_INET, SOCK_STREAM, 0))<0){
        perror("[ERROR] Creating socket fail !");
        return -1;
    }

    //bind socket
    ser_addr.sin_family= AF_INET;
    ser_addr.sin_addr.s_addr= INADDR_ANY;
    ser_addr.sin_port= htons(port);
    if(bind(ser_soc, (struct sockaddr*)&ser_addr, sizeof(ser_addr))<0){
        perror("[ERROR] Binding socket fail !");
        return -1;
    }

    //listen to connection 
    listen(ser_soc, max_connect);
    return ser_soc;
}

int ConnectServer(const char *host, int port){
    int cli_sock;
    struct sockaddr_in ser_sock;
    struct hostent* server;

    if((cli_sock= socket(AF_INET, SOCK_STREAM, 0))<0){
        perror("[ERROR] on creating socket ");
        return -1;
    }

    if((server= gethostbyname((char*)host))==NULL){
        perror("[ERROR] no such host ");
        return -1;
    }

    bzero((char *)&ser_sock, sizeof(ser_sock));
    ser_sock.sin_family= AF_INET;
    bcopy((char*)server->h_addr, (char*)&(ser_sock.sin_addr.s_addr), server->h_length);
    ser_sock.sin_port= htons(port);

    if(connect(cli_sock, (struct sockaddr*)&ser_sock, sizeof(ser_sock))<0){
        perror("[ERROR] on connecting host ");
        return -1;
    }
    return cli_sock;
}