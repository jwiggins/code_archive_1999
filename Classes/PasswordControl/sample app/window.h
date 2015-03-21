/*
	
	window.h
	
	John Wiggins. generic app shell. 1998
	
*/

#ifndef WINDOW_H
#define WINDOW_H

#ifndef _WINDOW_H
#include <Window.h>
#endif

class Window : public BWindow {

public:
				Window(BRect frame); 
virtual	bool	QuitRequested();
virtual	void	MessageReceived(BMessage *message);
};

#endif
