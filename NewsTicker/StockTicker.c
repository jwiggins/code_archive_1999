#include <stdio.h>                 /* Our header for standard input and output */ 
#include <string.h>                /* Our header for manipulating strings */ 
#include <net/socket.h>        /* For using sockets under Be */ 
#include <net/netdb.h>          /* Name services for sockets (IP to Host and vice versa) */  

/* Function prototype */ 
void stock_symbol_lookup(char *stocksymbol); 
bool has_tcp_data_come_in(int socket, int seconds); 

/* The first function our program calls */          
int main(int argc, char *argv[]) 
{ 
	printf("BeOS Stock Ticker v0.2\n"); 
	printf("Copyright (c) 1999 Dennis Cox. All rights reserved\n"); 
	printf("email: dennis@durrow.com    http://www.durrow.com\n"); 

	/* We won't process any arguments until later on - we will assume "CSCO" as the stock symbol to lookup */ 
	stock_symbol_lookup("CSCO"); 

	return 0; 
} 

/* Our function that looks up and returns stock information on a passed string - however for right now we'll ignore what is passed to us */ 
void stock_symbol_lookup(char *stocksymbol) 
{ 
	struct sockaddr_in sa;  /* Socket address */ 
   
	int tcp_socket; /* Socket itself */ 
   
	long host_addr; /* We need this to figure out the IP of webservices.pcquote.com */ 
	struct hostent *theHost;   

	char buffer[256];  /* The buffer we will store the retrieve information */ 

	int status; /* just a general purpose variable */ 
   
	/* Create a stream TCP socket. */ 
	if((tcp_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)  /* If you used SOCK_DGRAM instead of SOCK_STREAM you would be using the UDP Protocol */ 
	{ 
		printf("error: failed to create socket\n"); 
		return; 
	} 

	/* Set the address format for the imminent bind. */ 
	sa.sin_family = AF_INET; 

	/* We'll choose an arbitrary port number translated to network byte order. */ 
	sa.sin_port = htons(80);  /* 80 is the port number of HTTP or in layman's terms a web server */ 

	/* Lookup our host */ 
	//theHost = gethostbyname("webservices.pcquote.com");
	theHost = gethostbyname("www.beoscentral.com");

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
	//strcpy(buffer,"GET /cgi-bin/excelget.exe?TICKER=CSCO HTTP/1.0\r\n\r\n");    /* basically the url "\r\n\r\n" means end of http packet */ 
	
	/* HTTP 1.1 wants an endline then Host: <host> \r\n\r\n . Not much different than 1.0*/
	strcpy(buffer,"GET /powerbosc.txt HTTP/1.1\nHost: www.beoscentral.com\r\n\r\n");
	
	/* Send our request */ 
	if((send(tcp_socket,buffer,((strlen(buffer))+1),0))<0) 
	{ 
		printf("error: failed to send\n"); 
		closesocket(tcp_socket); 
		return; 
	} 
	do 
	{ 
		status = has_tcp_data_come_in(tcp_socket,5);  /* let's wait up to 5 seconds to see if we have any incoming data */ 

		if(status > 0) 
		{ 
			status = recv(tcp_socket, buffer, 255,0);  /* grab the data! */ 

			printf("%s",buffer);  /* print out what we got */ 
		} 
	} while(status > 0 );
	printf("\n");

	closesocket(tcp_socket);  /* we no longer need our socket - so let's be nice and close it */ 
} 

/* Our select function - stolen right from the BeBook */ 
bool has_tcp_data_come_in(int socket, int seconds) 
{ 
	struct timeval tv; 
	struct fd_set fds; 
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