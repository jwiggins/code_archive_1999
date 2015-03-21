/*
	
	main.cpp
	
	John Wiggins. generic app shell. 1998
	
*/

#ifndef WINDOW_H
#include "window.h"
#endif
#ifndef VIEW_H
#include "view.h"
#endif
#ifndef MAIN_H
#include "main.h"
#endif

main()
{	
	Application myApplication;
	myApplication.Run();
	
	return(0);
}

Application::Application()
		  		  : BApplication("application/x.vnd-Prok.PassControl")
{
	Window		*aWindow;
	View		*aView;
	BRect		aRect;

	// set up a rectangle and instantiate a new window
	aRect.Set(100, 100, 349, 149);
	aWindow = new Window(aRect);
	
	// set up a rectangle and instantiate a new view
	// view rect should be same size as window rect but with left top at (0, 0)
	aRect.OffsetTo(B_ORIGIN);
	aView = new View(aRect, "FooView");
	
	// add view to window
	aWindow->AddChild(aView);
	
	// make window visible
	aWindow->Show();
}