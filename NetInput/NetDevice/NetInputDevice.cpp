#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

extern "C" _EXPORT BInputServerDevice* instantiate_input_device();

class NetInputDevice;

class NetInputDevice : public BInputServerDevice {
public:
	NetInputDevice();
	virtual ~NetInputDevice();

	virtual status_t Start(const char *device, void *);
	virtual	status_t Stop(const char *, void *);

private:
	thread_id _Thread;
	static int32 _StartWatchPort(void *arg);
	void _WatchPort();
	unsigned short _Port;
	int recv_socket;
	
	// binds a socket on a local port and then fires up the _WatchPort() loop thread
	static int32 FireAndForgetNetConnect(void *arg); 
	
	status_t _Connect(); // does the actual "firing up" of _WatchPort()
	void _Disconnect();
	
	void debug_output(const char *);
};


BInputServerDevice* instantiate_input_device()
{
	return (new NetInputDevice());
}


NetInputDevice::NetInputDevice()
	: BInputServerDevice()
{
	// we are a keyboard and a mouse :)
	// we can get away with one device though, _plus_ it makes life easier
	input_device_ref *device[2];
	input_device_ref mover = {"NetMouseDevice", B_POINTING_DEVICE, NULL};
	device[0] = &mover;
	device[1] = NULL;
	
	// hello world!
	debug_output("NetInputDevice: Hello World!");
	
	// To listen on a specific port, make a file with this line _only_:
	// NET_INPUT_PORT = <number between 0 and 65535>
	// (ignore the < and >)
	// save it as '/boot/home/config/settings/NetInputDevice'
	// read settings
	FILE *settings_file = NULL;
	BPath path;
	
	find_directory(B_USER_SETTINGS_DIRECTORY, &path);
	path.Append("NetInputDevice"); // '/boot/home/config/settings/NetInputDevice'
	
	debug_output("NetInputDevice: opening the settings file...");
	settings_file = fopen(path.Path(), "r");
	if(settings_file)
	{
		debug_output("NetInputDevice: Reading settings..");
		fscanf(settings_file, "NET_INPUT_PORT = %d\n", &_Port);
	}
	else
		_Port = 666; // default is 666
	
	debug_output("NetInputDevice: closing the settings file");
	fclose(settings_file);
	
	// no socket yet
	recv_socket = -1;
	
	debug_output("NetInputDevice: Registering with the input server.");
	RegisterDevices(device);
}


NetInputDevice::~NetInputDevice()
{
	// nada
}


status_t NetInputDevice::Start(const char *device, void *)
{	
	debug_output("NetInputDevice: Start()");
	
	// only spawn here. don't resume. The fire and forget thread below will
	// take care of that 
	_Thread = spawn_thread(_StartWatchPort, device, B_REAL_TIME_DISPLAY_PRIORITY+4, this);
	
	// fire and forget thread that waits for the net_server to poke its
	// head up, then binds a socket and resum_thread()'s the listen thread
	resume_thread(spawn_thread(FireAndForgetNetConnect, "wait_for_net", B_NORMAL_PRIORITY, this));
	
	return (B_NO_ERROR);
}


status_t NetInputDevice::Stop(const char *, void *)
{
	kill_thread(_Thread);
	_Disconnect();
	
	debug_output("NetInputDevice: Stop()");
	return (B_NO_ERROR);
}

int32 NetInputDevice::_StartWatchPort(void *arg)
{
	NetInputDevice *self = (NetInputDevice*)arg;
	self->_WatchPort();
	return (B_NO_ERROR);
}

void NetInputDevice::_WatchPort()
{
	debug_output("NetInputDevice: Starting the listen thread");
	ssize_t length, bytes_read;
	char *buffer, *buffer_start, dbg_msg_buff[255];
	BMessage *event;
	status_t err = B_OK;
	sockaddr_in sa;
	int clientsize, clientsocket;
	
	
	while (true)
	{
		
		// block until someone connects to us
		clientsocket = accept(recv_socket, (sockaddr *)&sa, &clientsize);
		debug_output("NetInputDevice: accept() returned. somebody connected to us...");
		
		if(clientsocket < 0)
		{
			// bad juju from accept. bail like a sissy
			_Disconnect();
			break;
		}
		
		while(err >= B_OK && clientsocket >= 0)
		{
			// Block until we find the size of the next message
			debug_output("NetInputDevice: Calling recv()");
			err = recv(clientsocket, &length, 4, 0); // read a long int off the net
			
			debug_output("NetInputDevice: recv() returned. wonder if we got anything...");
			if(err > 0)
			{
				//sprintf(dbg_msg_buff, "NetInputDevice: length before swap = %d", length);
				//debug_output(dbg_msg_buff);
				//memset(dbg_msg_buff, 0, 255);
							
				length = B_BENDIAN_TO_HOST_INT32(length); // byteswap from network to host
				
				sprintf(dbg_msg_buff, "NetInputDevice: length after swap = %d", length);
				debug_output(dbg_msg_buff);
				memset(dbg_msg_buff, 0, 255);
				
				buffer = (char*)malloc(length);
				buffer_start = buffer; // cache a pointer to the first byte
				bytes_read = 0;
				event = NULL;
				event = new BMessage();
				
				sprintf(dbg_msg_buff, "NetInputDevice: addr of buffer after malloc = %p", buffer);
				debug_output(dbg_msg_buff);
				memset(dbg_msg_buff, 0, 255);
			
				// read loop
				while(err > 0 && bytes_read < length)
				{
					err = recv(clientsocket, buffer, length, 0);  // grab the data!
					if(err > 0)
					{
						bytes_read += err; // add to the bytes_read counter
						buffer += err;
					}
				}
				
				buffer = buffer_start; // restore the pointer to the first byte
				buffer_start = NULL; // un-point buffer_start
				
				if(err < 0)
				{
					if(bytes_read >= 0)
					{
						debug_output("NetInputDevice: failed to read full packet");
					}
					else
					{
						debug_output("NetInputDevice: recv error");
					}
				}
				else if ((err = event->Unflatten(buffer)) < 0)
				{
					debug_output("NetInputDevice: event->Unflatten(buffer) error (bad packet)");
					debug_output(strerror(err));
					err = B_OK; // don't want this error to kill our connection
				}
				else
				{
					debug_output("NetInputDevice: successfully grabbed a packet. Enqueuing...");
					EnqueueMessage(event);	
					event = NULL;
				}
				
				debug_output("NetInputDevice: freeing the message buffer");
				sprintf(dbg_msg_buff, "NetInputDevice: addr of buffer before free = %p", buffer);
				debug_output(dbg_msg_buff);
				memset(dbg_msg_buff, 0, 255);
				free(buffer);
				buffer = NULL;
				debug_output("NetInputDevice: message buffer freed");
				
				if(event != NULL)
				{
					debug_output("NetInputDevice: deleting the event message");
					delete(event);
					event = NULL;
				}
				else
				{
					debug_output("NetInputDevice: no need to delete event message. looping...");
				}
			}
		}
		if(clientsocket > 0)
			closesocket(clientsocket); // clean up the client socket
		clientsocket = -1;
	}
}

int32 NetInputDevice::FireAndForgetNetConnect(void *arg)
{
	// wait for the net_server to start (specifically, it's socket server thread)
	while(find_thread("socket server") == B_NAME_NOT_FOUND)
		snooze(1000 * 1000); // poll once per second
		
	// allrighty! the net_server should be alive.
	((NetInputDevice *)arg)->_Connect(); // call connect
	
	return B_OK;
}

status_t NetInputDevice::_Connect()
{
	sockaddr_in sa;
	
	// standard socket/bind/listen
	// die like a, err, hmmm. <insert witticism> if things don't work out
	
	// make a socket
	recv_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(recv_socket < 0)
		return B_ERROR;
	
	sa.sin_family = AF_INET;
	sa.sin_port = htons(_Port);
	sa.sin_addr.s_addr = INADDR_ANY;
	memset(sa.sin_zero, 0, sizeof(sa.sin_zero));
	
	// bind it
	if(bind(recv_socket, (sockaddr *)&sa, sizeof(sa)) < 0)
		return B_ERROR;
	
	// we only want one connection
	if(listen(recv_socket, 1) < 0)
		return B_ERROR;
	
	debug_output("NetInputDevice: socket is bound and listening");
	
	// start the listen thread
	// we do it here to avoid a nasty race condition.
	// (cuz this thread would freak out if recv_socket wasn't listening...)
	resume_thread(_Thread);
	
	// woohoo everything's cool
	return B_OK;
}

void NetInputDevice::_Disconnect()
{
	closesocket(recv_socket);
	recv_socket = -1;
}

void NetInputDevice::debug_output(const char *blah)
{
	// very small chance that not tolerating a timeout would cause us
	// to drop a message, but don't count on it.
	// (not exactly a high traffic port, 'ol velvet... )
	write_port_etc( find_port("_VelvetJones_"), 0, blah, strlen(blah),	B_TIMEOUT, 250);
}
