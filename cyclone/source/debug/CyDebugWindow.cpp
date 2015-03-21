// ============================================================================= 
//   CyDebugWindow.cpp                                    Created: 04/25/98 
// 
//   See CyDebugWindow.h for information.
// =============================================================================

#include "CyDebugWindow.h"
#include "CyGlobals.h"
#include "CyToggleMenuItem.h"

// -----------------------------------------------------------------------------
// CyDebugWindow -- setup the debugging window
// -----------------------------------------------------------------------------
CyDebugWindow::CyDebugWindow(CyConnection *connection, BRect frame)
	: BWindow(frame, "James Bond", B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL,  
		B_FRAME_EVENTS), _connection(connection)
{
	float			width, height;
	int32			center = frame.Width() / 2;
	BBox			*tempBox;
	BFont			*monospaced;
	BMenu			*tempMenu, *enabledMenu;
	BMenuField		*tempMenuField;
	BRect			layout;
	BScrollView		*tempScroller;
	BTextControl	*tempText;
	BTextView		*tempTextView;
	
	// Background
	tempBox = new BBox(Bounds(), "_background", B_FOLLOW_ALL);
	tempBox->SetBorder(B_PLAIN_BORDER);
	AddChild(tempBox);

	// Menu field at bottom
	enabledMenu = new BMenu("Enabled Fields");
	enabledMenu->AddItem(new CyToggleMenuItem(kRawTag, NULL));
	enabledMenu->AddItem(new CyToggleMenuItem(kPrefixTag, NULL));
	enabledMenu->AddItem(new CyToggleMenuItem("command", NULL));
	enabledMenu->AddItem(new CyToggleMenuItem(kParamTag, NULL));
	enabledMenu->AddItem(new CyToggleMenuItem(kTimeTag, NULL));

	tempMenu = new BMenu("Options");
	tempMenu->AddItem(new CyToggleMenuItem("Use Parser", NULL));
	tempMenu->AddItem(enabledMenu);
	tempMenu->FindItem(kRawTag)->SetMarked(true);
	tempMenu->FindItem("Use Parser")->SetMarked(true);
	
	layout = tempBox->Bounds();
	layout.InsetBy(10, 10);
	layout.top = layout.bottom - 20;
	layout.left = layout.right - 56;
	tempMenuField = new BMenuField(layout, "_menuField", NULL,
		tempMenu, B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW);
	tempBox->AddChild(tempMenuField);

	// Input box
	layout = tempBox->Bounds();
	layout.InsetBy(10, 10);
	layout.top = layout.bottom - 20;
	layout.right = tempMenuField->Frame().left - 10;
	tempText = new BTextControl(layout, "_targetBox", "Channel Target", "#cytest",
		NULL, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW);
	tempText->SetTarget(this);
	tempText->SetDivider(80);
	tempBox->AddChild(tempText);

	// Input box
	layout = tempBox->Bounds();
	layout.InsetBy(10, 10);
	layout.left -= 5;
	layout.right += 2;
	layout.bottom -= 30;
	layout.top = layout.bottom - 20;
	tempText = new BTextControl(layout, "_inputText", NULL, "",
		new BMessage(CY_SEND_INPUT), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM);
	tempText->SetTarget(this);
	tempBox->AddChild(tempText);

	// Channel Text
	layout = tempBox->Bounds();
	layout.InsetBy(10, 10);
	layout.bottom -= 60;
	layout.right -= B_V_SCROLL_BAR_WIDTH;
	tempTextView = new BTextView(layout, "_channelText", layout, 
		B_FOLLOW_ALL, B_WILL_DRAW);
	layout = tempTextView->Bounds();
	layout.InsetBy(2, 2);
	tempTextView->SetTextRect(layout);
	tempTextView->MakeEditable(false);	
	tempTextView->SetWordWrap(true);
	tempTextView->SetStylable(true);
	
	monospaced = new BFont();
	monospaced->SetFamilyAndStyle("ProFontISOLatin1", "Regular");
	monospaced->SetSize(9);
	tempTextView->SetFontAndColor(monospaced);
	tempText->TextView()->SetFontAndColor(monospaced);
		
	// The Scrollbars around the channel text.
	layout = tempBox->Bounds();
	layout.InsetBy(2, 2);
	tempTextView->SetTextRect(layout);
	tempScroller = new BScrollView("_channelScroller", tempTextView, 
		B_FOLLOW_ALL, B_WILL_DRAW, false, true);
	tempBox->AddChild(tempScroller);

	SetSizeLimits(250, 1280, 200, 960);
	Show();
}

// -----------------------------------------------------------------------------
// ~CyDebugWindow -- clean up
// -----------------------------------------------------------------------------
CyDebugWindow::~CyDebugWindow()
{
}

// -----------------------------------------------------------------------------
// MessageReceived
// -----------------------------------------------------------------------------
void CyDebugWindow::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
		case CY_DEBUG_MESSAGE:
		{
			BTextView		*view = (BTextView *) FindView("_channelText");
			char			*text;
			message->FindString(kDebugTag, &text);

			// Move to the end so we append instead of insert.
			view->Select(view->TextLength(), view->TextLength());
			view->Insert("DEBUG : ");
			view->Insert(text);
			view->Insert("\n");

			// Scroll to the end.
			view->ScrollToOffset(view->TextLength() - 1);
			break;
		}
		
		case CY_SERVER_MESSAGE:
		case CY_CLIENT_COMMAND:
		{
			BMenu			*menu;
			BMenuField		*field = (BMenuField *) FindView("_menuField");
			BTextView		*view = (BTextView *) FindView("_channelText");
			menu = field->Menu();

			rgb_color		orange = {255, 85, 0, 255};
			rgb_color		purple = {128, 0, 128, 255};
			rgb_color		green = {0, 128, 0, 255};
			rgb_color		red = {196, 0, 0, 255};
			rgb_color		blue = {0, 0, 128, 255};
			rgb_color		black = {0, 0, 0, 255};
			
			// Move to the end so we append instead of insert.
			view->Select(view->TextLength(), view->TextLength());
			
			// Raw irc codes.
			if(menu->FindItem(kRawTag)->IsMarked()) {
				char 		*text;
				message->FindString(kRawTag, &text);
				view->SetFontAndColor((BFont *)NULL, 0, &blue); 
				view->Insert("     ");
				view->Insert(kRawTag);
				view->SetFontAndColor((BFont *)NULL, 0, &black); 
				view->Insert(" : ");
				if(text != NULL) {
					view->Insert(text);
				} else {
					view->Insert("<none>");
				}
				view->Insert("\n");
			}
			
			// The prefix (server).
			if(menu->FindItem(kPrefixTag)->IsMarked()) {
				char 		*text;
				message->FindString(kPrefixTag, &text);
				view->SetFontAndColor((BFont *)NULL, 0, &orange); 
				view->Insert("  ");
				view->Insert(kPrefixTag);
				view->SetFontAndColor((BFont *)NULL, 0, &black); 
				view->Insert(" : ");
				if(text != NULL) {
					view->Insert(text);
				} else {
					view->Insert("<none>");
				}
				view->Insert("\n");
			}

			// Print the command tag if it is present.
			// If it's not, then print the numeric.
			if(menu->FindItem("command")->IsMarked()) {
				if(message->HasString(kCommandTag)) {
					char 		*text;
					message->FindString(kCommandTag, &text);
					view->SetFontAndColor((BFont *)NULL, 0, &purple); 
					view->Insert(" ");
					view->Insert(kCommandTag);
					view->SetFontAndColor((BFont *)NULL, 0, &black); 
					view->Insert(" : ");
					view->Insert(text);
					view->Insert("\n");
				} else {
					int8			numeric;
					char 		*text = new char[100];
					message->FindInt8(kNumericTag, &numeric);
					sprintf(text, "%d", numeric);
					view->SetFontAndColor((BFont *)NULL, 0, &purple); 
					view->Insert(" ");
					view->Insert(kNumericTag);
					view->SetFontAndColor((BFont *)NULL, 0, &black); 
					view->Insert(" : ");
					view->Insert(text);
					view->Insert("\n");
					delete text;
				}
			}

			// Print out all the param tags.
			if(menu->FindItem(kParamTag)->IsMarked()) {
				int32		i(0);
				char 		*text;
				char 		*number = new char[100];
				while(message->FindString(kParamTag, i++, &text) == B_OK) {
					sprintf(number, "%d", i);
					view->SetFontAndColor((BFont *)NULL, 0, &green); 
					view->Insert(kParamTag);
					view->Insert("[");
					view->Insert(number);
					view->Insert("] : ");
					view->SetFontAndColor((BFont *)NULL, 0, &black);
					view->Insert(text);
					view->Insert("\n");
				}
				if(i == 1) {
					view->SetFontAndColor((BFont *)NULL, 0, &green); 
					view->Insert(kParamTag);
					view->Insert("[0] : ");
					view->SetFontAndColor((BFont *)NULL, 0, &black);
					view->Insert("<none>");
					view->Insert("\n");
				}	
			}

			// Print out the raw time, which should probably be
			// niced up a bit.
			if(menu->FindItem(kTimeTag)->IsMarked()) {
				int64		time;
				char 		*text = new char[100];
				message->FindInt64(kTimeTag, &time);
				sprintf(text, "%d", time);
				view->SetFontAndColor((BFont *)NULL, 0, &red); 
				view->Insert("    ");
				view->Insert(kTimeTag);
				view->SetFontAndColor((BFont *)NULL, 0, &black); 
				view->Insert(" : ");
				view->Insert(text);
				view->Insert("\n");
				delete text;
			}
			
			// Scroll to the end.
			view->ScrollToOffset(view->TextLength() - 1);
			delete message;
			break;
		}
		
		case CY_SEND_INPUT:
		{
			BMenu			*menu;
			BMenuField		*field = (BMenuField *) FindView("_menuField");
			BMessage		outMessage;
			BTextControl	*input = (BTextControl *) FindView("_inputText");
			BTextControl	*channel = (BTextControl *) FindView("_targetBox");
			BTextView		*view = input->TextView();
			int32			begin, end;
			
			menu = field->Menu();
			view->GetSelection(&begin, &end);
			outMessage.AddString(kTargetTag, channel->Text());
			
			if((begin == 0) && (end == view->TextLength()) && (end != 0)) {
				if(menu->FindItem("Use Parser")->IsMarked()) {
					if(view->Text()[0] == '/') {
						outMessage.what = CY_CLIENT_COMMAND;				
						outMessage.AddString(kCmdLineTag, view->Text() + 1);
					} else {
						outMessage.what = CY_CLIENT_MESSAGE;
						outMessage.AddString(kCmdLineTag, view->Text());
					}
				} else {
					outMessage.AddString(kRawTag, view->Text());
				}

				input->SetText("");
				
				_connection->PostMessage(&outMessage);
			}
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
void CyDebugWindow::FrameResized(float width, float height)
{
	BWindow::FrameResized(width, height);

	BRect			bounds;
	BTextView		*view = (BTextView *) FindView("_channelText");
	bounds = view->Bounds();
	bounds.InsetBy(2, 2);
	view->SetTextRect(bounds);

	FindView("_channelScroller")->Invalidate();
	FindView("_inputText")->Invalidate();
	FindView("_targetBox")->Invalidate();
}