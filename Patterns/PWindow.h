/*
	
	PWindow.h
	
	John Wiggins 1998.
	
*/

#ifndef PWINDOW_H
#define PWINDOW_H

#ifndef _WINDOW_H
#include <Window.h>
#endif

class HelloWindow : public BWindow {

public:
				HelloWindow(BRect frame); 
virtual	bool	QuitRequested();
};

#endif
