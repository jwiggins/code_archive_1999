/*
	
	main.h
	
	John Wiggins. generic app shell. 1998

*/

#ifndef MAIN_H
#define MAIN_H

#include <Application.h>
#include <stdio.h>
#include <string.h>
#include <unistd.be.h>
#include "NewsWindow.h"
#include "NewsView.h"
#include "attrnames.h"

class NewsApp : public BApplication {

public:
					NewsApp();
					~NewsApp();
virtual	void		RefsReceived(BMessage *msg);
virtual	void		MessageReceived(BMessage *msg);
virtual	void		AboutRequested();
virtual	bool		QuitRequested();
void				SetCurrentDirectory();
BWindow	*			FindWindow(const char *title);
status_t			Save(BMessage *msg);
private:
};
#endif
