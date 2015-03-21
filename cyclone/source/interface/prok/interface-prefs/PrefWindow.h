/*
	
	PrefWindow.h

*/

#ifndef PREF_WINDOW_H
#define PREF_WINDOW_H

#ifndef _WINDOW_H
#include <Window.h>
#endif

class PrefWindow : public BWindow {

public:
				PrefWindow(BRect frame); 
virtual	bool	QuitRequested();
};

#endif
