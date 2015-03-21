/*
	
	main.cpp

*/

#ifndef PREF_WINDOW_H
#include "PrefWindow.h"
#endif
#ifndef MAIN_H
#include "main.h"
#endif

int main()
{	
	Application *myApp;

	myApp = new Application();
	myApp->Run();
	
	delete myApp;
	return 0;
}

Application::Application()
		  	: BApplication("application/x.vnd-cyclone-interfaceprefs")
{
	PrefWindow		*window;
	BRect			rect;

	// set up a rectangle and instantiate a new window
	rect.Set(100, 100, 349, 349);
	window = new PrefWindow(rect);
}