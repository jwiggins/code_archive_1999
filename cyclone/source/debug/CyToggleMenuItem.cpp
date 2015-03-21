// ============================================================================= 
//   CyToggleMenuItem.cpp                                      Created: 04/25/98 
// 
//   See CyToggleMenuItem.h for information.
// =============================================================================

#include "CyToggleMenuItem.h"

// -----------------------------------------------------------------------------
// CyToggleMenuItem -- wrapper for BMenuItem constructor
// -----------------------------------------------------------------------------
CyToggleMenuItem::CyToggleMenuItem(const char *label, BMessage *message, 
	char shortcut, uint32 modifiers)
	: BMenuItem(label, message, shortcut, modifiers)
{
}
	
// -----------------------------------------------------------------------------
// CyToggleMenuItem -- wrapper for BMenuItem constructor
// -----------------------------------------------------------------------------
CyToggleMenuItem::CyToggleMenuItem(BMenu *menu, BMessage *message)
	: BMenuItem(menu, message)
{
}
	
// -----------------------------------------------------------------------------
// CyToggleMenuItem -- wrapper for BMenuItem constructor
// -----------------------------------------------------------------------------
CyToggleMenuItem::CyToggleMenuItem(BMessage *data)
	: BMenuItem(data)
{
}
	
// -----------------------------------------------------------------------------
// Invoke -- Invoker which also toggles marked-ness
// -----------------------------------------------------------------------------
status_t CyToggleMenuItem::Invoke(BMessage *message)
{
	SetMarked(!IsMarked());
	return BMenuItem::Invoke(message);
}