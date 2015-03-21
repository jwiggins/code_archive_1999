/*
	
	LoginView.h

*/

#ifndef LOGIN_VIEW_H
#define LOGIN_VIEW_H

#include <View.h>
#include <Button.h>
#include "PassControl.h"

class LoginView : public BView {

public:
				LoginView(BRect frame, char *name); 
virtual	void	AttachedToWindow();

};

#endif
