/*
	
	MultiView.cpp
	
	John Wiggins. Cyclone GUI. 1998
	
*/

#include "MultiView.h"

// -----------------------------------------------------------------------------
// ctor
// -----------------------------------------------------------------------------
MultiView::MultiView(BRect rect, char *name, const char *nick, target_type type)
	   	   : BView(rect, name, B_FOLLOW_ALL, B_WILL_DRAW | B_FRAME_EVENTS | B_PULSE_NEEDED), _type(type)
{
	// convenience
	BRect		bounds = Bounds();
	// some font sensitive stuff
	BFont font(be_fixed_font);
	SetFont(&font);
	interfaceUnit = ((int32)font.Size());
	charwidth = ((int32)font.StringWidth(" "));
	
	// fill in the users nick
	usernick = new char [ strlen(nick) + 1 ];
	strcpy(usernick, nick);
	
	// init some shit
	resizeing = false;
	userlistwidth = 150;
	divider = bounds.right - userlistwidth;
	SetViewColor(220,220,220); // oh goth! how the world is so grey!
	
	// make stuff NULL just incase its not used in this view
	_UserList = NULL;
	_UserScrollView = NULL;
	_topic = NULL;
	resizefilter = NULL;
	
	// now for some real fun.
	// look at type. Construct views unique to that type of CyViews (hmm)
	switch(type)
	{
		// PUBLIC and PRIVATE use _basically_ the same view
		case PUBLIC:
		case PRIVATE:
			{
				// B_VIEW_RESIZED watchdog filter
				resizefilter = new resizeFilter();
				AddFilter(resizefilter);
				// left side of view *********************
				rect.Set(interfaceUnit, interfaceUnit*2, divider - (B_V_SCROLL_BAR_WIDTH+(interfaceUnit>>1)), bounds.bottom - (interfaceUnit*5.5));
				_ChannelText = new BTextView(rect,
										"_ChannelText", 
										BRect(1,1, rect.right - (interfaceUnit<<1), rect.bottom - interfaceUnit),
										 B_FOLLOW_ALL, 
										 B_WILL_DRAW | B_FRAME_EVENTS | B_PULSE_NEEDED );
				_ChannelText->MakeEditable(false); // the program does the editing Thankyouverymuch
				_ChannelText->SetFontAndColor(be_fixed_font);
				_ChannelText->SetStylable(true); // we wanna do annoying colors baby!

				_ChannelScrollView = new BScrollView("_ChannelScroller",
													 _ChannelText, 
													B_FOLLOW_TOP | B_FOLLOW_LEFT,
													B_FRAME_EVENTS,false, true, B_NO_BORDER);
				AddChild(_ChannelScrollView);
		
				// right side of view **********************************
	
				rect.Set(divider + (interfaceUnit>>1), interfaceUnit*2, bounds.right - (interfaceUnit*2.5), bounds.bottom - ((interfaceUnit*5.5) + B_H_SCROLL_BAR_HEIGHT));
				_UserList = new BListView(rect,
										"_UserList", 
										B_SINGLE_SELECTION_LIST,
										B_FOLLOW_ALL,
										B_WILL_DRAW | B_FRAME_EVENTS);
				_UserList->SetFont(be_fixed_font);
	
				_UserScrollView = new BScrollView("_UserListScroller", 
													_UserList, 
													B_FOLLOW_TOP | B_FOLLOW_LEFT,
													0,
													true, true, B_NO_BORDER);
				AddChild(_UserScrollView);
	
				// bottom of window *************************************

				rect.Set(interfaceUnit, 
						bounds.bottom - (interfaceUnit*3),
						bounds.right - interfaceUnit, 
						bounds.bottom - interfaceUnit);
				_InputText = new inputTextView(rect, "_EntryFeild");
				AddChild(_InputText);
				//_InputText->TextView()->SetFontAndColor(be_fixed_font);
				_InputText->MakeFocus();
			}
			break; // End PUBLIC View construction
		case CONSOLE:
			{
				// top of view *********************
				rect.Set(interfaceUnit, interfaceUnit, bounds.right - (interfaceUnit*2.5), bounds.bottom - (interfaceUnit*6.5));
				_ChannelText = new BTextView(rect,
										"_ChannelText", 
										BRect(1,1, rect.right - (interfaceUnit*2), rect.bottom - interfaceUnit),
										 B_FOLLOW_ALL, 
										 B_WILL_DRAW | B_FRAME_EVENTS | B_PULSE_NEEDED );
				_ChannelText->MakeEditable(false); // the program does the editing Thankyouverymuch
				_ChannelText->SetFontAndColor(be_fixed_font);
				_ChannelText->SetStylable(true); // we wanna do annoying colors baby!

				_ChannelScrollView = new BScrollView("_ChannelScroller",
													 _ChannelText, 
													B_FOLLOW_ALL,
													B_FRAME_EVENTS,false, true, B_NO_BORDER);
				AddChild(_ChannelScrollView);
	
				// bottom of window *************************************

				rect.Set(interfaceUnit, 
						bounds.bottom - (interfaceUnit*3),
						bounds.right - interfaceUnit, 
						bounds.bottom - interfaceUnit);
				_InputText = new inputTextView(rect, "_EntryFeild");
				AddChild(_InputText);
				//_InputText->TextView()->SetFontAndColor(be_fixed_font);
				_InputText->MakeFocus();
			}
			break;
		default:
			{
				// nothing for now
			}
			break;
	}
}

// -----------------------------------------------------------------------------
// dtor
// -----------------------------------------------------------------------------
MultiView::~MultiView()
{
	delete _ChannelText; // the BScrollView does not delete this for me
	delete [] usernick;
	if(_UserList)
		delete _UserList; // the BScrollView does not delete this for me
	if(_topic)
		delete [] _topic;
	if(RemoveFilter(resizefilter))
	{
		delete resizefilter;
	}
}

// -----------------------------------------------------------------------------
// FrameResized
// -----------------------------------------------------------------------------
void MultiView::FrameResized(float nWidth, float nHeight)
{
	//printf("FrameResized()\n");
	BView::FrameResized(nWidth, nHeight);
	int32 width = nWidth, height = nHeight;
	if(_type == PUBLIC || _type == PRIVATE)
	{
		if(!resizeing) resizeing = true;
		divider = width - userlistwidth;
		FindView("_ChannelScroller")->MoveTo(interfaceUnit, interfaceUnit*2);
		FindView("_UserListScroller")->MoveTo(divider + (interfaceUnit>>1), interfaceUnit*2);
		FindView("_UserListScroller")->ResizeTo(userlistwidth - (interfaceUnit+(interfaceUnit>>1)), height - interfaceUnit*7.5);
		FindView("_ChannelScroller")->ResizeTo(divider - (interfaceUnit+(interfaceUnit>>1)), height - interfaceUnit*7.5);
		BRect	channelTextFrame = _ChannelText->Frame();
		_ChannelText->SetTextRect(BRect(1,1, channelTextFrame.right - (interfaceUnit), channelTextFrame.bottom));
		_ChannelText->ScrollToOffset(_ChannelText->TextLength() - 1);
		if(_type == PUBLIC)
			DrawUsers();
		if(_topic)
			DrawTopic();
	}
	SetHighColor(ViewColor());
	FillRect(BRect(0,height - interfaceUnit*7.5, width,height - interfaceUnit*3));
	SetHighColor(0,0,0);
	DrawString(usernick, BPoint(interfaceUnit, height - interfaceUnit*4));
	DrawString(" - This space for rent :P");
	Flush();
}

// -----------------------------------------------------------------------------
// Pulse
// -----------------------------------------------------------------------------
void MultiView::Pulse()
{
	//printf("Pulse()\n");
	if(_type == PRIVATE || _type == PUBLIC)
	{
		if(resizeing)
		{
			BPoint	point;
			uint32	buttons;
			GetMouse(&point, &buttons);
			if(!buttons) // buttons are up
			{
				if((Bounds().bottom - interfaceUnit*7.5) != _UserScrollView->Frame().bottom)
					FrameResized(Bounds().right, Bounds().bottom);
				else if(_UserScrollView->Frame().right > Frame().right)
					FrameResized(Bounds().right, Bounds().bottom);
			
				// now flush the MessageQueue() of its B_VIEW_RESIZED msgs
				BMessage *kill;
				BMessageQueue *q = Window()->MessageQueue();
				while((kill = q->FindMessage(B_VIEW_RESIZED, 1)) != NULL)
				{
					q->RemoveMessage(kill);
				}
				resizeing = false;
			}
			else
			{
				return; // we wanna wait until they're done
			}
		}
	}
}

// -----------------------------------------------------------------------------
// Draw
// -----------------------------------------------------------------------------
void MultiView::Draw(BRect updateRect)
{
	//printf("Draw()\n");
	if(_type == PUBLIC)
		DrawUsers();
	if(_topic)
		DrawTopic();
	DrawString(usernick, BPoint(interfaceUnit, Bounds().bottom - interfaceUnit*4));
	DrawString(" - This space for rent :P");
	Flush();
}

// -----------------------------------------------------------------------------
// MouseDown
// -----------------------------------------------------------------------------
void MultiView::MouseDown(BPoint point)
{
	if(_type == PUBLIC || _type == PRIVATE)
	{
		// continue as scheduled
		if(find_thread("mouse poller") < 0)
		{
			// we havent started a thread yet, continue
			if((point.x > (divider - (interfaceUnit>>1))) && (point.x < (divider + (interfaceUnit>>1))))
			{
				// mmk. they clicked between the channel view and the userlist
				// check y. why not..
				if((point.y > (interfaceUnit<<1)) && (point.y < (Bounds().bottom - interfaceUnit*7.5)))
				{
					// okie dokey. we've passed all those silly conditionals, so lets actually do something!
					thread_id	tid = spawn_thread(mouse_poll_start, "mouse poller", B_NORMAL_PRIORITY, (void *)this);
					resume_thread(tid);
				}
			}
		}
	}
}

// -----------------------------------------------------------------------------
// SetTopic
// -----------------------------------------------------------------------------
void MultiView::SetTopic(const char *topic)
{
	//printf("SetTopic()\n");
	if(!_topic) // _topic is NULL
	{
		_topic = new char [ strlen(topic) + 1 ];
		strcpy(_topic, topic);
	}
	else
	{
		delete [] _topic;
		_topic = new char [ strlen(topic) + 1 ];
		strcpy(_topic, topic);
	}
	if(Window()->Lock())
	{
		DrawTopic();
		Window()->Unlock();
	}
}

// --------------------------------------------------
// ChangeNick
// --------------------------------------------------
void MultiView::ChangeNick(const char *newnick)
{
	//printf("ChangeNick()\n");
	if(!usernick) // usernick is NULL
	{
		usernick = new char [ strlen(newnick) + 1 ];
		strcpy(usernick, newnick);
	}
	else
	{
		delete [] usernick;
		usernick = new char [ strlen(newnick) + 1 ];
		strcpy(usernick, newnick);
	}
	if(Window()->Lock())
	{
		SetHighColor(ViewColor());
		FillRect(BRect(0,Bounds().bottom - interfaceUnit*7.5, Bounds().right,Bounds().bottom - interfaceUnit*3));
		SetHighColor(0,0,0);
		DrawString(usernick, BPoint(interfaceUnit, Bounds().bottom - interfaceUnit*4));
		DrawString(" - This space for rent :P");
		Flush();
		Window()->Unlock();
	}
}

// --------------------------------------------------
// DrawUsers
// --------------------------------------------------
void MultiView::DrawUsers()
{
	//printf("DrawUsers()\n");
	// Window must be locked before calling this function!
	BListView *list = (BListView *)FindView("_UserList");
	char	numUserString[13]; // find me a channel with > 99,999 users and i will change this
	sprintf(numUserString, "Users: %d", ((int32)list->CountItems())); // init the string
	int32	stringlen = strlen(numUserString);
	int32	drawnlen = (int32)(((Bounds().right - divider)/charwidth));
	
	SetHighColor(ViewColor());
	FillRect(BRect(divider,0, Bounds().right,interfaceUnit<<1));
	SetHighColor(0,0,0);
	if(drawnlen < stringlen)
	{
		DrawString(numUserString, drawnlen-3, BPoint(divider + interfaceUnit, (interfaceUnit*1.5)));
		DrawString(B_UTF8_ELLIPSIS);
	}
	else
	{
		DrawString(numUserString, drawnlen, BPoint(divider + interfaceUnit, (interfaceUnit*1.5)));
	}
	Flush();
}

// --------------------------------------------------
// DrawTopic
// --------------------------------------------------
void MultiView::DrawTopic()
{
	//printf("DrawTopic()\n");
	// Window must be locked before calling this function!
	int32 topiclen = strlen(_topic), drawnlen = (int32)(((divider  - (interfaceUnit*3))/charwidth) - 7);
	SetHighColor(ViewColor());
	FillRect(BRect(0,0, divider,interfaceUnit<<1));
	SetHighColor(0,0,0);
	DrawString("Topic: ", BPoint(interfaceUnit, (interfaceUnit*1.5)));
	if(drawnlen < topiclen)
	{
		DrawString(_topic, drawnlen-3);
		DrawString(B_UTF8_ELLIPSIS);
	}
	else
	{
		DrawString(_topic, drawnlen);
	}
	Flush();
}

// -----------------------------------------------------------------------------
// mouse_poll_start
// -----------------------------------------------------------------------------
int32 MultiView::mouse_poll_start(void *arg)
{
	MultiView	*obj = (MultiView *)arg;
	return (obj->mouse_poll());
}

// -----------------------------------------------------------------------------
// mouse_poll
// -----------------------------------------------------------------------------
int32 MultiView::mouse_poll()
{
	int32	width;
	BPoint	point, cache;
	uint32	buttons;
	if(Window()->Lock())
	{
		width = Bounds().right;
		GetMouse(&point, &buttons, true);
		Window()->Unlock();
	}
	while( buttons )
	{
		// check to see if the mouse is even moving
		if(cache.x != point.x)
		{
			// make sure the user isnt going crazy with the divider :)
			// else the op goes crazy on them...	
			if(point.x > (interfaceUnit<<3) && point.x < (width - (interfaceUnit<<2)))
			{
				if(Window()->Lock())
				{
					divider = point.x;
					DividerMoved();
					Window()->Unlock();
				}
			}
		}
		snooze(75 * 1000);
		cache = point;
		if(Window()->Lock())
		{
			GetMouse(&point, &buttons, true);
			Window()->Unlock();
		}
	}
	return B_OK;
}

// -----------------------------------------------------------------------------
// DividerMoved
// -----------------------------------------------------------------------------
void MultiView::DividerMoved()
{
	BRect	bounds = Bounds();
	userlistwidth = bounds.right - divider;
	FindView("_UserListScroller")->MoveTo(divider + (interfaceUnit>>1), interfaceUnit*2);
	FindView("_UserListScroller")->ResizeTo(userlistwidth - (interfaceUnit+(interfaceUnit>>1)), bounds.bottom - interfaceUnit*7.5);
	FindView("_ChannelScroller")->ResizeTo(divider - (interfaceUnit+(interfaceUnit>>1)), bounds.bottom - interfaceUnit*7.5);
	BRect	channelTextFrame = _ChannelText->Frame();
	_ChannelText->SetTextRect(BRect(1,1, channelTextFrame.right - (interfaceUnit), channelTextFrame.bottom));
	_ChannelText->ScrollToOffset(_ChannelText->TextLength() - 1);
	if(_type == PUBLIC)
		DrawUsers();
	if(_topic)
		DrawTopic();
}