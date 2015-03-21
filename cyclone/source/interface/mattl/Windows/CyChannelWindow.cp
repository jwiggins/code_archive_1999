// ============================================================================= 
//   CyChannelWindow.cpp                                    Created: 04/25/98 
// 
//   See CyChannelWindow.h for information.
// =============================================================================

#include "CyChannelWindow.h"
#include "CyEnvironment.h"
#include "CyConstants.h"

// -----------------------------------------------------------------------------
// CyChannelWindow -- setup the channel window
// -----------------------------------------------------------------------------
CyChannelWindow::CyChannelWindow(CyDispatcher *dispatcher, const char *target,
							BRect frame)
	: BWindow(frame, target, B_DOCUMENT_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL,  
		B_FRAME_EVENTS), _dispatcher(dispatcher)
{
	float			width, height;
	int32			center = frame.Width() / 2;
	BFont			*monospaced;
	BRect			layout;
	
	_target = new char[100];
	strncpy(_target,target,99);
	
	// Background
	layout = Bounds();
	layout.right -= B_V_SCROLL_BAR_WIDTH;
	layout.top = layout.bottom - B_H_SCROLL_BAR_HEIGHT + 1;
	_status = new BBox(layout, "_status", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM);
	_status->SetBorder(B_PLAIN_BORDER);
	AddChild(_status);

	layout = Bounds();
	layout.bottom -= B_H_SCROLL_BAR_HEIGHT;
	_background = new BBox(layout, "_background", B_FOLLOW_ALL);
	_background->SetBorder(B_PLAIN_BORDER);
	AddChild(_background);

	// Input box
	layout = _background->Bounds();
	layout.InsetBy(10, 10);
	layout.left -= 5;
	layout.right += 2;
	layout.top = layout.bottom - 20;
	_input = new BTextControl(layout, "_input", NULL, "",
		new BMessage(CY_SEND_INPUT), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM);
	_input->SetTarget(this);
	_background->AddChild(_input);

	// Channel Text
	layout = _background->Bounds();
	layout.InsetBy(10, 10);
	layout.bottom -= 30;
	layout.right -= B_V_SCROLL_BAR_WIDTH;
	_channel = new BTextView(layout, "_channel", layout, 
		B_FOLLOW_ALL, B_WILL_DRAW);
	_channel->MakeEditable(false);	
	_channel->SetWordWrap(true);
	_channel->SetStylable(true);
	
	monospaced = new BFont();
	monospaced->SetFamilyAndStyle("ProFontISOLatin1", "Regular");
	monospaced->SetSize(9);

	_channel->SetFontAndColor(monospaced);
	_input->TextView()->SetFontAndColor(monospaced);
		
	// The Scrollbars around the channel text.
	layout = _background->Bounds();
	layout.InsetBy(2, 2);
	_channel->SetTextRect(layout);
	_channelScroll = new BScrollView("_channelScroll", _channel, 
		B_FOLLOW_ALL, B_WILL_DRAW, false, true);
	_background->AddChild(_channelScroll);

	SetSizeLimits(250, 1280, 200, 960);
	Show();
}

// -----------------------------------------------------------------------------
// ~CyChannelWindow -- clean up
// -----------------------------------------------------------------------------
CyChannelWindow::~CyChannelWindow()
{
}

// -----------------------------------------------------------------------------
// PrintMessage
// -----------------------------------------------------------------------------
void CyChannelWindow::PrintMessage(BMessage *msg)
{
	char 			*buf, *text;
	string			nick, lower;

	_channel->Select(_channel->TextLength(), _channel->TextLength());
	_channel->SetFontAndColor(NULL, NULL, &kBlackColor);
	
	nick = _dispatcher->Environment()->Get(kNickEnv);
	for(int i = 0; i < nick.length(); i++) {
		nick[i] = tolower(nick[i]);
	}

	if(msg->FindString(kNickTag, &buf) != B_OK) {
		msg->FindString(kPrefixTag, &buf);
	}
	
	if (msg->FindString(kParamTag, 1, &text) == B_OK) {
		lower = text;	
		for(int i = 0; i < lower.length(); i++) {
			lower[i] = tolower(lower[i]);
		}
	}

	if(search(lower.begin(), lower.end(), nick.begin(), nick.end()) != lower.end()) {
		_channel->SetFontAndColor(NULL, NULL, &kNickColor);
	}

	if(_dispatcher->Environment()->Get(kNickEnv) == buf) {
		_channel->SetFontAndColor(NULL, NULL, &kEchoColor);
	}

	for(int i = 0; i < 10 - strlen(buf); i++) {
		_channel->Insert(" ");
	}

	_channel->Insert(buf);
	_channel->Insert(": ");
	_channel->Insert(text);
	_channel->Insert("\n");
	
	_channel->ScrollToOffset(_channel->TextLength() - 1);
}

// -----------------------------------------------------------------------------
// PrintAction
// -----------------------------------------------------------------------------
void CyChannelWindow::PrintAction(BMessage *msg)
{
	char 			*buf, *text;
	string			nick, lower;

	_channel->Select(_channel->TextLength(), _channel->TextLength());
	_channel->SetFontAndColor(NULL, NULL, &kActionColor);
	
	if(msg->FindString(kNickTag, &buf) != B_OK) {
		msg->FindString(kPrefixTag, &buf);
	}
	
	nick = _dispatcher->Environment()->Get(kNickEnv);
	for(int i = 0; i < nick.length(); i++) {
		nick[i] = tolower(nick[i]);
	}

	if (msg->FindString(kParamTag, 1, &text) == B_OK) {
		lower = text;	
		for(int i = 0; i < lower.length(); i++) {
			lower[i] = tolower(lower[i]);
		}
	}

	if(search(lower.begin(), lower.end(), nick.begin(), nick.end()) != lower.end()) {
		_channel->SetFontAndColor(NULL, NULL, &kNickColor);
	}

	if(_dispatcher->Environment()->Get(kNickEnv) == buf) {
		_channel->SetFontAndColor(NULL, NULL, &kEchoColor);
	}


	_channel->Insert("          * ");
	_channel->Insert(buf);
	_channel->Insert(" ");
	_channel->Insert(text);	
	_channel->Insert("\n");
	
	_channel->ScrollToOffset(_channel->TextLength() - 1);
}

// -----------------------------------------------------------------------------
// PrintJoin
// -----------------------------------------------------------------------------
void CyChannelWindow::PrintJoin(BMessage *msg)
{
	char 				*name, *channel, *prefix;
	string				host;
	string::size_type	boundary;

	msg->FindString(kNickTag, 0, &name);
	msg->FindString(kParamTag, 0, &channel);
	msg->FindString(kPrefixTag, 0, &prefix);

	host = prefix;
	boundary = host.find_first_of('!');
	_channel->Select(_channel->TextLength(), _channel->TextLength());
	_channel->SetFontAndColor(NULL, NULL, &kModeColor);
	_channel->Insert(name);

	if(boundary != string::npos) {
		host = host.substr(boundary + 1);
		_channel->Insert(" (");
		_channel->Insert(host.c_str());
		_channel->Insert(")");
	}

	_channel->Insert(" has joined ");
	_channel->Insert(channel);
	_channel->Insert(".\n");
}

// -----------------------------------------------------------------------------
// PrintPart
// -----------------------------------------------------------------------------
void CyChannelWindow::PrintPart(BMessage *msg)
{
	char 				*name, *channel, *prefix;
	string				host;
	string::size_type	boundary;

	msg->FindString(kNickTag, 0, &name);
	msg->FindString(kParamTag, 0, &channel);
	msg->FindString(kPrefixTag, 0, &prefix);

	host = prefix;
	boundary = host.find_first_of('!');
	_channel->Select(_channel->TextLength(), _channel->TextLength());
	_channel->SetFontAndColor(NULL, NULL, &kModeColor);
	_channel->Insert(name);

	if(boundary != string::npos) {
		host = host.substr(boundary + 1);
		_channel->Insert(" (");
		_channel->Insert(host.c_str());
		_channel->Insert(")");
	}

	_channel->Insert(" has parted ");
	_channel->Insert(channel);
	_channel->Insert(".\n");
}

// -----------------------------------------------------------------------------
// MessageReceived
// -----------------------------------------------------------------------------
void CyChannelWindow::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
		case CY_SERVER_MESSAGE:
		{
			char *command;
			if (message->FindString(kCommandTag, &command) == B_OK)
			{
				if (!strcasecmp(command, "PRIVMSG"))
					PrintMessage(message);
				else if (!strcasecmp(command, "ACTION"))
					PrintAction(message);
				else if (!strcasecmp(command, "JOIN"))
					PrintJoin(message);
				else if (!strcasecmp(command, "PART"))
					PrintPart(message);
			}
			break;
		}
		
		case CY_SEND_INPUT:
		{
			BMessage		outMessage;
			int32			begin, end;
			char 			delimiter = '/';
			string			echo;
		
			
			_input->TextView()->GetSelection(&begin, &end);
			if((begin == 0) && (end == _input->TextView()->TextLength()) && (end != 0))
			{
				outMessage.AddString(kTargetTag, _target);
				
				if ( *(_input->Text()) == delimiter )
				{
					outMessage.what = CY_CLIENT_COMMAND;
					outMessage.AddString(kClientTextTag, (_input->Text())+1);
				}
				else
				{
					fprintf(stderr,"sending CY_CLIENT_MESSAGE\n");
					outMessage.what = CY_CLIENT_MESSAGE;
					outMessage.AddString(kClientTextTag, _input->Text());
				}
			}
			_dispatcher->PostMessage(&outMessage);
			_input->SetText("");
			break;
		}
		
		default:
		{
			BWindow::MessageReceived(message);
			break;
		}
	}
}

// -----------------------------------------------------------------------------
// FrameResized -- force redraw of certain things on resize
//                 update the text frame
// -----------------------------------------------------------------------------
void CyChannelWindow::FrameResized(float width, float height)
{
	BWindow::FrameResized(width, height);

	BRect			bounds;
	bounds = _channel->Bounds();
	bounds.InsetBy(2, 2);
	_channel->SetTextRect(bounds);
	
	_channel->Invalidate();
	_input->Invalidate();
}

// -----------------------------------------------------------------------------
// QuitRequested 
// -----------------------------------------------------------------------------
bool CyChannelWindow::QuitRequested()
{
	BMessage	msg(CY_CLIENT_COMMAND);
	string		text;
	
	text = "part ";
	text.append(_target);
	msg.AddString(kClientTextTag, text.c_str());
	_dispatcher->PostMessage(&msg);
	return false;
}
