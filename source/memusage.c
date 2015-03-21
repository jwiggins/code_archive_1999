#include <stdio.h>
#include <OS.h>

int main(int argc, char *argv[])
{
	int32 cookie1 = 0, cookie2 = 0;
	size_t size, ram_size;
	area_info a_info; /* hold data about areas */
	team_info t_info; /* hold data about teams */
	//printf("entering main loop.\n");
	
	while(get_next_team_info(&cookie1, &t_info) == B_OK)
	{
		cookie2 = size = ram_size = 0;
		//printf("entering inner loop.\n");
		while(get_next_area_info(t_info.team, &cookie2, &a_info) == B_OK)
		{
			size += a_info.size;
			ram_size += a_info.ram_size;
		}
		//printf("exiting inner loop.\n");
		printf("TEAM %7ld (%s):\n", t_info.team, t_info.args);
		printf("Allocated: %5ldK\tResident: %5ldK\n", (size/1024), (ram_size/1024));
	}
	
	
	return 0;
}
