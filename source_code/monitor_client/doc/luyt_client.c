#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pcap.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 65536

void error(const char *message);
int ConnectServer(const char *host, int port);
void my_callback(u_char *args, const struct pcap_pkthdr* pkthdr, const u_char* packet);

int cli_sock;
struct sockaddr_in source,dest;

int main(int argc, char ** argv){
    char buffer_read[BUFFER_SIZE];
    char buffer_write[BUFFER_SIZE];
    char errbuf[PCAP_ERRBUF_SIZE];
    char *dev;
    bpf_program filter;
    bpf_u_int32 netp;
    bpf_u_int32 maskp;
    pcap_t *handler;

    //
    //-----CONNECT TO SERVER HOST-----
    //
    if(argc<4){
        fprintf(stderr, "Please enter host name, port and filter code !");
        return 1;
    }
    if((cli_sock=ConnectServer(argv[1], atoi(argv[2])))<0){
        return 1;
    }
    
        //
        //CAPTURE PACKET
        //
    if((dev= pcap_lookupdev(errbuf))==NULL){
        fprintf(stderr, "%s\n", errbuf);
        close(cli_sock);
        return 1;
    }

    pcap_lookupnet(dev, &netp, &maskp, errbuf);

    if((handler = pcap_open_live(dev, BUFSIZ, 1,-1, errbuf))==NULL){
        fprintf(stderr, "%s\n", errbuf);
        close(cli_sock);
        return 1;
    }
 
    /* Now we'll compile the filter expression*/
    if(pcap_compile(handler, &filter, argv[3], 0, netp) == -1) {
        fprintf(stderr, "Error calling pcap_compile\n");
        close(cli_sock);
        return 1;
    }
 
    /* set the filter */
    if(pcap_setfilter(handler, &filter) == -1) {
        fprintf(stderr, "Error setting filter\n");
        close(cli_sock);
        return 1;
    }
 
    /* loop for callback function */
    pcap_loop(handler, -1, my_callback, NULL);

    //
    //-----CLOSE CONNECTION-----
    //
    close(cli_sock);
    return 0;
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

void my_callback(u_char *args, const struct pcap_pkthdr* pkthdr, const u_char* packet)
{
    u_char* buffer= (u_char*) packet+14;
    struct iphdr *iph = (struct iphdr*)buffer;
    //ProcessPacket(buffer, ntohs(iph->tot_len));
    source.sin_addr.s_addr = iph->saddr;
    dest.sin_addr.s_addr= iph->daddr;
    // if(strcmp(inet_ntoa(source.sin_addr), "192.168.1.2")!=0||strcmp(inet_ntoa(source.sin_addr), "192.168.1.2")!=0){
        if(write(cli_sock, buffer, ntohs(iph->tot_len))<0){
            perror("[ERROR] write to socket ");
            close(cli_sock);
            exit(1);
        }

    // }
}