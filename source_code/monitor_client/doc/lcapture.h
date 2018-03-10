#include <pcap.h>
#include <string.h>
#include "lsocket.h"
#include "lpheader.h"

#define ERR_BUFF 128
#define SMS_BUFF 65541

typedef struct CaptureInfo{
    int devIndex;
    char *filterStr;
} CaptureInfo;

int GetIfaceNames(pcap_if_t **devs, char *ifaceNames);

void *CapturePacket(void *par);

void CaptureHandler(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet);