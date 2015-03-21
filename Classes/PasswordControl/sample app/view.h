/*
	
	view.h
	
	John Wiggins. generic app shell. 1998
	
*/

#ifndef VIEW_H
#define VIEW_H

#ifndef _VIEW_H
#include <View.h>
#endif

class View : public BView {

public:
				View(BRect frame, char *name); 
virtual	void	AttachedToWindow();
virtual	void	Draw(BRect updateRect);

};

#endif
