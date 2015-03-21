/*
	
	main.h

*/

#ifndef MAIN_H
#define MAIN_H

#include <Application.h>
#include <Alert.h>
#include <File.h>
#include <Path.h>
#include <FindDirectory.h>
#include "BCPWindow.h"
#include "BCPMessages.h"
#include "LoginView.h"

class BCPApp : public BApplication {

public:
					BCPApp();
virtual	void		MessageReceived(BMessage *msg);
virtual	bool		QuitRequested();
virtual	void		ReadyToRun();
BWindow	*			FindWindow(const char *title);
void				kill_apps();
void				unkill_apps(); // :P
void				set_pass(const char *pass);
status_t			read_pass();
status_t			write_pass(const char *pass);
status_t			change_pass(const char *old, const char *_new);
bool				ok_to_quit() { return success; } const;

private:
char				*_password;
// booleans
bool				mail_daemon, success;
};

#endif
