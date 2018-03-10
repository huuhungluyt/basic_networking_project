#include "lpheader.h"

void print_ip_header(struct iphdr *ip)
{
    struct sockaddr_in sourceip, destip;

    memset(&sourceip, 0, sizeof(sourceip));
    sourceip.sin_addr.s_addr = ip->saddr;
    memset(&destip, 0, sizeof(destip));
    destip.sin_addr.s_addr = ip->daddr;

    printf("[ IP Header ]\n");
    printf("\t|-IP version          : %d\n", (unsigned char)ip->version);
    printf("\t|-Ip header length    : %d DWORDS or %d BYTES\n", (unsigned char)ip->ihl, ((unsigned char)ip->ihl) * 4);
    printf("\t|-Type of service     : %d\n", (unsigned char)ip->tos);
    printf("\t|-Total length        : %d BYTES\n", ntohs(ip->tot_len));
    printf("\t|-Identification      : %d\n", ntohs(ip->id));
    printf("\t|-Time to live        : %d\n", (unsigned char)ip->ttl);
    printf("\t|-Protocol            : %d\n", (unsigned char)ip->protocol);
    printf("\t|-Checksum            : %d\n", ntohs(ip->check));
    printf("\t|-Source IP           : %s\n", inet_ntoa(sourceip.sin_addr));
    printf("\t|-Destination IP      : %s\n", inet_ntoa(destip.sin_addr));
}

void print_tcp_header(struct tcphdr *tcp)
{
    printf("[ TCP Header ]\n");
    printf("\t|-Source port : %u\n", ntohs(tcp->source));
    printf("\t|-Destination port    : %u\n", ntohs(tcp->dest));
    printf("\t|-Sequence numer      : %u\n", ntohl(tcp->seq));
    printf("\t|-Acknowledge number  : %u\n", ntohl(tcp->ack_seq));
    printf("\t|-Header length       : %d DWORDS or %d BYTES\n", (unsigned char)tcp->doff, (unsigned char)tcp->doff * 4);
    printf("\t|-Flags               :\n");
    // printf("\t\t|-CWR       : %d\n", (unsigned char)tcp->cwr);
    // printf("\t\t|-ECN       : %d\n", (unsigned char)tcp->ece);
    printf("\t\t|-URG       : %d\n", (unsigned char)tcp->urg);
    printf("\t\t|-ACK       : %d\n", (unsigned char)tcp->ack);
    printf("\t\t|-PSH       : %d\n", (unsigned char)tcp->psh);
    printf("\t\t|-RST       : %d\n", (unsigned char)tcp->rst);
    printf("\t\t|-SYN       : %d\n", (unsigned char)tcp->syn);
    printf("\t\t|-FIN       : %d\n", (unsigned char)tcp->fin);
    printf("\t|-Window size         : %d\n", ntohs(tcp->window));
    printf("\t|-Checksum            : %d\n", ntohs(tcp->check));
    printf("\t|-Urgent Pointer      : %d\n", tcp->urg_ptr);
}

void print_udp_header(struct udphdr *udp)
{
    printf("[ UPD Header ]\n");
    printf("\t|-Source port         : %d\n", ntohs(udp->source));
    printf("\t|-Destination port    : %d\n", ntohs(udp->dest));
    printf("\t|-UDP length          : %d\n", ntohs(udp->len));
    printf("\t|-UDP checksum        : %d\n", ntohs(udp->check));
}

void print_icmp_header(struct icmphdr *icmp)
{
    printf("[ ICMP Header ]\n");
    printf("\t|-Type                : %d", (unsigned char)(icmp->type));
    if (((unsigned char)icmp->type) == 11)
        printf("\t(TTL Expired)\n");
    else if (((unsigned char)icmp->type) == ICMP_ECHOREPLY)
        printf("\t(ICMP Echo Reply)\n");
    printf("\t|-Code                : %d\n", (unsigned char)(icmp->code));
    printf("\t|-Checksum            : %d\n", ntohs(icmp->checksum));
}

void print_packet_headers(unsigned char *data, int size)
{
    printf("[ Data Payload ]\n");
    unsigned char *ipoffset = data;
    struct iphdr *ip = (struct iphdr *)ipoffset;
    int iphlen = ip->ihl * 4;
    switch (ip->protocol)
    {
    case 1:{
        //ICMP
        printf("\n-----------------------ICMP PACKET----------------------\n");
        printf("\n");
        print_ip_header(ip);
        printf("\n");
        print_raw_data(ipoffset, iphlen);
        printf("\n");
        struct icmphdr *icmp = (struct icmphdr *)(ipoffset + iphlen);
        print_icmp_header(icmp);
        printf("\n");
        print_raw_data(ipoffset + iphlen, sizeof(struct icmphdr));
        printf("\nData Payload\n");
        print_raw_data(ipoffset + iphlen + sizeof(struct icmphdr), size - sizeof(struct icmphdr) - iphlen);
        printf("--------------------------------------------------------\n");
        break;
    } 

    case 2:{}
        break;

    case 6:{
        printf("\n-----------------------TCP PACKET----------------------\n");
        printf("\n");
        print_ip_header(ip);
        printf("\n");
        print_raw_data(ipoffset, iphlen);
        printf("\n");
        struct tcphdr *tcp = (struct tcphdr *)(ipoffset + iphlen);
        print_tcp_header(tcp);
        printf("\n");
        print_raw_data(ipoffset + iphlen, tcp->doff * 4);
        printf("\nData Payload\n");
        print_raw_data(ipoffset + iphlen + tcp->doff * 4, size - tcp->doff * 4 - iphlen);
        printf("--------------------------------------------------------\n");
        break;
    } //TCP
        

    case 17:{
        printf("\n-----------------------UDP PACKET----------------------\n");
        printf("\n");
        print_ip_header(ip);
        printf("\n");
        print_raw_data(ipoffset, iphlen);
        printf("\n");
        struct udphdr *udp = (struct udphdr *)(ipoffset + iphlen);
        print_udp_header(udp);
        printf("\n");
        print_raw_data(ipoffset + iphlen, sizeof(struct udphdr));
        printf("\nData Payload\n");
        print_raw_data(ipoffset + iphlen + sizeof(struct udphdr), size - sizeof(struct udphdr) - iphlen);
        printf("--------------------------------------------------------\n");
        break;
    } //UDP
        

    default:{}
        break;
    }
}

void print_raw_data(unsigned char *data, int size)
{
    for (int i = 0; i < size; i++)
    {
        //HET 1 HANG
        if (i != 0 && i % 16 == 0)
        {
            printf("\t\t");
            for (int j = i - 16; j < i; j++)
            {
                //KI TU CO NGHIA
                if (data[j] >= 32 && data[j] <= 128)
                    printf("%c", (unsigned char)data[j]);

                //KI TU KHONG CO NGHIA
                else
                    printf(".");
            }
            printf("\n");
        }

        if (i % 16 == 0)
            printf("\t");
        printf(" %02X", (unsigned int)data[i]);

        if (i == size - 1) //print the last spaces
        {
            for (int j = 0; j < 15 - i % 16; j++)
                printf("   "); //extra spaces

            printf("\t\t");

            for (int j = i - i % 16; j <= i; j++)
            {
                if (data[j] >= 32 && data[j] <= 128)
                    printf("%c", (unsigned char)data[j]);
                else
                    printf(".");
            }
            printf("\n");
        }
    }
}

bool IsLocked(const char *type, const char *ip)
{
    char cmd[128] = "iptables -L ";
    char buffer[128];
    strcat(cmd, type);
    strcat(cmd, " -v -n | grep \"");
    strcat(cmd, ip);
    strcat(cmd, "\"");
    // printf("%s\n", cmd);
    FILE *pipe = popen(cmd, "r");
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    try
    {
        while (!feof(pipe))
        {
            if (fgets(buffer, 128, pipe))
            {
                // printf("KET QUA: %s\n", buffer);
                // result = new char[strlen(buffer) + 1];
                // strcpy(result, buffer);
                return true;
            }
            return false;
        }
    }
    catch (...)
    {
        pclose(pipe);
        printf("ERROR");
        throw;
    }
    pclose(pipe);
}

bool Lock(const char *ip)
{
    bool flag= false;
    if (!IsLocked("INPUT", ip))
    {
        char cmd[128] = "iptables -I INPUT -s ";
        strcat(cmd, ip);
        strcat(cmd, " -j DROP");
        system(cmd);
        flag= true;
    }
    if (!IsLocked("OUTPUT", ip))
    {
        char cmd[128] = "iptables -I OUTPUT -s ";
        strcat(cmd, ip);
        strcat(cmd, " -j DROP");
        system(cmd);
        flag= true;
    }
    if (!IsLocked("FORWARD", ip))
    {
        char cmd[128] = "iptables -I FORWARD -s ";
        strcat(cmd, ip);
        strcat(cmd, " -j DROP");
        system(cmd);
        flag= true;
    }
    return flag;
}

bool Unlock(const char *ip)
{
    bool flag = false;
    if (IsLocked("INPUT", ip))
    {
        char cmd[128] = "iptables -D INPUT -s ";
        strcat(cmd, ip);
        strcat(cmd, " -j DROP");
        system(cmd);
        flag= true;
    }
    if (IsLocked("OUTPUT", ip))
    {
        char cmd[128] = "iptables -D OUTPUT -s ";
        strcat(cmd, ip);
        strcat(cmd, " -j DROP");
        system(cmd);
        flag= true;
    }
    if (IsLocked("FORWARD", ip))
    {
        char cmd[128] = "iptables -D FORWARD -s ";
        strcat(cmd, ip);
        strcat(cmd, " -j DROP");
        system(cmd);
        flag= true;
    }
    return flag;
}