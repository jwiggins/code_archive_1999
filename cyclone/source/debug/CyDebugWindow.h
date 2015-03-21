// ============================================================================= 
//    CyDebugWindow.h                                          Created: 04/25/98 
// 
//    A Debug Console for Cyclone
// 
//    History:
//       04/28/97  mnl    moved checkbox into menu
//                        added target control
//                        strip leading '/' off of parsed messages
//                        memory leak fixed
//                        added CY_DEBUG_MESSAGE type
//       04/25/98  mnl    created
// =============================================================================

#include <Window.h>

#include "CyConnection.h"

#ifndef CY_DEBUG_WINDOW_H_

class CyDebugWindow : public BWindow
{
public:
						CyDebugWindow(CyConnection *connection, 
							BRect frame = BRect(50, 50, 500, 400));
						~CyDebugWindow();
					
	void				MessageReceived(BMessage *message);
	void				FrameResized(float width, 
							float height);

private:
	CyConnection		*_connection;
};

#define CY_DEBUG_WINDOW_H_ 1
#endif