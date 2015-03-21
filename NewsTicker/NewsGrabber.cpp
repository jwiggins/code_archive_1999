#include "NewsGrabber.h"

NewsGrabber::NewsGrabber()
: BLooper()
{
}

void NewsGrabber::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
		default:
		{
			BLooper::MessageReceived(msg);
			break;
		}
	}
}

void NewsGrabber::GrabNews(const char *SiteHost, const char *URI, char *buffer)
{
	sockaddr_in sa; /* Socket address */

	int tcp_socket; /* Socket itself */
   
	long host_addr; /* We need this to figure out the IP of webservices.pcquote.com */
	hostent *theHost;

	char buffer[256]; /* The buffer we will store the retrieve information */

	int status; /* just a general purpose variable */

	/* Create a stream TCP socket. */
	if((tcp_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	/* If you used SOCK_DGRAM instead of SOCK_STREAM you would be using the UDP Protocol */
	{
		printf("error: failed to create socket\n");
		return;
	}

	/* Set the address format for the imminent bind. */
	sa.sin_family = AF_INET;

	/* We'll choose an arbitrary port number translated to network byte order. */
	sa.sin_port = htons(80);
	/* 80 is the port number of HTTP or in layman's terms a web server */

	/* Lookup our host */
	//theHost = gethostbyname("webservices.pcquote.com");
	theHost = gethostbyname(SiteHost);

	sa.sin_addr.s_addr = *(ulong *)theHost->h_addr;

	/* Clear sin_zero. */
	memset(sa.sin_zero, 0, sizeof(sa.sin_zero));

	/* connect to the web server */
	status = connect(tcp_socket, (struct sockaddr *)&sa, sizeof(sa));

	if(status < 0)
	{
		printf("error: failed to connect\n");
		closesocket(tcp_socket);

		printf("%s\n",strerror(status));
		return;
	}

	/* Build our request */
	/* basically the url "\r\n\r\n" means end of http packet */ 
	//strcpy(buffer,"GET /cgi-bin/excelget.exe?TICKER=CSCO HTTP/1.0\r\n\r\n");

	/* HTTP 1.1 wants an endline then Host: <host> \r\n\r\n . Not much different than 1.0*/
	sprintf(buffer,"GET %s HTTP/1.1\nHost: %s\r\n\r\n", URI, SiteHost);

	/* Send our request */
	if((send(tcp_socket,buffer,((strlen(buffer))+1),0))<0)
	{
		printf("error: failed to send\n");
		closesocket(tcp_socket);
		return;
	}
	do
	{
		status = has_tcp_data_come_in(tcp_socket,5);
		/* let's wait up to 5 seconds to see if we have any incoming data */
		if(status > 0)
		{
			status = recv(tcp_socket, buffer, 255,0);  /* grab the data! */

			printf("%s",buffer);  /* print out what we got */
		}
	} while(status > 0 );
	printf("\n");

	/* we no longer need our socket - so let's be nice and close it */
	closesocket(tcp_socket);
}

bool NewsGrabber::has_tcp_data_come_in(int socket, int seconds)
{
	timeval tv;
	fd_set fds;
	int n;

	tv.tv_sec = seconds;
	tv.tv_usec = 0;

	/* Initialize (clear) the socket mask. */
	FD_ZERO(&fds);

	/* Set the socket in the mask. */
	FD_SET(socket, &fds);
	select(socket + 1, &fds, NULL, NULL, &tv);

	/* If the socket is still set, then it's ready to read. */
	return FD_ISSET(socket, &fds);
}

void	 NewsGrabber::do_error(const char *error)
{
	(new BAlert("error", error, "ok"))->Go(NULL);
}