#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

int main(int ac, char **av) 
{
    int sockfd, channel;
    struct sockaddr_rc addr = { 0 };
    char *dest;    

    if(ac < 3)
    {
        printf("[%s] <bd_addr> <service>\n",av[0]);
        exit(1); 
    }
    dest    = av[1];
    channel = atoi(av[2]);

    sockfd = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if (sockfd < 0) 
    {
        exit(1);
    }

    addr.rc_family  = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t)channel; 
    str2ba( dest, &addr.rc_bdaddr );

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) 
    {
        exit(1);
    }

    close(sockfd);
    return 0;
}
