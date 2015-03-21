#include <iostream.h>
#include <be/kernel/OS.h>
#include <unistd.h>

bigtime_t uptime();
void clock_format();
void main()
{
	bigtime_t lastUpdateTime = system_time();
	system_info info;
	get_system_info ( &info );
	int numcpus = info.cpu_count;
	
	
	cout << "Load Info: " << endl;
	//actual cpu status is taken
	bigtime_t first, second;
    get_system_info ( &info );
    bigtime_t fNew_one, fNew_two;
    float final, cpu_amount, system_amount;
	sleep(.2);

    for(int i=0; i < numcpus; i++ )
    {
    	sleep(.3);
    	first = system_time();
        get_system_info (&info);
		fNew_one = info.cpu_infos[i].active_time;
		sleep(1);
		second = system_time();
		get_system_info (&info);
		fNew_two = info.cpu_infos[i].active_time;
		cpu_amount = fNew_two - fNew_one;
		system_amount = second - first;
		final = (cpu_amount / system_amount) * 100.0;
		cout << "cpu " << i+1 << ": " << final << "%" << endl;
		sleep(.2);
	}
	//end cpu usage thingy
	
	
	//begin localtime
	time_t value1;
	
	value1 = time(NULL);
	puts( asctime(localtime(&value1)));
	//end localtime
	cout << "Your machine was booted ";
	clock_format();
	cout << " minutes ago." << endl;
	cout << "number of threads currently running: " << info.used_threads << endl; //running threads
	
	//begin silly uneccessary check for power :)
	if(is_computer_on() == 1) //im hoping this always returns true because 
	{							// if it doesn't, my program wont know what to say. :)				
		cout << "Your computer is on. :)" << endl; 
	} //whew... i sure am glad this baby's turned on... :)
}

bigtime_t uptime()
{
	bigtime_t awake_time;
	bigtime_t boottime = system_time();
	awake_time = boottime / 1000000 ;//seconds of boot time (i hope)
	awake_time = awake_time / 60; // boot minutes (if my last calculation was correct)
	
	return(awake_time);
}

void clock_format()
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
		cout << hours << " hours, ";
	}
	if(up > 0)
	{
		cout << up << " ";
	}
	return;
}
	
//eof