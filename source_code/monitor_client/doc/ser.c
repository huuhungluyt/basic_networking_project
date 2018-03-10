#include <stdio.h>
#include <string.h>
#include "lsocket.h"

#define BUFFER_SIZE 256

int main (int argc, char ** argv) {
    char read_buff[BUFFER_SIZE];
    char write_buff[BUFFER_SIZE];
    int ser_soc, cli_soc;
    socklen_t clilen;
    struct sockaddr_in ser_addr, cli_addr;

    //check port input
    if(argc<2){
        fprintf(stderr, "[ERROR] Please enter port number !\n");
        return 1;
    }

    //open server socket to listen connect
    if((ser_soc=OpenServerSocket(atoi(argv[1]), 5))<0){
        fprintf(stderr, "[ERROR] Open server socket fail !\n");
        return 1;
    }

    //accept connec from client    
    clilen= sizeof(cli_addr);
    if((cli_soc= accept(ser_soc, (struct sockaddr*)&cli_addr, &clilen))<0){
        perror("[ERROR] Cannot accept connect from lient !");
        return 1;
    }

    while(1){
        if(read(cli_soc, read_buff, sizeof(read_buff))<0) continue;
        printf("Client: %s\n", read_buff);

        if(strcmp(read_buff, "quit")==0){
            printf("Server closed !\n");
            break;
        }

        strcpy(write_buff, "Server: Read !");
        if(write(cli_soc, write_buff, strlen(write_buff))<0){
            perror("[ERROR] Cannot write to socket !");
            break;
        }
    }

}