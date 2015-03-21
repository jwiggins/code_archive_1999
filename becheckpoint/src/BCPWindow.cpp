/*
	
	BCPWindow.cpp

*/

#include "BCPWindow.h"


BCPWindow::BCPWindow(BRect frame)
		: BWindow(frame, "BeCheckPoint", B_TITLED_WINDOW,B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
}

bool BCPWindow::QuitRequested()
{
	if(((BCPApp *)be_app)->ok_to_quit())
	{
		be_app->PostMessage(B_QUIT_REQUESTED);
		return true;
	}
	else
		return false;
}

void BCPWindow::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
		case BUTTON_PRESS:
			{
				BMessage	*hey = new BMessage(PASSWORD_ATTEMPT);
				char *entered_password;
				PassControl	*pass = static_cast<PassControl *>(FindView("password"));
				
				// package up the password in the PassControl
				entered_password = new char [ strlen(pass->actualText()) + 1 ];
				strcpy(entered_password, pass->actualText());
				
				hey->AddString("password", entered_password);
				
				// send it to be_app
				be_app->PostMessage(hey);
			}
			break;
		case CHANGE_PASSWORD_DIALOG:
			{
				BRect rect;
				ChangePassWindow *win;
				
				rect.Set(299, 299, 599, 374);
				win = new ChangePassWindow(rect);
				
				win->Show();
			}
			break;
		case SHAKE_WINDOW:
			{
				msg->PrintToStream();
				MoveBy(50,0);
				snooze(80 * 1000);
				MoveBy(-50,0);
				snooze(80 * 1000);
				MoveBy(50,0);
				snooze(80 * 1000);
				MoveBy(-50,0);
				snooze(80 * 1000);
			}
			break;
		default:
			BWindow::MessageReceived(msg);
			break;
	}
}