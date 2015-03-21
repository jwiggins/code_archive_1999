/*

	myFilters.cpp
	John Wiggins. Cyclone GUI. 1998
	
*/

#include "myFilters.h"
#include "myTextViews.h"
#include <string.h>

// ********************************************************
//
//	resizeFilter. keeps window resizes from freaking out.
//	( nuff said. )
// ********************************************************
resizeFilter::resizeFilter()
			: BMessageFilter(B_PROGRAMMED_DELIVERY, B_ANY_SOURCE, B_VIEW_RESIZED)
{ lastMsg = 0; }

filter_result resizeFilter::Filter(BMessage *msg, BHandler **target)
{
	if(!strcmp(((BHandler *)*target)->Name(), "_RootView")) // slow, but it doesnt break
	{														// .. far as i know ..	
		// do cool stuff here
		status_t	err;
		if(lastMsg == 0)
		{
			if((err = msg->FindInt64("when", &lastMsg)) < B_NO_ERROR)
			{
				// bad
				return B_DISPATCH_MESSAGE;
			}
			// otherwise
			return B_DISPATCH_MESSAGE;
		}
		bigtime_t	currentMsg, copy;
		if((err = msg->FindInt64("when", &currentMsg)) < B_NO_ERROR)
		{
			// bad
		}
		else
		{
			// good 
			copy = currentMsg;
			currentMsg -= lastMsg;
			if( currentMsg < 150000 ) // last msg was less than 200000 usecs ago
			{
				// dont resize
				return B_SKIP_MESSAGE;
			}
			lastMsg = copy;
			return B_DISPATCH_MESSAGE;
		}
			
	}
	return B_DISPATCH_MESSAGE;
}

// ********************************************************
//
//	focusFilter. keeps the focus on the input feild
//	* ripped from felix, even tho im not totally sure
//		how it accomplishes this
// ********************************************************

focusFilter::focusFilter()
			: BMessageFilter(B_PROGRAMMED_DELIVERY, B_ANY_SOURCE, B_KEY_DOWN)
{
}

filter_result focusFilter::Filter(BMessage *msg, BHandler **target)
{	
	// so simple its dangerous
	if(strcmp(((BHandler *)*target)->Name(), "_EntryFeild") != 0)
	{
		// if any command key is down, we want to bail immediately
		// otherwise we effective the disable most shortcuts for the
		// ENTIRE window. ouch.
		int32 modifiers;
		msg->FindInt32("modifiers", &modifiers);
		if(modifiers & B_COMMAND_KEY)
			return B_DISPATCH_MESSAGE;
		
		// the receiver is NOT the input BTextView, so **target
		// needs to be changed
		if(!((inputTextView *)((BWindow *)Looper())->FindView("_EntryFeild"))->IsFocus())
		{
			// set target to the BTextView in the entry field so that it gets this message
			*target = (inputTextView *)((BWindow *)Looper())->FindView("_EntryFeild");
			// then make it the focus view so it continues to get messages
			(((inputTextView *)((BWindow *)Looper())->FindView("_EntryFeild")))->MakeFocus(true);
		}
	}
	return B_DISPATCH_MESSAGE;
}