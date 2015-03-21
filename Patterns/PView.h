/*
	
	PView.h
	
	John Wiggins 1998
	
*/

#ifndef PVIEW_H
#define PVIEW_H

#ifndef _VIEW_H
#include <View.h>
#endif

class HelloView : public BView {

public:
				HelloView(BRect frame, char *name); 
virtual	void	AttachedToWindow();
virtual	void	Pulse(void);

private:
static int32	thread_func(void *arg); // what you spawn the thread with
int32			threadFunc(); // what actually does the work
thread_id		my_thread; // the thid

BBitmap			*foo_bitmap; // the bitmap
BScrollBar		*scroller; // the scrollbar
BBox			*box; // the box that holds the pattern #
BStringView		*poo; // the stringview that holds the number where the 
						// user can see it
};

#endif
