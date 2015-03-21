// ============================================================================= 
//   CyToggleMenuItem.h                                        Created: 04/25/98 
// 
//   History:
//      04/25/98  mnl    Created
// =============================================================================

#include <MenuItem.h>

#ifndef CY_TOGGLE_MENU_ITEM_H_

class CyToggleMenuItem : public BMenuItem
{
public:
						CyToggleMenuItem(const char *label,
							BMessage *message,
							char shortcut = 0,
							uint32 modifiers = 0);
						CyToggleMenuItem(BMenu *menu,
							BMessage *message = NULL);
						CyToggleMenuItem(BMessage *data);
protected:

	virtual status_t	Invoke(BMessage *message = NULL);
};

#define CY_TOGGLE_MENU_ITEM_H_ 1
#endif