#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <be/kernel/OS.h>

bigtime_t uptime();
void uptime_format();
void main()
{
	//c book ex.
	time_t value1;
	
	value1 = time(NULL);
	cout << endl;
	puts( asctime(localtime(&value1)));
	//end ex.
	cout << "uptime: ";
	uptime_format();
	cout << "minutes." << endl;
	return;
}

bigtime_t uptime()
{
	bigtime_t awake_time;
	bigtime_t boottime = system_time();
	awake_time = boottime / 1000000 ;//seconds of boot time (i hope)
	awake_time = awake_time / 60; // boot minutes (if my last calculation was correct)
	
	return(awake_time);
}

void uptime_format()
{
	int up = uptime();
	int days, hours;
	if(up > 1440)
	{
		days = (up/1440);
		up = up - (days * 1440);
		cout << days<< " days, ";
	}
	if(up > 60)
	{
		hours = (up/60);
		up = up - (hours * 60);
		cout << hours << " hours, and ";
	}
	if(up > 0)
	{
		cout << up << " ";
	}
	return;
}


	
//eof
