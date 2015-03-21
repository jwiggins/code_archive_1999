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
	Application *myApplication;

	myApplication = new Application();
	myApplication->Run();
	
	delete(myApplication);
	return(0);
}

Application::Application()
		  		  : BApplication("application/x.vnd-Prok.Cube")
{
	Window		*aWindow;
	View		*aView;
	BMenuBar	*myMenubar;
	BMenu		*firstMenu;
	BRect		aRect;

	// set up a rectangle and instantiate a new window
	aRect.Set(100, 100, 349, 349 + MENU_BAR_HEIGHT);
	aWindow = new Window(aRect);
	
	// a menu
	aRect.Set(0,0, aWindow->Bounds().Width(), MENU_BAR_HEIGHT);
	myMenubar = new BMenuBar(aRect, "menubar");
	firstMenu = new BMenu("Shape");
	firstMenu->AddItem(new BMenuItem("Cube", new BMessage(CUBE)));
	firstMenu->AddItem(new BMenuItem("Pyramid", new BMessage(PYRAMID)));
	firstMenu->AddItem(new BMenuItem("Parallela whatever", new BMessage(PARALLEL)));
	firstMenu->AddItem(new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED)));
	((BMenuItem *)firstMenu->ItemAt(3))->SetShortcut('Q', B_COMMAND_KEY);
	myMenubar->AddItem(firstMenu);
	
	
	// set up a rectangle and instantiate a new view
	// view rect should be same size as window rect but with left top at (0, 0)
	aRect.Set(0,MENU_BAR_HEIGHT -1, aWindow->Bounds().Width(), aWindow->Bounds().Height());
	aView = new View(aRect, "FooView");
	
	// add view to window
	aWindow->AddChild(aView);
	aWindow->AddChild(myMenubar);
	
	// make window visible
	aWindow->Show();
}