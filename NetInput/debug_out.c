#include <stdio.h>
#include <OS.h>
#include <malloc.h>

int main()
{
	port_id port;
	size_t length=0;
	char *buffer;
	int32 code;
	status_t err;
	
	port = create_port(100, "_VelvetJones_");
	
	if(port > 0)
	{
		while(true)
		{
			length = port_buffer_size(port); // block till we get a msg
			
			buffer = (char *)malloc(length+1);
			
			err = read_port(port, &code, buffer, length); // grab the msg
			if(err > 0)
				printf("%s\n", buffer); // print the msg
			
			free(buffer); // free
			buffer= NULL;		
			// loop
		}
		delete_port(port); // clean up
	}
	
	return 0;
}