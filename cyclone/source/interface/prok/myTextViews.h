/*
	
	myTextViews.h
	
	John Wiggins. Cyclone GUI. 1998
	
*/

#ifndef MY_TEXT_VIEWS_H
#define MY_TEXT_VIEWS_H

#include <TextView.h>

class ircTextView : public BTextView {

public:
				ircTextView(BRect frame, const char *name, BRect textRect);
};

class inputTextView : public BTextView {

public:
				inputTextView(BRect frame, const char *name);

virtual	void	Paste(BClipboard *clipboard);
virtual	void	MakeFocus(bool focused = true);
virtual	void	KeyDown(const char *bytes, int32 numBytes);
virtual	void	Draw(BRect updateRect);
};

#endif