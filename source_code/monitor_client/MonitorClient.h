#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <pcap.h>
#include <pthread.h>

#include "lpheader.h"

#define CLIENT (MonitorClient::GetInstance())
#define SMS_SIZE 65536
#define BUFF_SIZE 128
#define ERR_SIZE 128

typedef struct CaptureInfo
{
    int cliSoc;
    const char *devName;
    const char *filterStr;
} CaptureInfo;

class MonitorClient
{
  public:
    static MonitorClient *GetInstance();
    static void DestroyInstance();

    int GetSock();
    void SetFilterStr(const char *filterStr);
    char *GetFilterStr();
    void SetDevIndex(int devIndex);
    int GetDevIndex();
    pcap_if_t *GetDevs();

    int Connect(const char *host,const int port);
    int Send(const char *buffer, const int size);
    int Receive(char *buffer, const int size);
    void StartController();
    void StopController();
    void StartCapture(int devIndex, const char *filterStr);
    void StopCapture();
    int GetIfaceNames(char *ifaceNames);
    void Close();

    static MonitorClient *instance;
  private:
    ~MonitorClient();
    MonitorClient();
    int cliSoc;
    int devIndex;
    char *filterStr;
    pcap_if_t *devs;
    pthread_t capture;
    pthread_t controller;
};


static void *controller_thread(void *par);
static void *capture_thread(void *par);
static void capture_callback(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet);