/*
	
	ChannelWindow.cpp
	
	John Wiggins. Cyclone GUI. 1998
	
*/

#include <Application.h>

#include "ChannelWindow.h"
#include "MultiView.h"
#include "CyEnvironment.h"
#include "CyConstants.h"

CycloneChannelWindow::CycloneChannelWindow(CyDispatcher *dispatcher,
										const char *name, CyTarget *handler,
										target_type type, BRect frame,
										int32 menuHeight )
	: BWindow(frame, name, B_TITLED_WINDOW , B_NOT_ZOOMABLE),
		_dispatcher(dispatcher), _handler(handler), _type(type)
{
	BMenuBar	*_MenuBar;
	BMenu		*_MainMenu;
	MultiView	*_RootView;
	
	// make a copy of the name
	_name = new char[100];
	strncpy(_name,name,99);
	
	// get the user's nick
	istring 	nick;
	nick = _dispatcher->environment.Get(NICK);
	frame.OffsetTo(B_ORIGIN);
	frame.top += menuHeight;
	
	// single class for all views. type indicates what you want it to be
	_RootView = new MultiView(frame, "_RootView", nick.c_str(), type);
	AddChild(_RootView);
	
	// make a menu
	frame.Set(0,0, frame.right, menuHeight);
	_MenuBar = new BMenuBar(frame, "_MenuBar");
	_MainMenu = new BMenu("Cyclone");
	_MainMenu->AddItem(new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED), 'Q', B_COMMAND_KEY));
	
	_MenuBar->AddItem(_MainMenu);
	// add view to window
	AddChild(_MenuBar);
	
	// construct our filter that keeps the focus on the input feild
	focusfilter = new focusFilter();
	AddCommonFilter(focusfilter);
	
	SetSizeLimits(200, 1e9, 150, 1e9);
	SetPulseRate(750000);
}

CycloneChannelWindow::~CycloneChannelWindow()
{
	delete [] _name;
	if(RemoveCommonFilter(focusfilter))
		delete focusfilter;
}

bool CycloneChannelWindow::QuitRequested()
{
	if (_type == PUBLIC)
	{
		BMessage msg(CY_CLIENT_MESSAGE);
		msg.AddString(kTargetTag,_name);				
		msg.AddString(kClientTextTag, (istring("/part ") + _name).c_str() );
		_dispatcher->PostMessage(&msg);
	}
	
	Lock();
	RemoveHandler(_handler);
	_handler->Exit();
	delete _handler;
	Unlock();
	return true;
}

void CycloneChannelWindow::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
/*		case TEXT_ENTRY:
			// the user invoked the Text entry field
			//((MainView *)FindView("kRootView"))->PostText();
			break; */
		case SEND_BUTTON:
		// the user hit the send button
		{
			inputTextView *view = (inputTextView *)FindView("_EntryFeild");
			BTextView *channel = (BTextView *)FindView("_ChannelText");

			BMessage outMessage(CY_CLIENT_MESSAGE);
			outMessage.AddString(kTargetTag,_name);				
			outMessage.AddString(kClientTextTag, view->Text());

			_dispatcher->PostMessage(&outMessage);
			view->SetText("");
			break;
		}
/*		case STUB:
			// stub message constant
			break; */
		default:
			BWindow::MessageReceived(msg);
			break;
	}
}

// --------------------------------------------------
// ChangeNick
// --------------------------------------------------
void CycloneChannelWindow::ChangeNick(const char *nick)
{
	((MultiView *)FindView("_RootView"))->ChangeNick(nick);
}
// -----------------------------------------------------------------------------
// SetName
// -----------------------------------------------------------------------------
void CycloneChannelWindow::SetName(const char *name)
{
	delete _name;
	_name = new char [ strlen(name) + 1 ];
	strcpy(_name, name);
}
// -----------------------------------------------------------------------------
// SetTopic
// -----------------------------------------------------------------------------
void CycloneChannelWindow::SetTopic(const istring &topic)
{
	if(_type == PUBLIC)	
		((MultiView *)FindView("_RootView"))->SetTopic(topic.c_str());
}
// -----------------------------------------------------------------------------
// PrintMessage
// -----------------------------------------------------------------------------
void CycloneChannelWindow::PrintMessage(const istring &nick, const istring &text)
{
	BTextView		*view = (BTextView *) FindView("_ChannelText");
	
	if(view->CountLines() > 1024)
	{
		view->Delete(0, view->OffsetAt(256));
		printf("trimming buffer.\n");
	}
		
	
	if(_dispatcher->environment.Get(NICK) == nick) {
		//view->SetFontAndColor(NULL, NULL, &kEchoColor);
	}

	view->Select(view->TextLength(), view->TextLength());
	view->Insert("<");
	view->Insert(nick.c_str());
	view->Insert("> ");
	
	view->Insert(text.c_str());
	
	view->Insert("\n");
	
	view->ScrollToOffset(view->TextLength() - 1);
}

// -----------------------------------------------------------------------------
// PrintText
// -----------------------------------------------------------------------------
void CycloneChannelWindow::PrintText(const istring &text)
{
	BTextView		*view = (BTextView *) FindView("_ChannelText");
	
	if(view->CountLines() > 1024)
	{
		view->Delete(0, view->OffsetAt(256));
		printf("trimming buffer.\n");
	}
	
	view->Select(view->TextLength(), view->TextLength());

	view->Insert(text.c_str());
	view->ScrollToOffset(view->TextLength()- 1);
}

// -----------------------------------------------------------------------------
// PrintAction
// -----------------------------------------------------------------------------
void CycloneChannelWindow::PrintAction(const istring &nick, const istring &text)
{
	BTextView		*view = (BTextView *) FindView("_ChannelText");
	
	if(view->CountLines() > 1024)
	{
		view->Delete(0, view->OffsetAt(256));
		printf("trimming buffer.\n");
	}
	if(_dispatcher->environment.Get(NICK) == nick) {
		//view->SetFontAndColor(NULL, NULL, &kEchoColor);
	}
	
	view->Select(view->TextLength(), view->TextLength());
	
	view->Insert("*");
	view->Insert(nick.c_str());
	view->Insert(" ");
	
	view->Insert(text.c_str());
	
	view->Insert("\n");
	view->ScrollToOffset(view->TextLength()- 1);	
}