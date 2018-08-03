#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#define BUFSIZE 65535

#define COLNRM  "\x1B[0m"
#define COLRED  "\x1B[31m"
#define COLGRN  "\x1B[32m"
#define COLYEL  "\x1B[33m"
#define COLBLU  "\x1B[34m"
#define COLMAG  "\x1B[35m"
#define COLCYN  "\x1B[36m"
#define COLWHT  "\x1B[37m"

void error(char *msg) 
{
    perror(msg);
    exit(0);
}

int main(int ac, char **av) 
{
    int sockfd, channel, n, len, gen;
    struct sockaddr_rc addr = { 0 };
    char buf[BUFSIZE];
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
        error("ERROR opening socket");

    addr.rc_family  = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t)channel; 
    str2ba( dest, &addr.rc_bdaddr );

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) 
          error("ERROR connecting");

    while(1)
    {
 start:
        bzero(buf, BUFSIZE);
        printf("[%sBLU%sSH%s @ %s%s%s :~]$ ",COLBLU,COLRED,COLNRM,COLWHT,dest,COLNRM);
        fgets(buf, BUFSIZE, stdin);
        if(strlen(buf) <2)
        {
            goto start; 
        }

        n = write(sockfd, buf, strlen(buf));
        if (n < 0) 
           error("ERROR writing to socket");

        sleep(1); 
        len = 0;
        gen = 0;
again:
        ioctl(sockfd, FIONREAD, &len);
        if(len > 0)        
        {
             gen++;
             n = read(sockfd, buf, len);
             if (n < 0) 
             {
                 error("uh oh\n");
             }
             printf("%s", buf);
             bzero(buf, BUFSIZE);
             goto again; 
        }
        if(gen == 0)
        {
            goto again;
        }
    }
    close(sockfd);
    return 0;
}
