#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <socket.h>
#include <netdb.h>

#include <Debug.h>
#include <FindDirectory.h>
#include <Path.h>
#include <InterfaceDefs.h>
#include <Message.h>
#include <OS.h>
#include <add-ons/input_server/InputServerDevice.h>
#include <SupportDefs.h>
#include <ByteOrder.h>

void _WatchPort(int recv_socket);
status_t _Connect(int *recv_socket, short _Port);
void _Disconnect(int recv_socket);
void hex_dump(char *blah, int32 length);

int main()
{
	int recv_socket;
	short _Port;
	
	// hello world!
	printf("NetInputDevice: Hello World!\n");
	
	// To listen on a specific port, make a file with this line _only_:
	// NET_INPUT_PORT = <number between 0 and 65535>
	// (ignore the < and >)
	// save it as '/boot/home/config/settings/NetInputDevice'
	// read settings
	FILE *settings_file = NULL;
	BPath path;
	
	find_directory(B_USER_SETTINGS_DIRECTORY, &path);
	path.Append("NetInputDevice"); // '/boot/home/config/settings/NetInputDevice'
	
	printf("NetInputDevice: opening the settings file...\n");
	settings_file = fopen(path.Path(), "r");
	if(settings_file)
	{
		printf("NetInputDevice: Reading settings..\n");
		fscanf(settings_file, "NET_INPUT_PORT = %d\n", &_Port);
	}
	else
		_Port = 666; // default is 666
	
	printf("NetInputDevice: closing the settings file\n");
	fclose(settings_file);
	
	// no socket yet
	recv_socket = -1;
	
	printf("NetInputDevice: Start()\n");
	_Connect(&recv_socket, _Port);
	_WatchPort(recv_socket);
}

void _WatchPort(int recv_socket)
{
	printf("NetInputDevice: Starting the listen thread\n");
	ssize_t bytes_read;
	union {
		char raw_len[4];
		size_t length;
	}; // anonymous union so we can loop through the first recv() ( paranoia :)
	char *buffer, *buffer_start;
	BMessage *event = NULL;
	status_t err = B_OK;
	sockaddr_in sa;
	int clientsize, clientsocket;
	
	while (true)
	{
		printf("NetInputDevice: Calling accept()\n");
		// block until someone connects to us
		clientsocket = accept(recv_socket, (sockaddr *)&sa, &clientsize);
		printf("NetInputDevice: accept() returned. somebody connected to us...\n");
		
		if(clientsocket < 0)
		{
			// bad juju from accept. bail like a sissy
			_Disconnect(recv_socket);
			break;
		}
		
		while(err >= B_OK && clientsocket >= 0)
		{
			// Block until we find the size of the next message
			printf("NetInputDevice: Calling recv()\n");
			
			bytes_read = 0;
			err = 1; // read loop won't run if this is 0 (B_OK)
			// ultra-paranoid read loop
			// read a long int off the net ( twisted. I know. )
			// (can recv() ever read less than 4 bytes?)
			while((err > 0) && (bytes_read < 4))
			{
				err = recv(clientsocket, &raw_len[bytes_read], (4 - bytes_read), 0);  // grab the data!
				if(err > 0)
				{
					bytes_read += err; // add to the bytes_read counter
				}
			}
			//err = recv(clientsocket, &length, 4, 0); // read a long int off the net
			
			printf("NetInputDevice: recv() returned. wonder if we got anything...\n");
			if(err > 0)
			{
				//sprintf(dbg_msg_buff, "NetInputDevice: length before swap = %d", length);
				//VelvetJones(dbg_msg_buff);
				//memset(dbg_msg_buff, 0, 255);
							
				length = B_BENDIAN_TO_HOST_INT32(length); // byteswap from network to host
				
				printf("NetInputDevice: length after swap = %d\n", length);
				
				buffer_start = buffer = (char*)malloc(length);
				//buffer_start = buffer; // cache a pointer to the first byte
				bytes_read = 0;
				err = 1; // read loop won't run if this is 0 (B_OK)
				//event = NULL;
				event = new BMessage();
				
				printf("NetInputDevice: addr of buffer after malloc = %p\n", buffer);
			
				// read loop
				while((err > 0) && (bytes_read < length))
				{
					err = recv(clientsocket, buffer_start, (length - bytes_read), 0);  // grab the data!
					if(err > 0)
					{
						bytes_read += err; // add to the bytes_read counter
						buffer_start += err; // advance the pointer
					}
				}
				
				//PrincessDeluxe(buffer, length); // remote PrintToStream()
				
				//buffer = buffer_start; // restore the pointer to the first byte
				buffer_start = NULL; // un-point buffer_start
				
				if(err < 0)
				{
					if(bytes_read >= 0)
					{
						printf("NetInputDevice: failed to read full packet\n");
					}
					else
					{
						printf("NetInputDevice: recv error\n");
					}
				}
				else if ((err = event->Unflatten(buffer)) < 0)
				{
					hex_dump(buffer,length);
					//event->PrintToStream(); // fuck errors!
					printf("NetInputDevice: event->Unflatten(buffer) error (bad packet): %s\n", strerror(err));
					err = B_OK; // don't want this error to kill our connection
				}
				else
				{
					printf("NetInputDevice: successfully grabbed a packet. Enqueuing...\n");
					
					// add the "when" (removed before sending across net)
					event->AddInt64("when", system_time());
					
					event->PrintToStream();
					//EnqueueMessage(event);	
					//event = NULL;
				}
				
				printf("NetInputDevice: freeing the message buffer\n");
				printf("NetInputDevice: addr of buffer before free = %p\n", buffer);
				
				free(buffer);
				buffer = NULL;
				printf("NetInputDevice: message buffer freed\n");
				
				if(event != NULL)
				{
					printf("NetInputDevice: deleting the event message\n");
					delete(event);
					event = NULL;
				}
				else
				{
					printf("NetInputDevice: no need to delete event message. looping...\n");
				}
			}
			else
			{
				printf("NetInputDevice: nope. killing the client socket. error: %s\n", strerror(errno));
				if(clientsocket > 0)
					closesocket(clientsocket); // clean up the client socket
				clientsocket = -1;
			}
		}
		if(clientsocket > 0)
			closesocket(clientsocket); // clean up the client socket
		clientsocket = -1;
	}
}

status_t _Connect(int *recv_socket, short _Port)
{
	sockaddr_in sa;
	
	// standard socket/bind/listen
	// die like a, err, hmmm. <insert witticism> if things don't work out
	
	// make a socket
	*recv_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(*recv_socket < 0)
		return B_ERROR;
	
	sa.sin_family = AF_INET;
	sa.sin_port = htons(_Port);
	sa.sin_addr.s_addr = INADDR_ANY;
	memset(sa.sin_zero, 0, sizeof(sa.sin_zero));
	
	// bind it
	if(bind(*recv_socket, (sockaddr *)&sa, sizeof(sa)) < 0)
		return B_ERROR;
	
	// we only want one connection
	if(listen(*recv_socket, 1) < 0)
		return B_ERROR;
	
	printf("NetInputDevice: socket is bound and listening\n");
	
	
	// woohoo everything's cool
	return B_OK;
}

void _Disconnect(int recv_socket)
{
	closesocket(recv_socket);
	recv_socket = -1;
}

void hex_dump(char *blah, int32 length)
{
	for(int32 i=0; i<length; i++)
	{
		printf("%c(%x) ", *(blah+i), *(blah+i));
		if(!(i % 25))
			printf("\n");
	}
	printf("\n");
}