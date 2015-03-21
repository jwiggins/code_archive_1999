#include <E-mail.h>
#include <SupportDefs.h>
#include <stdio.h>
#include <string.h>


int main()
{
	status_t err;
	int32 mailcount;
	mail_pop_account acct;
	
	if(get_pop_account(&acct) != B_OK)
		acct.pop_name[0] = 0;
	
	err = check_for_mail(&mailcount);
	if(err != B_OK)
	{
		switch(err)
		{
			default:
			{
				case B_MAIL_NO_DAEMON:
				{
					printf("Error: The mail daemon is not running.\n");
					break;
				}
				case B_MAIL_UNKNOWN_USER:
				{
					printf("Error: The POP server doesn't recognize the username%s%s\n",(acct.pop_name == 0)? ".":" ",(acct.pop_name == 0)? " ":acct.pop_name);
					break;
				}
				case B_MAIL_WRONG_PASSWORD:
				{
					printf("Error: Wrong password.\n");
					break;
				}
				case B_MAIL_UNKNOWN_HOST:
				{
					printf("Error: The server could not be found.\n");
					break;
				}
				case B_MAIL_ACCESS_ERROR:
				{
					printf("Error: The connection to the server failed.\n");
					break;
				}
				printf("%s\n",strerror(err));
				break;
			}
		}
	}
	else
		printf("mail retrieved. %d new messages\n",mailcount);
}