/* -*-C-*- tcpprobe.c */
/* tcpprobe - report on which tcp ports accept connections */
/* IO ERROR, error@axs.net, Sep 15, 1995 */

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>
#include <signal.h>

int main(int argc, char **argv)
{
	//int probeport = 0;
	struct hostent *host;
	int err, i, net;
	struct sockaddr_in sa;

	if (argc != 2)
	{
		printf("Usage: %s hostname\n", argv[0]);
		exit(1);
	}

	for (i = 1; i <= 65535; i++)
	{
		memset((void *)&sa, 0, sizeof(sa)); // clear it for another go round
		sa.sin_family = AF_INET;
		if(isdigit(*argv[1]))
		{
			// its already a straight ip addr. just plug it into the struct
			sa.sin_addr.s_addr = inet_addr(argv[1]);
		}
		else if((host = gethostbyname(argv[1])) != 0)
		{
			strncpy((char *)&sa.sin_addr, (char *)host->h_addr, sizeof(sa.sin_addr));
		}
		else
		{
			// gethostbyname failed
			//herror(argv[1]);
			//exit(2);
			printf("Unable to look up host %s\n", argv[1]);
			return;
		}
		sa.sin_port = htons(i); // which port
		//printf("Opening socket on port %d\r", i);
		//fflush(stdout);
		net = socket(AF_INET, SOCK_STREAM/*SOCK_DGRAM*/, IPPROTO_TCP);
		if(net < 0)
		{
			printf("\nsocket() failed on port %d", i);
			fflush(stdout);
			closesocket(net);
			continue;
		}
		err = connect(net, (struct sockaddr *)&sa, sizeof(sa));
		if(err < B_NO_ERROR)
		{
			//printf("%s %-5d %s                    \r", argv[1], i, strerror(errno));
			//fflush(stdout);
		}
		else
		{
			printf("%s %-5d accepted.                      \n", argv[1], i);
		}
		//printf("Closing socket on port %d\r", i);
		closesocket(net);
	}
	printf("\n");
	fflush(stdout);
	return (0);
}