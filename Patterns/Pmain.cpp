/*
	
	Pmain.cpp
	
	John Wiggins. 1998
	This software is supplied "as-is". Blah Blah Blah. You get the 
	idea. Don't come crying to me or anyone else if this software 
	does anything nasty to your system. I have made it as bug free 
	as needed.
	
*/

#ifndef PWINDOW_H
#include "PWindow.h"
#endif
#ifndef PVIEW_H
#include "PView.h"
#endif
#ifndef PMAIN_H
#include "Pmain.h"
#endif

// Hmmm... Wonder what this source started out as?
// HelloWorld. For all your AppShell(tm) needs...
main()
{	
	HelloApplication *myApplication;

	myApplication = new HelloApplication();
	myApplication->Run();
	
	delete(myApplication);
	return(0);
}

HelloApplication::HelloApplication()
		  		  : BApplication("application/x-vnd.Prok-Patterns")
{
	HelloWindow		*aWindow;
	HelloView		*aView;
	BRect			aRect;

	// set up a rectangle and instantiate a new window
	aRect.Set(100, 100, 244, 209 + B_H_SCROLL_BAR_HEIGHT);
	aWindow = new HelloWindow(aRect);
	
	// set up a rectangle and instantiate a new view
	// view rect should be same size as window rect but with left top at (0, 0)
	aRect.OffsetTo(B_ORIGIN);
	aView = new HelloView(aRect, "Patterns");
	
	// add view to window
	aWindow->AddChild(aView);
	
	// make window visible
	aWindow->Show();
}