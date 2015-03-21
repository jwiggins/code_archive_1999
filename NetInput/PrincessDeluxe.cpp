#include <stdio.h>
#include <OS.h>
#include <malloc.h>

#include <Message.h>

int main()
{
	port_id port;
	size_t length=0;
	char *buffer;
	int32 code;
	status_t err;
	
	BMessage *msg = new BMessage();
	
	port = create_port(100, "_PrincessDeluxe_");
	
	if(port > 0)
	{
		while(true)
		{
			length = port_buffer_size(port); // block till we get a msg
			
			buffer = (char *)malloc(length+1);
			
			err = read_port(port, &code, buffer, length); // grab the msg
			if(err > 0)
			{
				if(msg->Unflatten(buffer) == B_OK)
				{
					msg->PrintToStream();
				}
				else
				{
					buffer[length] = '\0';
					printf("PrincessDeluxe: Bad message.\n %s\n", buffer);
				}
			}
			
			msg->MakeEmpty();
			free(buffer); // free
			buffer= NULL;		
			// loop
		}
		delete_port(port); // clean up
	}
	
	return 0;
}