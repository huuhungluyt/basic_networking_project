#include <pcap.h>
#include <stdio.h>

typedef struct ip_address{
	u_char octet1;
	u_char octet2;
	u_char octet3;
	u_char octet4;
}ip_address;

typedef struct MAC_Header{
	//6 bytes for SOURCE MAC ADDRESS
	u_char smac1;
	u_char smac2;
	u_char smac3;
	u_char smac4;
	u_char smac5;
	u_char smac6;
	//6 bytes for DESTINATION MAC ADDRESS
	u_char dmac1;
	u_char dmac2;
	u_char dmac3;
	u_char dmac4;
	u_char dmac5;
	u_char dmac6;
	//2 bytes for ETHERNET TYPE
	u_short type;
}MAC_Header;

typedef struct IP_Header{
	u_char ver_hlen;	//4 bits for IP VERSION, 4 bits for IP HEADER LENGTH
	u_char tos;			//1 byte for TYPE OF SERVICE
	u_short tolen;		//2 bytes for IP PACKET TOTAL LENGTH
	u_short id;			//2 bytes for PACKET ID
	u_short frag;		//3 bits for FRAGMENT FLAG (0, DF, MF), 13 for FRAGMENT OFFSET;
	u_char ttl;			//1 byte for TIME TO LIVE
	u_char proto;		//1 byte for NEXT HEADER PROTOCOL
	u_short cs;			//2 bytes for CHECKSUM
	ip_address sip;		//4 bytes for SOURCE IP ADDRESS
	ip_address dip;		//4 bytes for DESTINATION IP ADDRESS
	u_int option;		//4 bytes for OPTION FIELD
}IP_Header;

typedef struct UDP_Header{
	u_short sport;		//2 bytes for SOURCE PORT
	u_short dport;		//2 bytes for DESTINATION PORT
	u_short len;		//2 bytes for UPD LENGTH
	u_short cs;			//2 bytes for UDP CHECKSUM
}UDP_Header;


typedef struct TCP_Header{
	u_short sport;		//2 bytes for SOURCE PORT
	u_short dport;		//2 bytes for DESTINATION PORT
	u_int seq;			//4 bytes for SEQUENCE NUMBER
	u_int ack;			//4 bytes for ACKNOWLEDGEMENT NUMBER
	u_char dr;			//4 bits for DATA OFFSET, 4 bits for RESERVED
	u_char cflag;		//8 bits for CONTROL FLAGS
	u_short wsize;		//2 bytes for WINDOW SIZE
	u_short cs;			//2 bytes for CHECKSUM
	u_short up;			//2 bytes for URGENT POINTER
}TCP_Header;

void PrintMACHeader(struct MAC_Header *header);

void PrintIPHeader(struct IP_Header *header);

void PrintUDPHeader(struct UDP_Header *header);

void PrintTCPHeader(struct TCP_Header *header);

void PrintDevInfo(int i, pcap_if_t *dev);

char *IpToString(u_long in);

char *Ip6ToString(struct sockaddr *addrstruct, char *ip6str);