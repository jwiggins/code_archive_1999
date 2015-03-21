/*
	
	myTextViews.cpp
	
	John Wiggins. Cyclone GUI. 1998
	
*/

#include "myTextViews.h"
#include "MultiView.h"
#include "ChannelWindow.h"

/*
*
*	ircTextView class. this is the main text view in the window
*
*/
ircTextView::ircTextView(BRect frame, const char *name, BRect textRect)
			: BTextView(frame, name, textRect, be_fixed_font, NULL, 
						B_FOLLOW_ALL, B_WILL_DRAW)
{
	// we dont need the cursor to blink thankyouverymuch :)
	// this should get rid of B_PULSE_NEEDED
	SetFlags(Flags() & B_PULSE_NEEDED);
	MakeEditable(false);
	SetStylable(true);
}


/*
*
*	inputTextView class. handles all input.
*	Pasteing _will_ be supported :)
*
*/
inputTextView::inputTextView(BRect frame, const char *name)
			: BTextView(frame, name, BRect(2,2, frame.right, frame.bottom),
						be_fixed_font, NULL,
						B_FOLLOW_BOTTOM | B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW)
{
}

void inputTextView::Paste(BClipboard *clipboard)
{
	if(clipboard->Lock())
	{
		char		*data;
		char		*copy_of_data;
		ssize_t		num_bytes;
		BMessage	*msg = clipboard->Data();
		// do stuff with data here
		if(msg->FindData("text/plain", B_MIME_TYPE, &data, &num_bytes) == B_NO_ERROR)
		{
			// yay
			// make a copy
			//printf("Making copy of found data\n");
			copy_of_data = new char [ num_bytes + 1];
			memset((void *)copy_of_data, 0, num_bytes+1);
			strncpy(copy_of_data, data, num_bytes); // I hope thats how you do it. double pointers suck	
		}
		else
		{
			//printf("FindData() failed. Bailing out of Paste.\n");
			return;
		}
		// we shouldn't need the clipboard from here out
		clipboard->Unlock();
		
		//printf("FindData() succeded, heres what it got:\n%s\n", copy_of_data);
		// ok, we can parse through the paste now
		int32	counter=0, line_len=0;
		char	*line_buffer;
		while(counter <= num_bytes)
		{
			// we're looking for '\n'
			// have you seen him?
		
			if(copy_of_data[counter] == '\n')
			{
				line_buffer = new char [ line_len + 1];
				memset((void *)line_buffer, 0, line_len+1);
				strncpy(line_buffer, (copy_of_data + (counter - line_len)), line_len);
				//printf("Paste()- We found a line!\n Here it is:\n%s\n", line_buffer);
				// post the text
				// we may need to lock the window here
				if(TextLength() > 0)
				{
					// theres already some text in the view
					// append to whats there before you flush this line
					//printf("TextLength() > 0 append.\n");
					Select(TextLength(), TextLength());
					Insert(line_buffer);
					
					// send it out
					BMessage outMessage(CY_CLIENT_MESSAGE);
					outMessage.AddString(kTargetTag, ((CycloneChannelWindow *)Window())->Get_name());				
					outMessage.AddString(kClientTextTag, Text());
					((CyDispatcher *)((CycloneChannelWindow *)Window())->Get_dispatcher())->PostMessage(&outMessage);
					
					// clear the view
					SetText("");
				}
				else
				{
					BMessage outMessage(CY_CLIENT_MESSAGE);
					outMessage.AddString(kTargetTag, ((CycloneChannelWindow *)Window())->Get_name());				
					outMessage.AddString(kClientTextTag, line_buffer);
					((CyDispatcher *)((CycloneChannelWindow *)Window())->Get_dispatcher())->PostMessage(&outMessage);
				}
				
				// get ready to start over
				delete [] line_buffer;
				line_buffer = NULL;
				line_len = 0;
			}
			else
			{
				line_len++;
			}
			counter++;
		}
		
		// get that last bit of data out
		if(line_len > 0)
		{
			line_buffer = new char [ line_len + 1];
			memset((void *)line_buffer, 0, line_len+1);
			strncpy(line_buffer, (copy_of_data + (counter - line_len)), line_len);
			if((counter - line_len) == 0)
			{
				// there wasnt a single '\n' in the paste
				// append to whatever's in the View
				Select(TextLength(), TextLength());
				Insert(line_buffer);
				//printf("No endline append.\n");
			}
			else
			{
				// post the text
				// we may need to lock the window here
				BMessage outMessage(CY_CLIENT_MESSAGE);
				outMessage.AddString(kTargetTag, ((CycloneChannelWindow *)Window())->Get_name());				
				outMessage.AddString(kClientTextTag, line_buffer);
				((CyDispatcher *)((CycloneChannelWindow *)Window())->Get_dispatcher())->PostMessage(&outMessage);
			}
			//clean up
			delete [] line_buffer;
			line_buffer = NULL;
		}
		
		delete [] copy_of_data;
	}
}

void inputTextView::MakeFocus(bool focused)
{
	BView::MakeFocus(focused); // i think I want BView's version instead of BTextView's
	if(focused)
	{
		// move the insertion point to the end
		Select(TextLength(), TextLength());
	}
}

void inputTextView::KeyDown(const char *bytes, int32 numBytes)
{
	switch(bytes[0])
	{
		case B_ENTER:
			// the user wants to post some text
			Window()->PostMessage(SEND_BUTTON);
			break;
		case B_UP_ARROW:
			// command history
			break;
		case B_DOWN_ARROW:
			// command history
			break;
		default:
			BTextView::KeyDown(bytes, numBytes);
			break;
	}
}

void inputTextView::Draw(BRect updateRect)
{
	// white out the view, then call the inherited drawing code
	//SetHighColor(255,255,255);
	//FillRect(Bounds());
	BTextView::Draw(updateRect);
	// draw a thin border around the edge
	//SetPenSize(1.);
	//SetHighColor(24,24,24);
	//StrokeRect(Bounds());
}