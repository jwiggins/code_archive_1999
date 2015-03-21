/*
	
	window.cpp
	
	John Wiggins. generic app shell. 1998
	
*/

#ifndef _APPLICATION_H
#include <Application.h>
#endif

#ifndef WINDOW_H
#include "window.h"
#endif

#ifndef PASSCONTROL_H
#include "PassControl.h"
#endif
#include <Alert.h>

#define BUTTON_MSG 31338
Window::Window(BRect frame)
				: BWindow(frame, "Shell", B_TITLED_WINDOW,B_NOT_ZOOMABLE)
{

}

bool Window::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return(TRUE);
}

void Window::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		case BUTTON_MSG:
			{
				const char *foo = ((PassControl *)ChildAt(0)->ChildAt(0))->actualText();
				printf("%s\n", foo);
				(new BAlert("alert", foo, "ok"))->Go();
			}
			break;
		default:
			BWindow::MessageReceived(message);
	}
}
