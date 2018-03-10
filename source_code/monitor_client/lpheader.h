#include <string.h> 
#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

#include <iostream>

void print_packet_headers(unsigned char *data, int size);

void print_ip_header(struct iphdr *ip);
void print_tcp_header(struct tcphdr *tcp);
void print_udp_header(struct udphdr *udp);
void print_icmp_header(struct icmphdr *icmp);

void print_raw_data(unsigned char *data, int size);

bool IsLocked(const char *type, const char *ip);
bool Lock(const char *ip);
bool Unlock(const char *ip);