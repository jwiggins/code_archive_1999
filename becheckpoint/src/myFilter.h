#ifndef MYFILTER_H
#define MYFILTER_H

#include <MessageFilter.h>

class myFilter : public BMessageFilter {
public:
						myFilter();
virtual filter_result	Filter(BMessage *message, BHandler **target);

};
#endif
