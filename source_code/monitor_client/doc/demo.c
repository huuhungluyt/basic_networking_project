#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

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

int main(int argc, char **argv)
{
    if (strcmp(argv[argc-2], "lock") == 0)
    {
        if (Lock(argv[argc-1]))
            printf("Ip is locked !");
        else
            printf("Ip in black list !");
    }
    else
    {
        if (Unlock(argv[argc-1]))
            printf("Ip is unlocked !");
        else
            printf("Ip not in black list !");
    }
    return 1;
}