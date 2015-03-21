#include "TickerWindow.h"

TickerWindow::TickerWindow()
: BWindow(BRect(0,0,499,39), "News Ticker", B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_NOT_ZOOMABLE|B_NOT_RESIZABLE)
{
	AddChild(new TickerView(Bounds()));
	SetPulseRate(250000);
	Show();
}

bool TickerWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}