/*
	
	PrefView.h

*/

#ifndef PREF_VIEW_H
#define PREF_VIEW_H

#ifndef _VIEW_H
#include <View.h>
#endif

class PrefView : public BView {

public:
				PrefView(BRect frame, char *name); 
virtual	void	AttachedToWindow();
virtual	void	Draw(BRect updateRect);

};

#endif
