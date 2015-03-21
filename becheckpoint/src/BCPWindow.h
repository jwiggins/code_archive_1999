/*
	
	BCPWindow.h

*/

#ifndef BCP_WINDOW_H
#define BCP_WINDOW_H
#include <Window.h>
#include <Application.h>
#include <string.h>
#include <Message.h>
#include "main.h"
#include "BCPMessages.h"
#include "ChangePassWindow.h"
#include "PassControl.h"


class BCPWindow : public BWindow {

public:
				BCPWindow(BRect frame); 
virtual	bool	QuitRequested();
virtual	void	MessageReceived(BMessage *msg);
};

#endif
