#include "lsocket.h"
#include <pcap.h>
#include <pthread.h>

#define BUFFER_SIZE 256



void *SMSHandler(void *par)
{
    pcap_if_t *devs = 0;
    char read_buff[BUFFER_SIZE];
    char write_buff[BUFFER_SIZE];
    char err_buff;
    int cli_soc = *((int *)par);
    while (1)
    {
        memset(read_buff, 0, sizeof(read_buff));
        if (read(cli_soc, read_buff, sizeof(read_buff)) < 0)
        {
            perror("[ERROR] Read from server fail !");
            continue;
        }

        //Yeu cau danh sach cac network interfaces;
        if (strcmp(read_buff, "iface") == 0)
        {
            strcpy(write_buff, "iface:");
            printf("Num of ifaces: %d\n", GetIfaceNames(&devs, write_buff));
            if (write(cli_soc, write_buff, strlen(write_buff)) < 0)
            {
                perror("[ERROR] Write to server fail !");
                continue;
            }
        }
        else if (strcmp(read_buff, "quit"))
        {
            printf("i quit\n");
            break;
        }
    }
    pcap_freealldevs(devs);
}

int main(int argc, char **argv)
{
    int cli_soc;
    pthread_t smsHandler;
    //check hostname input and port input
    if (argc < 3)
    {
        fprintf(stderr, "[ERROR] Please enter hostname and port number !\n");
        return 1;
    }

    //Connect to server
    if ((cli_soc = ConnectServer(argv[1], atoi(argv[2]))) < 0)
    {
        fprintf(stderr, "[ERROR] Connect to server fail !\n");
        return 1;
    }
    printf("Connected to server !\n");

    pthread_create(&smsHandler, NULL, &SMSHandler, (void *)&cli_soc);
    printf("Start sms handler !\n");
    pthread_join(smsHandler, NULL);

    close(cli_soc);
    return 0;
}