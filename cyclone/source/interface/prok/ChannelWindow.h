/*
	
	ChannelWindow.h
	
	John Wiggins. Cyclone GUI. 1998
	
*/

#include <string>
#include "CyDispatcher.h"

#ifndef CHANNEL_WINDOW_H
#define CHANNEL_WINDOW_H

#include <Window.h>
#include "myFilters.h"

#define STUB			'stub'

class CycloneChannelWindow : public BWindow {

public:
					CycloneChannelWindow(CyDispatcher *dispatcher,
							const char *name, CyTarget *handler,
							target_type type, BRect frame,
							int32 menuHeight);
					~CycloneChannelWindow();
virtual	bool		QuitRequested();
virtual void		MessageReceived(BMessage *msg);

void				ChangeNick(const char *nick);
void				SetName(const char *name);
void				SetTopic(const istring &topic);
void				PrintMessage(const istring &nick, const istring &text);
void				PrintText(const istring &text);
void				PrintAction(const istring &nick, const istring &text);

const char			*Get_name() { return _name; }
CyDispatcher		*Get_dispatcher() { return _dispatcher; }

private:
char				*_name;
CyDispatcher		*_dispatcher;
CyTarget			*_handler;
target_type			_type;
focusFilter			*focusfilter;

};

#endif
