#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>  
#include <signal.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

int main(int ac , char *av[])
{
   int socket_desc, c, client_sock, channel;
   struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 }; 
   char client_addr[24]; 
   char *src; 

   if(ac  < 2)
   {
       printf("[%s] <bd_addr>\n",av[0]);
       exit(1); 
   }
 
   src  = av[1]; 
   channel = 13; 

   socket_desc = socket(AF_BLUETOOTH , SOCK_STREAM , BTPROTO_RFCOMM);
   if (socket_desc == -1)
   {
       printf("Could not create socket\n");
       exit(1); 
   }

   loc_addr.rc_family = AF_BLUETOOTH; 
   str2ba(src, &loc_addr.rc_bdaddr);
   loc_addr.rc_channel = (uint8_t) channel; 

   if (bind(socket_desc,(struct sockaddr *)&loc_addr , sizeof(loc_addr)) < 0)
   {
       printf("Bind failed. Error");
       return 1;
   }

   listen(socket_desc , 3);
   printf("[+] Waiting for incoming connections...\n");

   c = sizeof(struct sockaddr_rc);
   while(1)
   {
        client_sock = accept(socket_desc, (struct sockaddr *)&rem_addr, (socklen_t*)&c);
        ba2str( &rem_addr.rc_bdaddr, client_addr);
        printf("[+] Accepted connection from %s\n",client_addr);
   }
   return 0;
}

