/*
	
	window.h
	
	John Wiggins. generic app shell. 1998
	
*/

#ifndef NEWS_WINDOW_H
#define NEWS_WINDOW_H

#include <Window.h>
#include <FilePanel.h>
#include "NewsView.h"

extern const char *attribute_names[];

class NewsWindow : public BWindow {

public:
				NewsWindow(BRect frame); 
virtual	bool	QuitRequested();
virtual void	MessageReceived(BMessage *msg);
private:
BFilePanel		*saver;
};

#endif
