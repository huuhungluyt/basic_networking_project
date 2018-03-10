#include "packet_header.h"


void PrintMACHeader(struct MAC_Header *header){
	printf("[ MAC HEADER ]\n");
	printf("Source MAC address: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n", header->smac1, header->smac2,header->smac3, header->smac4,header->smac5, header->smac6);
	printf("Destination MAC address: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n", header->dmac1, header->dmac2,header->dmac3, header->dmac4,header->dmac5, header->dmac6);
	printf("Ethernet type: %d\n", header->type);
}


void PrintIPHeader(struct IP_Header *header){
	struct ip_address sip= header->sip;
	struct ip_address dip= header->dip;
	printf("[ IP HEADER ]\n");
	printf("Version: %d\n", (header->ver_hlen&0xf0)>>4);
	printf("Header length: %d\n", (header->ver_hlen&0x0f)*4);
	printf("Precedence: %d\tType of service: %d\n", (header->tos&0xe0)>>5, header->tos&0x1f);
	printf("Total length: %d\n", header->tolen);
	printf("Packet ID: %x\n", header->id);
	printf("Fragment flags: %d\tFragment offset: %d\n", (header->frag&0xe000)>>13, header->frag&0x1fff);
	printf("Time to live: %d\n", header->ttl);
	printf("Next header protocol: %d\n", header->proto);
	printf("Checksum: %d\n", header->cs);
	printf("Source IP address: %d.%d.%d.%d\n", sip.octet1, sip.octet2, sip.octet3, sip.octet4);
	printf("Destination IP address: %d.%d.%d.%d\n", dip.octet1, dip.octet2, dip.octet3, dip.octet4);
}

void PrintUDPHeader(struct UDP_Header *header){
	printf("[ UDP HEADER ]\n");
	printf("Source port: %d\n", ntohs(header->sport));
	printf("Destination port: %d\n", ntohs(header->dport));
	printf("UDP Length: %d\n", header->len);
	printf("UDP Checksum: %d\n", header->cs);
}

void PrintTCPHeader(struct TCP_Header *header){
	printf("[ TCP HEADER ]\n");
	printf("Source port: %d\n", ntohs(header->sport));
	printf("Destination port: %d\n", ntohs(header->dport));
	printf("Sequence number: %x\n", header->seq);
	printf("Acknowledgement number: %x\n", header->ack);
	printf("Data offset: %x\n", (header->dr&0xf000)>>4);
	printf("Reserved: %d\n", header->dr&0x0f);
	printf("Flags: %s %s %s %s %s %s %s %s\n",
		(header->cflag&0x80)?"C":"-",(header->cflag&0x40)?"E":"-",(header->cflag&0x20)?"U":"-",(header->cflag&0x10)?"A":"-",
		(header->cflag&0x08)?"P":"-",(header->cflag&0x04)?"R":"-",(header->cflag&0x02)?"S":"-",(header->cflag&0x01)?"F":"-");
	printf("Window size: %d\n", header->wsize);
	printf("Checksum: %x\n", header->cs);
	printf("Urgent pointer: %x\n", header->up);
}

void PrintDevInfo(int i, pcap_if_t *dev){
	printf("\n\n%d.\t%s\n", i, dev->name);
	if(dev->description) printf("\tDescription: %s\n", dev->description);
	printf("\tLoopback: %s\n", (dev->flags&PCAP_IF_LOOPBACK)?"Yes":"No");
	pcap_addr_t *addrs= dev->addresses, *temp;
	for(temp= addrs; temp; temp= temp->next){
		switch(temp->addr->sa_family){
		case AF_INET:
			printf("\n\tAddress family: AF_INET\n");
			if(temp->addr) printf("\tAddress: %s\n", IpToString(((struct sockaddr_in*)temp->addr)->sin_addr.s_addr));
			if(temp->netmask) printf("\tNetmask: %s\n", IpToString(((struct sockaddr_in*)temp->netmask)->sin_addr.s_addr));
			if(temp->broadaddr) printf("\tBroadcast: %s\n", IpToString(((struct sockaddr_in*) temp->broadaddr)->sin_addr.s_addr));
			if(temp->dstaddr) printf("\tDestination: %s\n", IpToString(((struct sockaddr_in*) temp->dstaddr)->sin_addr.s_addr));
			break;
		case AF_INET6:
			printf("\n\tAddress family: AF_INET6\n");
			char ip6str[PCAP_BUF_SIZE];
			if(temp->addr) printf("\tAddress: %s\n", Ip6ToString(temp->addr, ip6str));
			break;
		default:
			printf("\n\tAddress family not found !\n");
		}
	}
}

char *IpToString(u_long in){
	static char ipstr[3*4+3+1];
	u_char *p= (u_char *) &in;
	_snprintf_s(ipstr, sizeof(ipstr), "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
	return ipstr;
}

char *Ip6ToString(struct sockaddr *addrstruct, char *ip6str){
	socklen_t addrlen= sizeof(sockaddr_in6);

	if(getnameinfo(addrstruct, addrlen, ip6str, sizeof(ip6str), NULL, NULL, NI_NUMERICHOST)!=0) return NULL;
	return ip6str;
}