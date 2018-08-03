#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

int main(int ac, char **av) 
{
    uint8_t bytes[6];
    char  bluead[32]; 
    char   name[512];
    int    values[6];
    int   timeout = atoi(av[2]); 
    int     sock; 
    int        i;
    bdaddr_t adr;

    if(ac < 2)
    {
        return 1;
    }
    if( 6 == sscanf( av[1], "%x:%x:%x:%x:%x:%x%*c",
                            &values[0], &values[1], &values[2],
                            &values[3], &values[4], &values[5] ) )
    {
        for( i = 0; i < 6; ++i )
            bytes[i] = (uint8_t) values[i];
    }

    else
    {
        printf("Doesn't look like a valid address?\n");
    }

    printf("%x:%x:%x:%x:%x:%x\n",bytes[0],bytes[1],bytes[2],
                                 bytes[3],bytes[4],bytes[5]);
    --bytes[5];
    printf("%x:%x:%x:%x:%x:%x\n",bytes[0],bytes[1],bytes[2],
                                 bytes[3],bytes[4],bytes[5]);

    sprintf(bluead,"%x:%x:%x:%x:%x:%x",bytes[0],bytes[1],bytes[2],
                                       bytes[3],bytes[4],bytes[5]);
 
    sock = hci_open_dev(0);
    if (sock < 0)
    {
        printf("bluetooth failed opening adaptor\n");
        exit(1);
    }

    str2ba(bluead,&adr);
    if (hci_read_remote_name(sock, &adr, sizeof(name), name, timeout) == 0)
    {
        printf("%s\n",name);
    }
    usleep(50);
    return 0;
}
