#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>

#include <socket.h>

#include <Debug.h>
#include <List.h>
#include <Message.h>
#include <OS.h>

#include <add-ons/input_server/InputServerFilter.h>
#include <InterfaceDefs.h>

extern "C" _EXPORT BInputServerFilter* instantiate_input_filter();

enum {
	FILTER_START = 'strt',
	FILTER_STOP = 'stop',
	FILTER_LOCKOUT = 'lock'
};

class NetInputFilter : public BInputServerFilter 
{
public:
	NetInputFilter();
	virtual ~NetInputFilter();
	virtual	filter_result Filter(BMessage *message, BList *);
private:
	thread_id _WatchThread;
	static int32 _StartWatchPort(void *arg);
	void _WatchPort(void);
	
	thread_id _QueueThread;
	static int32 _StartQueuePort(void *arg);
	void _QueuePort(void);
	
	void _Connect(short,long);
	void _ConnectionDied();
	void debug_output(const char *);

	port_id _CommandPort, _NetQueuePort, _debug_port;
	int32 _InputLockMode;
	int	_Remotesocket;
	bool _currently_sending;
};

BInputServerFilter* instantiate_input_filter()
{
	return (new NetInputFilter());
}


NetInputFilter::NetInputFilter()
{
	_InputLockMode = 0;
	_Remotesocket = -1;
	_QueueThread = -1; // no queue thread yet
	_currently_sending = false;
	_debug_port = find_port("_VelvetJones_");
	
	
	debug_output("NetInputFilterPort: Hello World!");
	
	_NetQueuePort = create_port(1,"NetInputFilterPort:net"); // one message deep
	_CommandPort = create_port(100,"NetInputFilterPort:cmd");
	
	debug_output("NetInputFilterPort: ports were created.");
	
	if(_CommandPort < 0 || _NetQueuePort < 0)
	{
		debug_output("inputRecorderFilter: create_port error for the command port or the net queue port");
	}
	
	debug_output("NetInputFilterPort: spawning the command thread");
	// the command thread
	_WatchThread = spawn_thread(_StartWatchPort, "NetInputFilter:cmd", B_REAL_TIME_DISPLAY_PRIORITY+4, this);
	resume_thread(_WatchThread);
	
	debug_output("NetInputFilterPort: ready to rock and roll");
}

NetInputFilter::~NetInputFilter()
{
	kill_thread(_QueueThread);
	kill_thread(_WatchThread);
	
	delete_port(_CommandPort);
	delete_port(_NetQueuePort);
	
	if(_Remotesocket > 0)
		closesocket(_Remotesocket);
	_Remotesocket = -1;
}

int32 NetInputFilter::_StartWatchPort(void *arg)
{
	NetInputFilter *self = (NetInputFilter*)arg;
	self->_WatchPort();
	return (B_NO_ERROR);
}

void NetInputFilter::_WatchPort(void)
{
	int32 code;
	ssize_t length;
	char *buffer;
	BMessage *event;
	status_t err;

	while (true)
	{
		// Block until we find the size of the next message
		length = port_buffer_size(_CommandPort);
		if(length >= 0) // err otherwise
		{
			buffer = (char *)malloc(length);
			event = NULL;
			event = new BMessage();
			err = read_port(_CommandPort, &code, buffer, length);
			debug_output("NetInputFilterPort: got a command msg...");
			if(err != length)
			{
				if(err >= 0)
				{
					debug_output("NetInputFilterPort:failed to read full packet");
				}
				else
				{
					debug_output("NetInputFilterPort: read_port error");
				}
			}
			else if ((err = event->Unflatten(buffer)) < 0)
			{
				debug_output("NetInputFilterPort: event->Unflatten(buffer) encountered an error");
			}
			else
			{
				switch(event->what)
				{
					case FILTER_START:
					{
						debug_output("NetInputFilterPort: FILTER_START message");
						// create a socket to the remote host and start passing
						// input to it.
						// message contains:
						// host_addr - int32 - ip addr of remote host, needs to be swapped
						// host_port - short - port # or remote host,  needs to be swapped
						
						long host_addr;
						short host_port;
						
						if(event->FindInt32("host_addr", &host_addr) == B_OK)
						{
							if(event->FindInt16("host_port", &host_port) == B_OK)
								_Connect(host_port, host_addr);
						}
						
						
						break;
					}
					case FILTER_STOP:
					{
						debug_output("NetInputFilterPort: FILTER_STOP message");
						// terminate the connection with extreme predjudice
						// the input device on the remote end should get an
						// error message from recv() if this is successful
						
						_ConnectionDied();
						
						break;
					}
					case FILTER_LOCKOUT:
					{
						debug_output("NetInputFilterPort: FILTER_LOCKOUT message");
						// toggle sending of input messages. The scroll lock key
						// has the same effect.
						bool mode;
						if(event->FindBool("IsLocked", &mode) == B_OK)
						{
							if(mode)
							{
								_InputLockMode = 1;
								debug_output("NetInputFilterPort: _InputLockMode = 1");
							}
							else
							{
								_InputLockMode = 0;
								debug_output("NetInputFilterPort: _InputLockMode = 0");
							}
						}
						break;
					}
					default:
					{
						debug_output("NetInputFilterPort: message not understood");
						break;
					}
				}
				free(buffer);
				if(event!=NULL)
				{
					delete(event);
					event = NULL;
				}
			}
		}
	}
}


int32 NetInputFilter::_StartQueuePort(void *arg)
{
	NetInputFilter *self = (NetInputFilter*)arg;
	self->_QueuePort();
	return (B_NO_ERROR);
}

void NetInputFilter::_QueuePort(void)
{
	int32 code;
	ssize_t length, temp_length;
	char *buffer, dbg_msg_buff[255];
	status_t err;

	while (true)
	{
		// Block until we find the size of the next message
		length = port_buffer_size(_NetQueuePort);
		buffer = (char *)malloc(length);
		err = read_port(_NetQueuePort, &code, buffer, length);
		debug_output("NetInputFilterPort: got a message to pipe across the net!");
		sprintf(dbg_msg_buff, "NetInputFilterPort: length = %ld", length);
		debug_output(dbg_msg_buff);
		memset(dbg_msg_buff, 0, 255);
		if(err != length)
		{
			if(err >= 0)
			{
				debug_output("NetInputFilterPort: failed to read full packet (port)");
			}
			else
			{
				debug_output("NetInputFilterPort: read_port error");
			}
		}
		else if(_Remotesocket > 0)
		{
			// fire that puppy off to neverneverland
			temp_length = B_HOST_TO_BENDIAN_INT32(length); // swapped length
			err = send(_Remotesocket, &temp_length, 4, 0); // send the swapped length
			if(err < 0)
			{
				// bail in case of error
				free(buffer); // free the buffer (cuz this is our last chance :)
				_ConnectionDied(); // this call will kill us
			}
			else
			{
				int32 bytes_sent=0;
				
				// send loop
				while(err > 0 && bytes_sent < length)
				{
					err = send(_Remotesocket, buffer, length, 0);  // send the data!
					if(err > 0)
					{
						bytes_sent += err; // add to the bytes_sent counter
						buffer += err;
					}
				}
				
				if(err < 0)
				{
					// bail in case of error
					free(buffer); // free the buffer (cuz this is our last chance :)
					_ConnectionDied(); // this call will kill us
				}
				else
					debug_output("NetInputFilterPort: successful send to remote locale");
			}
		}
		free(buffer); // free the buffer
	}
}

filter_result NetInputFilter::Filter(BMessage *message, BList *)
{
	status_t err;
	filter_result res = B_DISPATCH_MESSAGE;
		
	// if the user has locked the input, then skip the message.
	// (but still fire it out to the remote machine (maybe))
	if(_InputLockMode)
	{
		res = B_SKIP_MESSAGE;
	}
	
	if(modifiers() & B_SCROLL_LOCK || !_currently_sending)
	{
		// don't send it abroad
		res = B_DISPATCH_MESSAGE; // so we can save ourselves with the scroll lock
	}
	else if(_QueueThread > 0)
	{
		// send it abroad
		size_t length = message->FlattenedSize();
		char *stream=(char *)malloc(length);
		if(stream)
		{
			if ( message->Flatten(stream, length) == B_OK)
			{
				// try to write the queue port
				err = write_port_etc(_NetQueuePort, 0, stream, length, B_TIMEOUT, 0);
				if(err == B_WOULD_BLOCK)
				{
					// full queue. deal with it
					debug_output("NetInputFilterPort: dropped a packet.");
					
					// remove the "when" field from the messages. it has not meaning on a
					// remote machine.
					
					// B_MOUSE_MOVED message
					// "x" - int32 - change in x. positive to the left, negative to the right
					// "y" - int32 - change in y. positive going up, negative going down (flipped)
					
					// B_MOUSE_DOWN message
					// "x" - int32 - ignored ? (always 0)
					// "y" - int32 - ignored ? (always 0)
					
					// B_MOUSE_UP
					// same a 
					
					;
				}
				else
				{
					// success!
					;
				}
			}
			free(stream);
		}
	}	
	return (res);
}

void NetInputFilter::_Connect(short host_port, long host_addr)
{
	status_t err;
	
	if(_Remotesocket < 0)
	{
		sockaddr_in sa;
		char buffer[256];
		
		sprintf(buffer, " before swap: host_addr = %x, host_port = %d", host_addr, host_port);						
		
		debug_output("NetInputFilterPort: connecting to remote host");
		debug_output(buffer);
		
		_Remotesocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(_Remotesocket < 0)
			;
		else
		{
			sa.sin_family = AF_INET;
			sa.sin_port = htons(host_port);
			sa.sin_addr.s_addr = host_addr;//htonl(host_addr);
			memset(sa.sin_zero, 0, sizeof(sa.sin_zero));
			
			memset(buffer,0,256);
			sprintf(buffer, "after swap: host_addr = %x, host_port = %d", sa.sin_addr.s_addr, sa.sin_port);
			debug_output(buffer);
			
			err = connect(_Remotesocket, (sockaddr *)&sa, sizeof(sa));
			if(err < 0)
			{
				closesocket(_Remotesocket);
				_Remotesocket = -1;
				debug_output("NetInputFilterPort: failed creation of remote socket");
			}
			else
			{
				_currently_sending = true;
				debug_output("NetInputFilterPort: successful creation of remote socket");
				
				// now create the queue thread and start it
				_QueueThread = spawn_thread(_StartQueuePort, "NetInputFilter:net", B_REAL_TIME_DISPLAY_PRIORITY+4, this);
				resume_thread(_QueueThread);
			}
		}
	}
}

void NetInputFilter::_ConnectionDied()
{
	// kill the queue thread
	kill_thread(_QueueThread);
	_QueueThread = -1;
	// kill the socket (probably already dead)
	closesocket(_Remotesocket);
	_Remotesocket = -1;
	// hit the off switch
	_currently_sending = false;
	debug_output("NetInputFilterPort: Remote connection died.");
}

void NetInputFilter::debug_output(const char *blah)
{
	if(_debug_port > 0)
	{
		// very small chance that not tolerating a timeout would cause us
		// to drop a message, but don't count on it.
		// (not exactly a high traffic port, 'ol velvet... )
		write_port_etc( _debug_port, 0, blah, strlen(blah), B_TIMEOUT, 250);
	}
}