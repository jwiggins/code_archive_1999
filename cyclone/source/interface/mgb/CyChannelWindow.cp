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
							CyTarget *handler, target_type type, BRect frame)
	: BWindow(frame, target, B_DOCUMENT_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL,  
		B_FRAME_EVENTS), _dispatcher(dispatcher), _handler(handler), _type(type)
{	
	float			width, height;
	int32			center = frame.Width() / 2;
	BBox			*tempBox;
	BFont			*monospaced;
	BRect			layout;
	BScrollView		*tempScroller;
	
	_name = new char[100];
	strncpy(_name,target,99);
	
	// Background
	layout = Bounds();
	layout.right -= B_V_SCROLL_BAR_WIDTH;
	layout.top = layout.bottom - B_H_SCROLL_BAR_HEIGHT + 1;
	tempBox = new BBox(layout, "_status", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM);
	tempBox->SetBorder(B_PLAIN_BORDER);
	AddChild(tempBox);

	layout = Bounds();
	layout.bottom -= B_H_SCROLL_BAR_HEIGHT;
	tempBox = new BBox(layout, "_background", B_FOLLOW_ALL);
	tempBox->SetBorder(B_PLAIN_BORDER);
	AddChild(tempBox);

	// Input box
	layout = tempBox->Bounds();
	layout.InsetBy(10, 10);
	layout.left -= 5;
	layout.right += 2;
	layout.top = layout.bottom - 20;
	_inputText = new BTextControl(layout, "_inputText", NULL, "",
		new BMessage(CY_SEND_INPUT), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM,
		B_WILL_DRAW | B_NAVIGABLE );
	_inputText->SetTarget(this);
	tempBox->AddChild(_inputText);
	_inputText->MakeFocus();

	// Channel Text
	layout = tempBox->Bounds();
	layout.InsetBy(10, 10);
	layout.bottom -= 30;
	layout.right -= B_V_SCROLL_BAR_WIDTH;
	_outputText = new BTextView(layout, "_channelText", layout, 
		B_FOLLOW_ALL, B_WILL_DRAW);
	_outputText->MakeEditable(false);	
	_outputText->SetWordWrap(true);
	_outputText->SetStylable(true);
	
	monospaced = new BFont();
	monospaced->SetFamilyAndStyle("ProFontISOLatin1", "Regular");
	monospaced->SetSize(9);
	_outputText->SetFontAndColor(monospaced);
	_inputText->TextView()->SetFontAndColor(monospaced);
		
	// The Scrollbars around the channel text.
	layout = _inputText->Bounds();
	layout.InsetBy(2, 2);
	_outputText->SetTextRect(layout);
	tempScroller = new BScrollView("_channelScroller", _outputText, 
		B_FOLLOW_ALL, B_WILL_DRAW, false, true);
	tempBox->AddChild(tempScroller);

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
void CyChannelWindow::PrintMessage(const istring &nick, const istring &text)
{
	_outputText->Select(_outputText->TextLength(), _outputText->TextLength());
	_outputText->SetFontAndColor(NULL, NULL, &kBlackColor);
	
	if(_dispatcher->environment.Get(NICK) == nick) {
		_outputText->SetFontAndColor(NULL, NULL, &kEchoColor);
	}

	_outputText->Insert("<");
	_outputText->Insert(nick.c_str());
	_outputText->Insert("> ");
	
	_outputText->Insert(text.c_str());
	
	_outputText->Insert("\n");
	
	_outputText->ScrollToOffset(_outputText->TextLength() - 1);
}

// -----------------------------------------------------------------------------
// PrintText
// -----------------------------------------------------------------------------
void CyChannelWindow::PrintText(const istring &text)
{
	_outputText->Select(_outputText->TextLength(), _outputText->TextLength());
	_outputText->SetFontAndColor(NULL, NULL, &kActionColor);
	
	_outputText->Insert(text.c_str());
	
	_outputText->ScrollToOffset(_outputText->TextLength() - 1);
}

// -----------------------------------------------------------------------------
// PrintAction
// -----------------------------------------------------------------------------
void CyChannelWindow::PrintAction(const istring &nick, const istring &text)
{
	_outputText->Select(_outputText->TextLength(), _outputText->TextLength());
	_outputText->SetFontAndColor(NULL, NULL, &kActionColor);
	
	if(_dispatcher->environment.Get(NICK) == nick) {
		_outputText->SetFontAndColor(NULL, NULL, &kEchoColor);
	}

	_outputText->Insert("* ");
	_outputText->Insert(nick.c_str());
	_outputText->Insert(" ");
	
	_outputText->Insert(text.c_str());
	
	_outputText->Insert("\n");
	
	_outputText->ScrollToOffset(_outputText->TextLength() - 1);
}

// -----------------------------------------------------------------------------
// QuitRequested
// -----------------------------------------------------------------------------
bool CyChannelWindow::QuitRequested()
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
	delete _handler;
	Unlock();
	return true;
}


// -----------------------------------------------------------------------------
// MessageReceived
// -----------------------------------------------------------------------------
void CyChannelWindow::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
		case CY_SEND_INPUT:
		{
			BMessage		outMessage;
			BTextView		*view = _inputText->TextView();
			int32			begin, end;
			
			view->GetSelection(&begin, &end);
			if((begin == 0) && (end == view->TextLength()) && (end != 0))
			{
				outMessage.AddString(kTargetTag,_name);				
				outMessage.what = CY_CLIENT_MESSAGE;
				outMessage.AddString(kClientTextTag, _inputText->Text());
			}
			_dispatcher->PostMessage(&outMessage);
			view->SetText("");
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

	BRect bounds = _outputText->Bounds();
	bounds.InsetBy(2, 2);
	_outputText->SetTextRect(bounds);
	
	_outputText->ScrollToOffset(_outputText->TextLength() - 1);
	
	FindView("_channelScroller")->Invalidate();
	_inputText->Invalidate(); 
}