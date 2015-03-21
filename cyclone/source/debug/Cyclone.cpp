// ============================================================================= 
//      Cyclone.cpp                                            Created: 04/09/98 
// 
//      Contains enough code to start up a window so I can test the UI 
// 
//      History: 
//              04/09/98  mnl    conception 
// =============================================================================

#include "Cyclone.h"
#include "CyDebugWindow.h"

CyApplication::CyApplication(const char *signature)
	: BApplication(signature)
{
	SetPulseRate(3 * 1000 * 1000); // Pulse every 3 seconds	
	new CyDebugWindow(new CyConnection());	
}

CyApplication::~CyApplication()
{
}

void CyApplication::Pulse()
{
	if(CountWindows() == 0) {
		PostMessage(B_QUIT_REQUESTED);
	}
}

int main(char *argc, char *argv[]) {
	CyApplication	app(kApplicationSig);
	app.Run();
	return 0;
}