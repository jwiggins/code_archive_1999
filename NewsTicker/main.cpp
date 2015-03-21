#include <Application.h>
#include "TickerWindow.h"

int main(void)
{
	BApplication app("application/x-vnd.Prok-NewsTicker");
	
	new TickerWindow();
	app.Run();
	
	return B_OK;
}