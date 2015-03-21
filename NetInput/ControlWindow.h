#ifndef CONTROL_WINDOW_H
#define CONTROL_WINDOW_H

#include <Application.h>

#include <Window.h>
#include <TextControl.h>
#include <Button.h>
#include <CheckBox.h>
#include <View.h>

#include <OS.h>

#include <netdb.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>


// filter messages
enum {
	FILTER_START = 'strt',
	FILTER_STOP = 'stop',
	FILTER_LOCKOUT = 'lock'
};

// local messages
enum {
TEXT_CONTROL = 'text',
START_BUTTON = FILTER_START,
STOP_BUTTON = FILTER_STOP,
LOCK_INPUT = FILTER_LOCKOUT
};

status_t call_device(port_id devicePort, BMessage *event);

class ControlWindow : public BWindow {
public:
							ControlWindow();

virtual bool	QuitRequested();
virtual void	MessageReceived(BMessage *msg);

private:
port_id commandport;
};

#endif