#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/ip_icmp.h>   //Provides declarations for icmp header
#include <netinet/udp.h>   //Provides declarations for udp header
#include <netinet/tcp.h>   //Provides declarations for tcp header
#include <netinet/ip.h>    //Provides declarations for ip headers
#include <netinet/if_ether.h>
#include <arpa/inet.h>

#define PACKET_SIZE 65536
#define BUFFER_SIZE 256


void ProcessPacket(u_char* , int);
void print_ip_header(u_char* , int);
void print_tcp_packet(u_char* , int);
void print_udp_packet(u_char * , int);
void print_icmp_packet(u_char* , int);
void PrintData (u_char* , int);

int i, j;
FILE *logfile;
struct sockaddr_in source,dest;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char ** argv){
    logfile= fopen("xxx.txt", "w");
    char buffer_read[PACKET_SIZE];
    char buffer_write[BUFFER_SIZE];
    int ser_sock, newser_sock;
    struct sockaddr_in ser_addr, cli_addr;
    if(argc < 2){
        fprintf(stderr, "Please provide a port !");
        return 1;
    }

    ser_sock= socket(AF_INET, SOCK_STREAM, 0);
    if(ser_sock<0){
        error("ERROR on create !");
    }

    ser_addr.sin_family= AF_INET;
    ser_addr.sin_port= htons(atoi(argv[1]));
    ser_addr.sin_addr.s_addr= INADDR_ANY;
    if(bind(ser_sock, (struct sockaddr*)&ser_addr, sizeof(ser_addr))<0){
        error("ERROR on bind !");
    }

    listen(ser_sock, 5);


    unsigned int cli_len= sizeof(cli_addr);
    if((newser_sock=accept(ser_sock, (struct sockaddr*)&cli_addr, &cli_len))<0){
        error("ERROR on accept !");
    }

    while(1){
        bzero(buffer_read, sizeof(buffer_read));
        int n= read(newser_sock, buffer_read, sizeof(buffer_read));
        if(n<0) continue;
        printf("%d\n", strcmp(buffer_read, "quit"));
        if(!strcmp(buffer_read, "quit")){
            printf("SERVER STOP !\n");
            break;
        }
        u_char* buffer= (u_char*) buffer_read;
        struct iphdr *iph = (struct iphdr*)buffer;
        ProcessPacket(buffer, ntohs(iph->tot_len));

        strcpy(buffer_write, "Read!");
        n= write(newser_sock, buffer_write, strlen(buffer_write));
        if(n<0){
            perror("ERROR write to socket !");
            continue;
        }
    }

    close(ser_sock);
    close(newser_sock);
    fclose(logfile);
    return 0;
}


void ProcessPacket(u_char* buffer, int size)
{
    //Get the IP Header part of this packet
    struct iphdr *iph = (struct iphdr*)buffer;
    // ++total;
    switch (iph->protocol) //Check the Protocol and do accordingly...
    {
        case 1:  //ICMP Protocol
            // ++icmp;
            print_icmp_packet(buffer, size);
            break;
         
        // case 2:  //IGMP Protocol
        //     // ++igmp;
        //     print_igmp_packet(buffer, size);
        //     break;
         
        case 6:  //TCP Protocol
            // ++tcp;
            print_tcp_packet(buffer , size);
            break;
         
        case 17: //UDP Protocol
            // ++udp;
            print_udp_packet(buffer , size);
            break;
         
        default: //Some Other Protocol like ARP etc.
            // ++others;
            break;
    }
    // printf("TCP : %d   UDP : %d   ICMP : %d   IGMP : %d   Others : %d   Total : %d\r",tcp,udp,icmp,igmp,others,total);
}
 
void print_ip_header(u_char* Buffer, int Size)
{
    unsigned short iphdrlen;
         
    struct iphdr *iph = (struct iphdr *)Buffer;
    iphdrlen =iph->ihl*4;
     
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
     
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;
     
    fprintf(logfile,"\n");
    fprintf(logfile,"IP Header\n");
    fprintf(logfile,"   |-IP Version        : %d\n",(unsigned int)iph->version);
    fprintf(logfile,"   |-IP Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);
    fprintf(logfile,"   |-Type Of Service   : %d\n",(unsigned int)iph->tos);
    fprintf(logfile,"   |-IP Total Length   : %d  Bytes(Size of Packet)\n",ntohs(iph->tot_len));
    fprintf(logfile,"   |-Identification    : %d\n",ntohs(iph->id));
    //fprintf(logfile,"   |-Reserved ZERO Field   : %d\n",(unsigned int)iphdr->ip_reserved_zero);
    //fprintf(logfile,"   |-Dont Fragment Field   : %d\n",(unsigned int)iphdr->ip_dont_fragment);
    //fprintf(logfile,"   |-More Fragment Field   : %d\n",(unsigned int)iphdr->ip_more_fragment);
    fprintf(logfile,"   |-TTL      : %d\n",(unsigned int)iph->ttl);
    fprintf(logfile,"   |-Protocol : %d\n",(unsigned int)iph->protocol);
    fprintf(logfile,"   |-Checksum : %d\n",ntohs(iph->check));
    fprintf(logfile,"   |-Source IP        : %s\n",inet_ntoa(source.sin_addr));
    fprintf(logfile,"   |-Destination IP   : %s\n",inet_ntoa(dest.sin_addr));
}
 
void print_tcp_packet(u_char* Buffer, int Size)
{
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)Buffer;
    iphdrlen = iph->ihl*4;
     
    struct tcphdr *tcph=(struct tcphdr*)(Buffer + iphdrlen);
             
    fprintf(logfile,"\n\n***********************TCP Packet*************************\n");    
         
    print_ip_header(Buffer,Size);
         
    fprintf(logfile,"\n");
    fprintf(logfile,"TCP Header\n");
    fprintf(logfile,"   |-Source Port      : %u\n",ntohs(tcph->source));
    fprintf(logfile,"   |-Destination Port : %u\n",ntohs(tcph->dest));
    fprintf(logfile,"   |-Sequence Number    : %u\n",ntohl(tcph->seq));
    fprintf(logfile,"   |-Acknowledge Number : %u\n",ntohl(tcph->ack_seq));
    fprintf(logfile,"   |-Header Length      : %d DWORDS or %d BYTES\n" ,(unsigned int)tcph->doff,(unsigned int)tcph->doff*4);
    //fprintf(logfile,"   |-CWR Flag : %d\n",(unsigned int)tcph->cwr);
    //fprintf(logfile,"   |-ECN Flag : %d\n",(unsigned int)tcph->ece);
    fprintf(logfile,"   |-Urgent Flag          : %d\n",(unsigned int)tcph->urg);
    fprintf(logfile,"   |-Acknowledgement Flag : %d\n",(unsigned int)tcph->ack);
    fprintf(logfile,"   |-Push Flag            : %d\n",(unsigned int)tcph->psh);
    fprintf(logfile,"   |-Reset Flag           : %d\n",(unsigned int)tcph->rst);
    fprintf(logfile,"   |-Synchronise Flag     : %d\n",(unsigned int)tcph->syn);
    fprintf(logfile,"   |-Finish Flag          : %d\n",(unsigned int)tcph->fin);
    fprintf(logfile,"   |-Window         : %d\n",ntohs(tcph->window));
    fprintf(logfile,"   |-Checksum       : %d\n",ntohs(tcph->check));
    fprintf(logfile,"   |-Urgent Pointer : %d\n",tcph->urg_ptr);
    fprintf(logfile,"\n");
    fprintf(logfile,"                        DATA Dump                         ");
    fprintf(logfile,"\n");
         
    fprintf(logfile,"IP Header\n");
    PrintData(Buffer,iphdrlen);
         
    fprintf(logfile,"TCP Header\n");
    PrintData(Buffer+iphdrlen,tcph->doff*4);
         
    fprintf(logfile,"Data Payload\n");  
    PrintData(Buffer + iphdrlen + tcph->doff*4 , (Size - tcph->doff*4-iph->ihl*4) );
                         
    fprintf(logfile,"\n###########################################################");
}
 
void print_udp_packet(u_char *Buffer , int Size)
{
     
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)Buffer;
    iphdrlen = iph->ihl*4;
     
    struct udphdr *udph = (struct udphdr*)(Buffer + iphdrlen);
     
    fprintf(logfile,"\n\n***********************UDP Packet*************************\n");
     
    print_ip_header(Buffer,Size);           
     
    fprintf(logfile,"\nUDP Header\n");
    fprintf(logfile,"   |-Source Port      : %d\n" , ntohs(udph->source));
    fprintf(logfile,"   |-Destination Port : %d\n" , ntohs(udph->dest));
    fprintf(logfile,"   |-UDP Length       : %d\n" , ntohs(udph->len));
    fprintf(logfile,"   |-UDP Checksum     : %d\n" , ntohs(udph->check));
     
    fprintf(logfile,"\n");
    fprintf(logfile,"IP Header\n");
    PrintData(Buffer , iphdrlen);
         
    fprintf(logfile,"UDP Header\n");
    PrintData(Buffer+iphdrlen , sizeof udph);
         
    fprintf(logfile,"Data Payload\n");  
    PrintData(Buffer + iphdrlen + sizeof udph ,( Size - sizeof udph - iph->ihl * 4 ));
     
    fprintf(logfile,"\n###########################################################");
}
 
void print_icmp_packet(u_char* Buffer , int Size)
{
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)Buffer;
    iphdrlen = iph->ihl*4;
     
    struct icmphdr *icmph = (struct icmphdr *)(Buffer + iphdrlen);
             
    fprintf(logfile,"\n\n***********************ICMP Packet*************************\n");   
     
    print_ip_header(Buffer , Size);
             
    fprintf(logfile,"\n");
         
    fprintf(logfile,"ICMP Header\n");
    fprintf(logfile,"   |-Type : %d",(unsigned int)(icmph->type));
             
    if((unsigned int)(icmph->type) == 11) 
        fprintf(logfile,"  (TTL Expired)\n");
    else if((unsigned int)(icmph->type) == ICMP_ECHOREPLY) 
        fprintf(logfile,"  (ICMP Echo Reply)\n");
    fprintf(logfile,"   |-Code : %d\n",(unsigned int)(icmph->code));
    fprintf(logfile,"   |-Checksum : %d\n",ntohs(icmph->checksum));
    //fprintf(logfile,"   |-ID       : %d\n",ntohs(icmph->id));
    //fprintf(logfile,"   |-Sequence : %d\n",ntohs(icmph->sequence));
    fprintf(logfile,"\n");
 
    fprintf(logfile,"IP Header\n");
    PrintData(Buffer,iphdrlen);
         
    fprintf(logfile,"UDP Header\n");
    PrintData(Buffer + iphdrlen , sizeof icmph);
         
    fprintf(logfile,"Data Payload\n");  
    PrintData(Buffer + iphdrlen + sizeof icmph , (Size - sizeof icmph - iph->ihl * 4));
     
    fprintf(logfile,"\n###########################################################");
}
 
void PrintData (u_char* data , int Size)
{
     
    for(i=0 ; i < Size ; i++)
    {
        if( i!=0 && i%16==0)   //if one line of hex printing is complete...
        {
            fprintf(logfile,"         ");
            for(j=i-16 ; j<i ; j++)
            {
                if(data[j]>=32 && data[j]<=128)
                    fprintf(logfile,"%c",(unsigned char)data[j]); //if its a number or alphabet
                 
                else fprintf(logfile,"."); //otherwise print a dot
            }
            fprintf(logfile,"\n");
        } 
         
        if(i%16==0) fprintf(logfile,"   ");
            fprintf(logfile," %02X",(unsigned int)data[i]);
                 
        if( i==Size-1)  //print the last spaces
        {
            for(j=0;j<15-i%16;j++) fprintf(logfile,"   "); //extra spaces
             
            fprintf(logfile,"         ");
             
            for(j=i-i%16 ; j<=i ; j++)
            {
                if(data[j]>=32 && data[j]<=128) fprintf(logfile,"%c",(unsigned char)data[j]);
                else fprintf(logfile,".");
            }
            fprintf(logfile,"\n");
        }
    }
}