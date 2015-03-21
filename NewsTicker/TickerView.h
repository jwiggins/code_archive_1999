#ifndef TICKER_VIEW_H
#define TICKER_VIEW_H

#include <Application.h>
#include <MessageRunner.h>
#include <View.h>
#include <Bitmap.h>
#include <PopUpMenu.h>
#include <MenuItem.h>
#include <stdio.h>

#include "Ticker.h"
//#include "NewsGrabber.h"

class TickerView : public BView {
public:
				TickerView(BRect frame);
				~TickerView();
virtual void		AttachedToWindow();
virtual void		MessageReceived(BMessage *msg);
virtual void		Draw(BRect);
virtual void		MouseDown(BPoint point);
virtual void		Pulse();

private:
void				LoadAddons();
void				BuildPopUpMenu();
void				GetNews(const char *SiteName);


BList			*AddonList;
BPopUpMenu		*PopUpMenu;
BBitmap			*tickerbit;
Ticker			*ticker;
BMessageRunner	*ticker_pulse;
//NewsGrabber		*grabber;
};

#endif