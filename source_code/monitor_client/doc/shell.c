#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string.h>

char *exec(const char *cmd)
{
    char buffer[128];
    char *result = NULL;
    FILE *pipe = popen(cmd, "r");
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    try
    {
        while (!feof(pipe))
        {
            if (fgets(buffer, 128, pipe) != NULL)
            {
                printf("KET QUA: %s\n", buffer);
                result = new char[strlen(buffer) + 1];
                strcpy(result, buffer);
            }
        }
    }
    catch (...)
    {
        pclose(pipe);
        printf("ERROR");
        throw;
    }
    pclose(pipe);
    return result;
}

bool isLocked(const char *type, const char *ip)
{
    char cmd[128] = "iptables -L ";
    char buffer[128];
    strcat(cmd, type);
    strcat(cmd, " -v -n | grep \"");
    strcat(cmd, ip);
    strcat(cmd, "\"");
    printf("%s\n", cmd);
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

int main(int argc, char **argv)
{
    if (isLocked(argv[1], argv[2])) //31.13.95.36
    {
        printf("%s\n", "Locked");
    }
    else
    {
        printf("%s\n", "Unlock");
    }
    return 0;
}