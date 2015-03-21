/*
	
	view.h
	
	John Wiggins. generic app shell. 1998
	
*/

#ifndef NEWS_VIEW_H
#define NEWS_VIEW_H

#include <View.h>
#include <TextControl.h>
#include <TextView.h>
#include <CheckBox.h>
#include <ScrollView.h>
#include <Font.h>
#include <Rect.h>

extern const char *attribute_names[];

enum
{
SAVE = 'save',
SEND_PARCEL_TO_BE_APP = 'spba',
HERE_YA_GO__SAVE_THIS = 'hygs',
HERE__OPEN_THIS = 'hoth'
};

class NewsView : public BView {

public:
					NewsView(BRect frame, char *name);
virtual	void		AttachedToWindow();

private:
};

#endif
