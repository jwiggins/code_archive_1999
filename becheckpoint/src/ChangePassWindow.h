/*
	
	ChangePassWindow.h

*/

#ifndef CHANGE_PASS_WINDOW_H
#define CHANGE_PASS_WINDOW_H
#include <Window.h>
#include <Application.h>
#include <string.h>
#include <Message.h>
#include "BCPMessages.h"
#include "ChangePassView.h"
#include "PassControl.h"


class ChangePassWindow : public BWindow {

public:
				ChangePassWindow(BRect frame); 
virtual	bool	QuitRequested();
virtual	void	MessageReceived(BMessage *msg);
};

#endif
