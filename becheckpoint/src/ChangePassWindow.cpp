/*
	
	ChangePassWindow.cpp

*/

#include "ChangePassWindow.h"


ChangePassWindow::ChangePassWindow(BRect frame)
		: BWindow(frame, "Change Password", B_TITLED_WINDOW,B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	BRect			rect = frame;
	ChangePassView	*view;
	
	rect.OffsetTo(B_ORIGIN);
	view = new ChangePassView(rect, "view");
	AddChild(view);
}

bool ChangePassWindow::QuitRequested()
{
	PostMessage(B_QUIT_REQUESTED);
	return(TRUE);
}

void ChangePassWindow::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
		case BUTTON_PRESS:
			{
				BMessage *hey = new BMessage(PASSWORD_CHANGE);
				char *old_password, *new_password;
				PassControl	*old_pass = static_cast<PassControl *>(FindView("old"));
				PassControl	*new_pass = static_cast<PassControl *>(FindView("new"));
				
				// package up the text in the PassControls
				old_password = new char [ strlen(old_pass->actualText()) + 1 ];
				strcpy(old_password, old_pass->actualText());
				new_password = new char [ strlen(new_pass->actualText()) + 1 ];
				strcpy(new_password, new_pass->actualText());
				
				hey->AddString("oldpassword", old_password);
				hey->AddString("newpassword", new_password);
				
				// send it to be_app
				be_app->PostMessage(hey);
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