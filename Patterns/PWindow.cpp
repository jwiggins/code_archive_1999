/*
	
	PWindow.cpp
	
	John Wiggins 1998.
*/

#ifndef _APPLICATION_H
#include <Application.h>
#endif

#ifndef PWINDOW_H
#include "PWindow.h"
#endif

HelloWindow::HelloWindow(BRect frame)
				: BWindow(frame, "Patterns", B_TITLED_WINDOW,B_NOT_RESIZABLE|B_NOT_ZOOMABLE)
{
	SetPulseRate(150000); // this is how often the "blit" thread gets
							// spawned to draw a new bitmap.
							// every 150000 usecs seemed ok on my 6400/180
							// but your mileage may vary
}

bool HelloWindow::QuitRequested() // unchanged from regular HelloWorld 
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return(TRUE);
}
