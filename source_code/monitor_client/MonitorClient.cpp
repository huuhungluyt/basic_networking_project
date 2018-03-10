#include "MonitorClient.h"

MonitorClient *MonitorClient::instance = NULL;

MonitorClient::MonitorClient()
{
    cliSoc = 0;
    devs = NULL;
    controller = 0;
    capture = 0;
    filterStr = NULL;
}

MonitorClient::~MonitorClient()
{
    pcap_freealldevs(this->devs);
    if (filterStr)
    {
        delete[] filterStr;
        filterStr = NULL;
    }
}

MonitorClient *MonitorClient::GetInstance()
{
    if (!instance)
    {
        instance = new MonitorClient();
    }
    return instance;
}

void MonitorClient::DestroyInstance()
{
    if (instance)
    {
        delete instance;
        instance = NULL;
    }
}

int MonitorClient::Connect(const char *host, int port)
{
    struct sockaddr_in ser_sock;
    struct hostent *server;

    if ((this->cliSoc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("[ERROR] on creating socket ");
        return 0;
    }

    if ((server = gethostbyname((char *)host)) == NULL)
    {
        perror("[ERROR] no such host ");
        return 0;
    }

    bzero((char *)&ser_sock, sizeof(ser_sock));
    ser_sock.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&(ser_sock.sin_addr.s_addr), server->h_length);
    ser_sock.sin_port = htons(port);

    if (connect(this->cliSoc, (struct sockaddr *)&ser_sock, sizeof(ser_sock)) < 0)
    {
        perror("[ERROR] on connecting host ");
        return 0;
    }
    return this->cliSoc;
}

void MonitorClient::SetFilterStr(const char *filterStr)
{
    if (this->filterStr)
    {
        delete[] this->filterStr;
        this->filterStr = NULL;
    }
    this->filterStr = new char[strlen(filterStr) + 1];
    strcpy(this->filterStr, filterStr);
}

char *MonitorClient::GetFilterStr()
{
    return this->filterStr;
}

void MonitorClient::SetDevIndex(int devIndex)
{
    this->devIndex = devIndex;
}

int MonitorClient::GetDevIndex()
{
    return this->devIndex;
}

pcap_if_t *MonitorClient::GetDevs()
{
    return this->devs;
}

int MonitorClient::Send(const char *buffer, const int size)
{
    if (this->cliSoc)
    {
        return write(this->cliSoc, buffer, size);
    }
    return -1;
}

int MonitorClient::Receive(char *buffer, const int size)
{
    if (this->cliSoc)
    {
        memset(buffer, 0, size);
        return read(this->cliSoc, buffer, size);
    }
    return -1;
}

int MonitorClient::GetIfaceNames(char *ifaceNames)
{
    char errbuff[ERR_SIZE];
    pcap_if_t *dev;
    pcap_freealldevs(this->devs);

    if (pcap_findalldevs(&(this->devs), errbuff) < 0)
    {
        fprintf(stderr, "%s\n", errbuff);
        return 0;
    }

    int count = 0;
    for (dev = this->devs; dev; dev = dev->next)
    {
        count++;
        // printf("%d. %s\n", count, dev->name);
        if (count != 1)
            strcat(ifaceNames, ",");
        strcat(ifaceNames, dev->name);
    }

    return count;
}

void MonitorClient::StartController()
{
    printf("Start controller !\n");
    pthread_create(&(this->controller), NULL, &controller_thread, NULL);
    pthread_join(this->controller, NULL);
}

void MonitorClient::StopController(){
    printf("Stop controller !\n");
    pthread_cancel(this->capture);
    pthread_cancel(this->controller);
}

void MonitorClient::StartCapture(int devIndex, const char *filterStr)
{
    printf("Start capture !\n");
    pcap_if_t *dev;
    if (this->devs != NULL)
    {
        int i = 0;
        for (dev = this->devs; (i < devIndex) && dev; dev = dev->next, i++)
            ;
        if (i == devIndex)
        {
            CLIENT->SetDevIndex(devIndex);
            CLIENT->SetFilterStr(filterStr);

            pthread_create(&(this->capture), NULL, &capture_thread, NULL);
        }
    }
}

void MonitorClient::StopCapture()
{
    printf("Stop capture !\n");
    pthread_cancel(this->capture);
}

int MonitorClient::GetSock()
{
    return this->cliSoc;
}

void MonitorClient::Close()
{
    close(this->cliSoc);
}

void *controller_thread(void *par)
{
    char readbuffer[BUFF_SIZE];
    char writebuffer[SMS_SIZE];
    int cliSoc = CLIENT->GetSock();
    while (1)
    {
        if (CLIENT->Receive(readbuffer, sizeof(readbuffer)) < 0)
        {
            perror("[ERROR] Read from server fail !");
            continue;
        }
        char *check = strtok(readbuffer, ":");

        //Yeu cau danh sach cac network interfaces;
        if (strcmp(check, "iface") == 0)
        {
            memset(writebuffer, 0, SMS_SIZE);
            strcpy(writebuffer, "iface:");
            CLIENT->GetIfaceNames(writebuffer);
            if (CLIENT->Send(writebuffer, strlen(writebuffer)) < 0)
            {
                perror("[ERROR] Write to server fail !");
                continue;
            }
        }
        else if (strcmp(check, "stop") == 0)
        {
            CLIENT->StopCapture();
        }
        else if (strcmp(check, "start") == 0)
        {
            int devIndex = atoi(strtok(strtok(NULL, ":"), ","));
            char *filterStr = strtok(NULL, ",");
            CLIENT->StartCapture(devIndex, filterStr);
            // printf("%d\n", devIndex);
        }
        else if (strcmp(check, "lock") == 0)
        {
            char *lockIp = strtok(NULL, ":");
            memset(writebuffer, 0, SMS_SIZE);
            if (Lock(lockIp))
            {
                strcpy(writebuffer, "lock:ok");
                if (CLIENT->Send(writebuffer, strlen(writebuffer)) < 0)
                {
                    perror("[ERROR] Write to server fail !");
                    continue;
                }
            }else{
                strcpy(writebuffer, "lock:fail");
                if (CLIENT->Send(writebuffer, strlen(writebuffer)) < 0)
                {
                    perror("[ERROR] Write to server fail !");
                    continue;
                }
            }
        }else if(strcmp(check, "unlock") == 0){
            char *unlockIp = strtok(NULL, ":");
            memset(writebuffer, 0, SMS_SIZE);
            if (Unlock(unlockIp))
            {
                strcpy(writebuffer, "unlock:ok");
                if (CLIENT->Send(writebuffer, strlen(writebuffer)) < 0)
                {
                    perror("[ERROR] Write to server fail !");
                    continue;
                }
            }else{
                strcpy(writebuffer, "unlock:fail");
                if (CLIENT->Send(writebuffer, strlen(writebuffer)) < 0)
                {
                    perror("[ERROR] Write to server fail !");
                    continue;
                }
            }
        }
    }
}

void *capture_thread(void *par)
{
    char errbuff[ERR_SIZE];
    int devIndex = CLIENT->GetDevIndex();
    const char *filterStr = CLIENT->GetFilterStr();
    int cliSoc = MonitorClient::GetInstance()->GetSock();

    printf("%d\n", devIndex);
    pcap_if_t *dev = NULL;
    pcap_t *descr;
    struct bpf_program fp;
    bpf_u_int32 maskp; /* subnet mask */
    bpf_u_int32 netp;  /* ip */
    int i;

    for (i = 0, dev = CLIENT->GetDevs(); (i < devIndex) && dev; dev = dev->next, i++)
        ;

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
    pcap_loop(descr, -1, capture_callback, NULL);
}

void capture_callback(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    int cliSoc = MonitorClient::GetInstance()->GetSock();
    u_char *temp = (u_char *)packet;
    struct iphdr *iph = (struct iphdr *)(temp + 14);
    // print_packet_headers(temp + 14, ntohs(iph->tot_len));
    write(cliSoc, temp, ntohs(iph->tot_len) + 14);
}