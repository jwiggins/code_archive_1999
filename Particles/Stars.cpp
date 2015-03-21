/*
	
	Stars.cpp
	
	by Pierre Raynaud-Richard.
	
	Copyright 1998 Be Incorporated, All Rights Reserved.
	
*/

#ifndef STAR_WINDOW_H
#include "ParticleWindow.h"
#endif
#ifndef STARS_H
#include "Stars.h"
#endif

#include <Debug.h>

main()
{	
	StarsApp *myApplication;

	myApplication = new StarsApp();
	myApplication->Run();
	
	delete(myApplication);
	return(0);
}

StarsApp::StarsApp() : BApplication("application/x-vnd.Prok-Particles")
{
	aWindow = new ParticleWindow(BRect(120, 150, 540, 420), "Particles");
	
	// showing the window will also start the direct connection. If you
	// Sync() after the show, the direct connection will be established
	// when the Sync() return (as far as any part of the content area of
	// the window is visible after the show).
	aWindow->Show();
}
