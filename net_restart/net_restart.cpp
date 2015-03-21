#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <be/AppKit.h>

int main()
{
	status_t error;
	bool m_d = false;
	
	//to kill
	BMessage *msg = new BMessage(B_QUIT_REQUESTED);
	BMessenger *usps_one = new BMessenger("application/x-vnd.Be-NETS",-1,&error);
	BMessenger *usps_two = new BMessenger("application/x-vnd.Avatar.message_daemon", -1, &error);
	if(usps_two->IsValid())		// message_daemon is running. kill it first or suffer the  consequences
	{
		m_d = true;
		printf("killing message_daemon.\n");
		usps_two->SendMessage(msg);
		delete usps_two;
	}
	else
	{
		delete usps_two;
	}
	if(usps_one->IsValid())		// test if the net_server is here...
	{
		usps_one->SendMessage(msg);
		delete msg;
		delete usps_one;
	}
	else
	{
		printf ("net_server already dead...\n");
		delete msg;
		delete usps_one;
	}
	//end kill

	//to start
	system("/boot/beos/system/boot/Netscript"); //Run Netscript!
	if(m_d)
	{
		printf("restarting message_daemon.\n");
		be_roster->Launch("application/x-vnd.Avatar.message_daemon");
	}
	//end start
}