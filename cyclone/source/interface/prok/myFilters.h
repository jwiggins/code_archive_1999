/*

	myFilters.h
	John Wiggins. Cyclone GUI. 1998
	
*/

#ifndef MY_FILTERS_H
#define MY_FILTERS_H


#include <MessageFilter.h>

// resizeFilter
class resizeFilter : public BMessageFilter {
public:

							resizeFilter();
virtual	filter_result		Filter(BMessage *msg, BHandler **target);

private:
bigtime_t					lastMsg;
};

// focusFilter
class focusFilter : public BMessageFilter {
public:

							focusFilter();
virtual	filter_result		Filter(BMessage *msg, BHandler **target);

};


#endif