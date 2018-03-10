#include "MonitorClient.h"
int main(int argc, char **argv)
{
    char buffer[SMS_SIZE];
    CLIENT->Connect(argv[1], atoi(argv[2]));
    strcpy(buffer, "iface:");

    if(CLIENT->GetIfaceNames(buffer)<1){
        printf("[ERROR] No interfaces !\n");
        CLIENT->Close();
        return 0;
    }
    if(CLIENT->Send(buffer, strlen(buffer))<0){
        printf("[ERROR] Can not send !\n");
        CLIENT->Close();
        return 0;
    }
    CLIENT->StartController();
    CLIENT->StopController();
    CLIENT->Close();
    MonitorClient::DestroyInstance();
    return 0;
}