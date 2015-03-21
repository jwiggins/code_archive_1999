/*
	
	MultiView.h
	
	John Wiggins. Cyclone GUI. 1998
	
*/

#ifndef MULTI_VIEW_H
#define MULTI_VIEW_H

#include "myFilters.h"
#include "myTextViews.h"
#include <View.h>
#include "InterfaceDefs.h"

#define TEXT_ENTRY		'tent'
#define SEND_BUTTON		'send'

class resizeFilter;
class focusFilter;
class inputTextView;

class MultiView : public BView {

public:
				MultiView(BRect frame, char *name, const char *nick, target_type type);
				~MultiView();
virtual	void	FrameResized(float width, float height);
virtual	void	Pulse();
virtual	void	Draw(BRect updateRect);
virtual	void	MouseDown(BPoint point);

void			SetTopic(const char *topic);
void			ChangeNick(const char *newnick);
void			DrawUsers();
void			DrawTopic();
static int32	mouse_poll_start(void *arg);
int32			mouse_poll();
void			DividerMoved();

private:

BTextView		*_ChannelText;
BListView		*_UserList;
BScrollView		*_ChannelScrollView, *_UserScrollView;
inputTextView	*_InputText;
resizeFilter	*resizefilter;
target_type		_type;
int32			interfaceUnit, charwidth, divider, userlistwidth;
char			*usernick, *_topic;
bool			resizeing;

};

#endif
