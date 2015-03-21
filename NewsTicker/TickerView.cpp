#include "TickerView.h"

TickerView::TickerView(BRect frame)
: BView(frame, "Ticker View", B_FOLLOW_ALL, B_WILL_DRAW|B_PULSE_NEEDED)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	BuildPopUpMenu();
	
	tickerbit = new BBitmap(Bounds(), B_CMAP8, true);
	ticker = new Ticker(Bounds());
	tickerbit->AddChild(ticker);
	
	ticker_pulse = NULL;
	//ticker_pulse = new BMessageRunner(BMessenger(this), new BMessage(TICKER_PULSE), 250000,1);
	//if(ticker_pulse->InitCheck() == B_NO_ERROR)
	//	printf("The BMessageRunner is valid.\n");
}

TickerView::~TickerView()
{
	delete tickerbit;
	//delete ticker_pulse;
}

void TickerView::AttachedToWindow()
{
	BView::AttachedToWindow();
	PopUpMenu->SetTargetForItems(this);
	((BMenuItem *)PopUpMenu->ItemAt(0))->SetTarget(be_app);
}

void TickerView::MessageReceived(BMessage *msg)
{
	printf("tock\n");
	switch(msg->what)
	{
		default:
		{
			BView::MessageReceived(msg);
			break;
		}
	}
}

void TickerView::Draw(BRect update)
{
	DrawBitmap(tickerbit, update, update);
}

void TickerView::MouseDown(BPoint point)
{
	uint32 buttons = 0;
	
	GetMouse(&point, &buttons);
	if(buttons & B_SECONDARY_MOUSE_BUTTON)
	{
		ConvertToScreen(&point);
		PopUpMenu->Go(point, true, false, true);
	}
}

void TickerView::Pulse()
{
	if(tickerbit->Lock())
	{
		ticker->quasi_pulse();
		tickerbit->Unlock();
		Invalidate();
	}
}

void TickerView::LoadAddons()
{
}

void TickerView::BuildPopUpMenu()
{
	PopUpMenu = new BPopUpMenu("TickerPopUp", false, false);
	
	PopUpMenu->AddItem(new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED)));
}

void TickerView::GetNews(const char *SiteName)
{
}