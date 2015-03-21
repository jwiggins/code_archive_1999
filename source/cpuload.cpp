#include <iostream.h>
#include <be/kernel/OS.h>
#include <unistd.h>
void main()
{	
	//actual cpu status is taken
	bigtime_t first, second;
	system_info info;
    get_system_info ( &info );
    int numcpus = info.cpu_count;
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
}
//eof
