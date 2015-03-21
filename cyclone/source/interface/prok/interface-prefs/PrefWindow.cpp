/*
	
	PrefWindow.cpp

*/

#ifndef _APPLICATION_H
#include <Application.h>
#endif

#ifndef PREF_WINDOW_H
#include "PrefWindow.h"
#endif
#ifndef PREF_VIEW_H
#include "PrefView.h"
#endif

PrefWindow::PrefWindow(BRect frame)
		: BWindow(frame, "Interface Preferences", B_TITLED_WINDOW,B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	PrefView			*view;
	frame.OffsetTo(B_ORIGIN);
	view = new PrefView(frame, "_MainPrefsView");
	AddChild(view);
	Show();
}

bool PrefWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
