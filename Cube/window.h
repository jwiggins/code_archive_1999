/*
	
	window.h
	
	John Wiggins. generic app shell. 1998
	
*/

#ifndef WINDOW_H
#define WINDOW_H

#ifndef _WINDOW_H
#include <Window.h>
#endif
#define MENU_BAR_HEIGHT 20
#define CUBE		'cube'
#define	PYRAMID		'pyra'
#define	PARALLEL	'para'

class Window : public BWindow {

public:
				Window(BRect frame); 
virtual	bool	QuitRequested();
virtual void	MessageReceived(BMessage *msg);
};

#endif
