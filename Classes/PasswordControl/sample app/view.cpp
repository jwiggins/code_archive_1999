/*
	
	view.cpp
	
	John Wiggins. generic app shell. 1998
	
*/

#ifndef VIEW_H
#include "view.h"
#endif
#include "PassControl.h"
#define TEXT_MSG 31337
#define BUTTON_MSG 31338
View::View(BRect rect, char *name)
	   	   : BView(rect, name, B_FOLLOW_ALL, B_WILL_DRAW)
{
}


void View::AttachedToWindow()
{
	SetViewColor(220,220,220);
	BRect rect;
	
	rect.Set(25,25,175,49);
	PassControl *password = new PassControl(rect, "password", "password", NULL, new BMessage(TEXT_MSG));
	password->SetText("SetText() is smart"B_UTF8_SMILING_FACE); // test SetText()
	AddChild(password);
	password->MakeFocus(true);
	
	rect.Set(180,20,200,25);
	BButton *button = new BButton(rect, "button", "ok", new BMessage(BUTTON_MSG));
	AddChild(button);
}


void View::Draw(BRect)
{
}
