#ifndef TICKER_WINDOW_H
#define TICKER_WINDOW_H

#include <Application.h>
#include <Window.h>
#include "TickerView.h"

class TickerWindow : public BWindow {
public:
				TickerWindow();
virtual bool		QuitRequested();
};

#endif