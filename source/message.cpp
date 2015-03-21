#include <stdio.h>
#include <Application.h>
#include <OS.h>

int main(void)
{
	BApplication app("application/x-vnd.Prok-messagearea");
	BMessage *a_message = new BMessage('test');
	area_info a_info;
	area_id a_id;
	
	a_id = area_for((void *)a_message);
	
	if(get_area_info(a_id, &a_info) == B_OK)
	{
		printf("area name = %s\n", a_info.name);
		printf("area size = %d bytes\n", a_info.size);
		switch(a_info.lock)
		{
			case B_NO_LOCK:
				printf("area lock = B_NO_LOCK\n");
				break;
			case B_LAZY_LOCK:
				printf("area lock = B_LAZY_LOCK\n");
				break;
			case B_FULL_LOCK:
				printf("area lock = B_FULL_LOCK\n");
				break;
			case B_CONTIGUOUS:
				printf("area lock = B_CONTIGUOUS\n");
				break;
			case B_LOMEM:
				printf("area lock = B_LOMEM\n");
				break;
		}
		switch(a_info.protection)
		{
			case B_READ_AREA:
				printf("area protection = B_READ_AREA\n");
				break;
			case B_WRITE_AREA:
				printf("area protection = B_WRITE_AREA\n");
				break;
		}
		printf("area ram_size = %d\n", a_info.ram_size);
		printf("area team = %d\n", a_info.team);
		//printf("\n");
	}
	
	return B_OK;
}