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
	
	void VelvetJones(const char *blah);
	void PrincessDeluxe(char *blah, int32 size);

	port_id _CommandPort, _NetQueuePort;
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
	//_debug_port = find_port("_VelvetJones_");
	
	
	VelvetJones("NetInputFilterPort: Hello World!");
	
	_NetQueuePort = create_port(1,"NetInputFilterPort:net"); // one message deep
	_CommandPort = create_port(100,"NetInputFilterPort:cmd");
	
	VelvetJones("NetInputFilterPort: ports were created.");
	
	if(_CommandPort < 0 || _NetQueuePort < 0)
	{
		VelvetJones("inputRecorderFilter: create_port error for the command port or the net queue port");
	}
	
	VelvetJones("NetInputFilterPort: spawning the command thread");
	// the command thread
	_WatchThread = spawn_thread(_StartWatchPort, "NetInputFilter:cmd", B_REAL_TIME_DISPLAY_PRIORITY+4, this);
	resume_thread(_WatchThread);
	
	VelvetJones("NetInputFilterPort: ready to rock and roll");
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
			VelvetJones("NetInputFilterPort: got a command msg...");
			if(err != length)
			{
				if(err >= 0)
				{
					VelvetJones("NetInputFilterPort:failed to read full packet");
				}
				else
				{
					VelvetJones("NetInputFilterPort: read_port error");
				}
			}
			else if ((err = event->Unflatten(buffer)) < 0)
			{
				VelvetJones("NetInputFilterPort: event->Unflatten(buffer) encountered an error");
			}
			else
			{
				switch(event->what)
				{
					case FILTER_START:
					{
						VelvetJones("NetInputFilterPort: FILTER_START message");
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
						VelvetJones("NetInputFilterPort: FILTER_STOP message");
						// terminate the connection with extreme predjudice
						// the input device on the remote end should get an
						// error message from recv() if this is successful
						
						_ConnectionDied();
						
						break;
					}
					case FILTER_LOCKOUT:
					{
						VelvetJones("NetInputFilterPort: FILTER_LOCKOUT message");
						// toggle sending of input messages. The scroll lock key
						// has the same effect.
						bool mode;
						if(event->FindBool("IsLocked", &mode) == B_OK)
						{
							if(mode)
							{
								_InputLockMode = 1;
								VelvetJones("NetInputFilterPort: _InputLockMode = 1");
							}
							else
							{
								_InputLockMode = 0;
								VelvetJones("NetInputFilterPort: _InputLockMode = 0");
							}
						}
						break;
					}
					default:
					{
						VelvetJones("NetInputFilterPort: message not understood");
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
	char *buffer, *buffer_start;//, dbg_msg_buff[255];
	status_t err;

	while (true)
	{
		// Block until we find the size of the next message
		length = port_buffer_size(_NetQueuePort);
		buffer_start = buffer = (char *)malloc(length);
		err = read_port(_NetQueuePort, &code, buffer, length);
		
		//VelvetJones("NetInputFilterPort: got a message to pipe across the net!");
		
		//sprintf(dbg_msg_buff, "NetInputFilterPort: length = %ld", length);
		//VelvetJones(dbg_msg_buff);
		//memset(dbg_msg_buff, 0, 255);
		
		//PrincessDeluxe(buffer, length); // BMessage debug output
		
		if(err != length)
		{
			//VelvetJones("NetInputFilterPort: err != length");
			if(err >= 0)
			{
				//VelvetJones("NetInputFilterPort: failed to read full packet (port)");
			}
			else
			{
				//VelvetJones("NetInputFilterPort: read_port error");
			}
		}
		else if(_Remotesocket >= 0)
		{
			// fire that puppy off to neverneverland
			//VelvetJones("NetInputFilterPort: Sending...");
			
			temp_length = B_HOST_TO_BENDIAN_INT32(length); // swapped length
			err = send(_Remotesocket, &temp_length, 4, 0); // send the swapped length
			if(err < 0)
			{
				// bail in case of error
				//VelvetJones("NetInputFilterPort: Send error");
				
				free(buffer); // free the buffer (cuz this is our last chance :)
				_ConnectionDied(); // this call will kill us
			}
			else
			{
				int32 bytes_sent=0;
				
				// send loop
				while(err > 0 && bytes_sent < length)
				{
					err = send(_Remotesocket, buffer, (length - bytes_sent), 0);  // send the data!
					if(err > 0)
					{
						bytes_sent += err; // add to the bytes_sent counter
						buffer += err;
					}
				}
				
				buffer = buffer_start; // restore the pointer
				
				if(err < 0)
				{
					// bail in case of error
					//VelvetJones("NetInputFilterPort: Send error");
					free(buffer); // free the buffer (cuz this is our last chance :)
					_ConnectionDied(); // this call will kill us
				}
				else
					;//VelvetJones("NetInputFilterPort: successful send to remote locale");
			}
		}
		else
		{
			//VelvetJones("NetInputFilterPort: _Remotesocket is invalid");
		}
		free(buffer); // free the buffer
		buffer_start = buffer = NULL;
	}
}

filter_result NetInputFilter::Filter(BMessage *message, BList *)
{
	bool from_network;
	const char *dummy;
	status_t err;
	bigtime_t when;
	filter_result res = B_DISPATCH_MESSAGE;
	
	// don't propogate messages that came in off the network
	from_network = (message->FindString("netmsg",&dummy) == B_OK);
		
	// if the user has locked the input, then skip the message.
	// (but still fire it out to the remote machine (maybe))
	if(from_network || _InputLockMode)
	{
		res = B_SKIP_MESSAGE;
	}
	
	if(modifiers() & B_SCROLL_LOCK || !_currently_sending)
	{
		// don't send it abroad, and give the user their control back
		res = B_DISPATCH_MESSAGE; // so we can save ourselves with the scroll lock
	}
	else if(_QueueThread > 0 && !from_network)
	{
		// remove this before taking the size
		if(message->FindInt64("when", &when) == B_OK)
			message->RemoveName("when");
		
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
					//VelvetJones("NetInputFilterPort: dropped a packet.");
					
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
			
			// clean up
			free(stream);
		}
		
		// put the "when" back (the time difference should not be significant)
		message->AddInt64("when", when);
	}	
	return (res);
}

void NetInputFilter::_Connect(short host_port, long host_addr)
{
	status_t err;
	
	if(_Remotesocket < 0)
	{
		sockaddr_in sa;
		//char buffer[256];
		
		//sprintf(buffer, " before swap: host_addr = %x, host_port = %d", host_addr, host_port);						
		
		//VelvetJones("NetInputFilterPort: connecting to remote host");
		//VelvetJones(buffer);
		
		_Remotesocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(_Remotesocket < 0)
			;
		else
		{
			sa.sin_family = AF_INET;
			sa.sin_port = htons(host_port);
			sa.sin_addr.s_addr = host_addr;//htonl(host_addr);
			memset(sa.sin_zero, 0, sizeof(sa.sin_zero));
			
			//memset(buffer,0,256);
			//sprintf(buffer, "after swap: host_addr = %x, host_port = %d", sa.sin_addr.s_addr, sa.sin_port);
			//VelvetJones(buffer);
			
			err = connect(_Remotesocket, (sockaddr *)&sa, sizeof(sa));
			if(err < 0)
			{
				closesocket(_Remotesocket);
				_Remotesocket = -1;
				//VelvetJones("NetInputFilterPort: failed creation of remote socket");
			}
			else
			{
				_currently_sending = true;
				//VelvetJones("NetInputFilterPort: successful creation of remote socket");
				
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
	//VelvetJones("NetInputFilterPort: Remote connection died.");
}

void NetInputFilter::VelvetJones(const char *blah)
{
	// very small chance that not tolerating a timeout would cause us
	// to drop a message, but don't count on it.
	// (not exactly a high traffic port, 'ol velvet... )
	write_port_etc( find_port("_VelvetJones_"), 0, blah, strlen(blah),	B_TIMEOUT, 250);
}

void NetInputFilter::PrincessDeluxe(char *blah, int32 size)
{
	// very small chance that not tolerating a timeout would cause us
	// to drop a message, but don't count on it.
	// (not exactly a high traffic port, 'ol velvet... )
	write_port_etc( find_port("_PrincessDeluxe_"), 0, blah, size,	B_TIMEOUT, 250);
}
