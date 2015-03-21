#ifndef ABOUT_WINDOW_H
#define ABOUT_WINDOW_H

// app kit
#include <Application.h>
// interface kit
#include <Window.h>
#include <Button.h>
// storage kit
#include <Resources.h>
// media kit
#include <MediaRoster.h>
#include <Sound.h>
#include <SoundPlayer.h>
// ansi
#include <malloc.h>
#include <string.h>
#include <stdio.h>
// muh own
#include "MessageDefs.h"
#include "AboutView.h"
#include "AttrApp.h"
#include "AppResStrings.h"
#include "AppResStringDefs.h"

class JellikitAboutWindow : public BWindow {
public:
						JellikitAboutWindow(BRect frame);
virtual	bool			QuitRequested();
virtual	void			MessageReceived(BMessage *msg);

private:
char					*sound_ptr;
size_t					rez_size;
bool					media_server_is_alive;
};

#endif