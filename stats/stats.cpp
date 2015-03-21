#include <be/kernel/OS.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

void clock_format();

void main()
{
	time_t			time_value = time(NULL);
	char				time_string[MAX_TIMESTR+1];
	bigtime_t		clock_start, clock_total, cpu_start, cpu_finish;
	double			final;
	system_info		start_info, stop_info;
	int8			numcpus;
	
	get_system_info(&start_info); // fill up the info struct for some starting values
	numcpus = start_info.cpu_count;
	
	// begin uptime
	strftime(time_string, MAX_TIMESTR, "%I:%M%p", localtime(&time_value));
	printf("%s up ", time_string);
	clock_format(); // do the little uptime string
	
	//running threads & power check
	printf(" : %d threads : power %s : computer is%son fire\n", 
			start_info.used_threads, (is_computer_on() ? "on" : "off"), 
				(is_computer_on_fire() ? " not " : " "));
	
	//actual cpu status is taken
    // chunk one
    get_system_info(&start_info);
	clock_start = system_time();
	snooze( 1000 * 1000 ); // 1 mill usecs = 1 sec
	// chunk two
	get_system_info(&stop_info);
	clock_total = system_time() - clock_start;
	// now just do the math
	for(int i=0; i < numcpus; i++ )
    {
		cpu_start = start_info.cpu_infos[i].active_time;
		cpu_finish = stop_info.cpu_infos[i].active_time;
		final = (double)((((double)(cpu_finish - cpu_start))/((double)(clock_total))) * 100);
		printf("cpu %d: %5.2f\% ", i+1, final);
		if((((i+1) & 3) == 0) || (i == (numcpus-1))) printf("\n"); // 4 cpus per line
	}
	//end cpu usage thingy
}

void clock_format()
{
	int32 up = (system_time() / (60 * 1000 * 1000));
	int32 days, hours;
	if(up > 1440)
	{
		days = (up/1440);
		up = up - (days * 1440);
		printf("%d days, ", days);
		if(up > 60)
		{
			hours = (up/60);
			up = up - (hours * 60);
			printf("%d:", hours);
			if(up >= 0)
			{
				printf("%s%d", ((up > 9) ? "" : "0"), up);
			}
		}
		else
			printf("%d mins", up);
	}
	else
	{
		if(up > 60)
		{
			hours = (up/60);
			up = up - (hours * 60);
			printf("%d:", hours);
			if(up >= 0)
			{
				printf("%s%d", ((up > 9) ? "" : "0"), up);
			}
		}
		else
			printf("%d mins", up);
	}
}
	
//eof