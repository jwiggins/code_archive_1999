// ============================================================================= 
//    CyChannelWindow.h                                          Created: 04/25/98 
// 
//    A Debug Console for Cyclone
// 
//    History:
//       04/25/98  mnl    created
// =============================================================================

#include <Window.h>
#include <string>

#include "CyDispatcher.h"

#ifndef CY_CHANNEL_WINDOW_H_

class CyChannelWindow : public BWindow
{
public:
						CyChannelWindow(CyDispatcher *dispatcher,
							const char *target, CyTarget *handler, target_type type,
							BRect frame = BRect(50, 50, 500, 400) );
						~CyChannelWindow();
					
	void				MessageReceived(BMessage *message);
	void				FrameResized(float width, 
							float height);
	
	void PrintText(const istring &text);							
	void PrintMessage(const istring &nick, const istring &text);
	void				PrintAction(const istring &nick, const istring &text);
	
	virtual bool		QuitRequested();
	
private:
	char				*_name;
	CyDispatcher		*_dispatcher;
	CyTarget			*_handler;
	target_type			_type;
	
	BTextControl		*_inputText;
	BTextView			*_outputText;
};

#define CY_CHANNEL_WINDOW_H_ 1
#endif