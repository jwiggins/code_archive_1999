#include <Application.h>

#include "ControlWindow.h"

class ControlApp : public BApplication {
public:
					ControlApp();
};

ControlApp::ControlApp()
: BApplication("application/x-vnd.Prok-NetInput")
{
	(new ControlWindow())->Show();
}

int main()
{
	ControlApp app;
	
	app.Run();
	
	return B_OK;
}