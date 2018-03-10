#include "lcapture.h"

int GetIfaceNames(pcap_if_t **devs, char *ifaceNames)
{
    strcat(ifaceNames, "iface:");
    char errbuff[ERR_BUFF];
    pcap_if_t *dev;
    pcap_freealldevs(*devs);

    if (pcap_findalldevs(devs, errbuff) < 0)
    {
        fprintf(stderr, "%s\n", errbuff);
        return 0;
    }

    int count = 0;
    for (dev = *devs; dev; dev = dev->next)
    {
        count++;
        printf("%d. %s\n", count, dev->name);
        if(count!=1) strcat(ifaceNames, ",");
        strcat(ifaceNames, dev->name);
    }

    return count;
}


void *CapturePacket(void *par)
{
    CaptureInfo captureInfo= *((CaptureInfo*)par);
    pcap_if_t *devs= captureInfo.devs;
    int devIndex= captureInfo.devIndex;
    char *filterStr= captureInfo.filterStr;
    int cliSoc= captureInfo.cliSoc;

    pcap_if_t *dev;
    pcap_t *descr;
    char errbuff[ERR_BUFF];
    struct bpf_program fp;
    bpf_u_int32 maskp; /* subnet mask */
    bpf_u_int32 netp;  /* ip */
    int i;


    for (i = 0, dev = devs; i < devIndex - 1; i++, dev = dev->next);

    pcap_lookupnet(dev->name, &netp, &maskp, errbuff);

    // Kich hoat device de bat goi tin
    descr = pcap_open_live(dev->name, BUFSIZ, 1, -1, errbuff);

    if (descr == NULL)
    {
        printf("pcap_open_live(): %s\n", errbuff);
        return NULL;
    }

    // Dich bo loc
    if (pcap_compile(descr, &fp, filterStr, 0, netp) == -1)
    {
        fprintf(stderr, "Error calling pcap_compile\n");
        return NULL;
    }

    // Gan bo loc cho device
    if (pcap_setfilter(descr, &fp) == -1)
    {
        fprintf(stderr, "Error setting filter\n");
        return NULL;
    }

    // Bat goi tin
    pcap_loop(descr, -1, CaptureHandler, (u_char*)&cliSoc);
}


void CaptureHandler(u_char *par, const struct pcap_pkthdr *pkthdr, const u_char *packet){
    int cliSoc= *((int *)par);
    u_char *buffer = (u_char *)packet;
    struct iphdr *iph = (struct iphdr *)(buffer + 14);
    print_packet_headers(buffer + 14, ntohs(iph->tot_len));
    write(cliSoc, buffer, ntohs(iph->tot_len) + 14);
}