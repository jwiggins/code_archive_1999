#ifndef PASSCONTROL_H
#define PASSCONTROL_H

#include <TextControl.h>
#include "myFilter.h"

#define MAGIC_SIZE 64 // this is the starting length of the "actual" buffer

class PassControl : public BTextControl {
public:
					PassControl(BRect frame,
								const char *name,
								const char *label, 
								const char *initial_text, 
								BMessage *message /*,
								uint32 rmask = B_FOLLOW_LEFT | B_FOLLOW_TOP,
								uint32 flags = B_WILL_DRAW | B_NAVIGABLE*/);
								// Same as Be's Constructor
virtual				~PassControl(); // deletes filter
void				PopChar(); // deletes last character
void				PushChar(BMessage* msg); // adds a char to actual
const char			*actualText() const; // returns actual
private:

myFilter			*filter;
char				*actual;
int					actual_size; // Current size of "actual" buffer
int					length;
};

#endif
