/*
	
	view.h
	
	John Wiggins. generic app shell. 1998
	
*/

#ifndef VIEW_H
#define VIEW_H

#ifndef _VIEW_H
#include <View.h>
#endif
#ifndef DRAWVIEW_H
#include "drawView.h"
#endif

class View : public BView {

public:
				View(BRect frame, char *name);
				~View();
virtual	void	AttachedToWindow();
virtual	void	Draw(BRect updateRect);
virtual void	MouseDown(BPoint point);
void			translate();
drawView		*picasso;

private:
thread_id		mouse_thread; // so i can check on it easier
int32			threadFunc(); // does the actual work
static int32	thread_start(void *arg); // calls threadFunc
BBitmap			*foo;

double 			Rmat[4][4]; // my global rotation matrix
BPoint			previous, current;
};

#endif
