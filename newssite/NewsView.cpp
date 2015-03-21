/*
	
	view.cpp
	
	John Wiggins. generic app shell. 1998
	
*/

#include "NewsView.h"

NewsView::NewsView(BRect rect, char *name)
	   	   : BView(rect, name, B_FOLLOW_ALL, B_WILL_DRAW)
{
	SetViewColor(220, 220, 220);
}

void NewsView::AttachedToWindow()
{
	BTextControl		*Author=NULL, *Title=NULL, *URL=NULL, *Email=NULL, 
						*PPCDownload=NULL, *X86Download=NULL, *Source=NULL, *Day=NULL;
	BCheckBox			*New=NULL, *Publish=NULL;
	BTextView			*textOfArticle=NULL;
	BScrollView			*scrollerForText=NULL;
	//BButton				*preview, *add;
	BRect				rect, bounds = Bounds();
	BFont				font(be_plain_font);
	int32				interfaceUnit = ((int32)font.Size()), max_divider = (StringWidth("PPCDownload")+1);
	
	
	rect.Set(interfaceUnit*2, interfaceUnit*1, bounds.right - (interfaceUnit*2), interfaceUnit*3);
	Author = new BTextControl(rect, attribute_names[0], "Author", NULL, NULL);
	Author->SetAlignment(B_ALIGN_CENTER, B_ALIGN_LEFT);
	Author->SetDivider(max_divider);
	AddChild(Author);

	rect.Set(interfaceUnit*2, interfaceUnit*4, bounds.right - (interfaceUnit*2), interfaceUnit*6);
	Title = new BTextControl(rect, attribute_names[1], "Title", NULL, NULL);
	Title->SetAlignment(B_ALIGN_CENTER, B_ALIGN_LEFT);
	Title->SetDivider(max_divider);
	AddChild(Title);
	
	rect.Set(interfaceUnit*2, interfaceUnit*7, bounds.right - (interfaceUnit*2), interfaceUnit*9);
	URL = new BTextControl(rect, attribute_names[2], "URL", NULL, NULL);
	URL->SetAlignment(B_ALIGN_CENTER, B_ALIGN_LEFT);
	URL->SetDivider(max_divider);
	AddChild(URL);
	
	rect.Set(interfaceUnit*2, interfaceUnit*10, bounds.right - (interfaceUnit*2), interfaceUnit*12);
	Email = new BTextControl(rect, attribute_names[3], "Email", NULL, NULL);
	Email->SetAlignment(B_ALIGN_CENTER, B_ALIGN_LEFT);
	Email->SetDivider(max_divider);
	AddChild(Email);
	
	rect.Set(interfaceUnit*2, interfaceUnit*13, bounds.right - (interfaceUnit*2), interfaceUnit*15);
	PPCDownload = new BTextControl(rect, attribute_names[4], "PPCDownload", NULL, NULL);
	PPCDownload->SetAlignment(B_ALIGN_LEFT, B_ALIGN_LEFT);
	PPCDownload->SetDivider(max_divider);
	AddChild(PPCDownload);
	
	rect.Set(interfaceUnit*2, interfaceUnit*16, bounds.right - (interfaceUnit*2), interfaceUnit*18);
	X86Download = new BTextControl(rect, attribute_names[5], "X86Download", NULL, NULL);
	X86Download->SetAlignment(B_ALIGN_LEFT, B_ALIGN_LEFT);
	X86Download->SetDivider(max_divider);
	AddChild(X86Download);
	
	rect.Set(interfaceUnit*2, interfaceUnit*19, bounds.right - (interfaceUnit*2), interfaceUnit*21);
	Source = new BTextControl(rect, attribute_names[6], "Source", NULL, NULL);
	Source->SetAlignment(B_ALIGN_CENTER, B_ALIGN_LEFT);
	Source->SetDivider(max_divider);
	AddChild(Source);
	
	rect.Set(interfaceUnit*2, interfaceUnit*22, bounds.right - (interfaceUnit*2), interfaceUnit*24);
	Day = new BTextControl(rect, attribute_names[7], "Day", NULL, NULL);
	Day->SetAlignment(B_ALIGN_CENTER, B_ALIGN_LEFT);
	Day->SetDivider(max_divider);
	AddChild(Day);
	
	rect.Set(interfaceUnit*3, interfaceUnit*25, interfaceUnit*5 + StringWidth("New"), interfaceUnit*27);
	New = new BCheckBox(rect, attribute_names[8], "New", NULL);
	AddChild(New);
	
	rect.Set(interfaceUnit*7 + StringWidth("New"), interfaceUnit*25, interfaceUnit*9 + StringWidth("New") + StringWidth("Publish"), interfaceUnit*27);
	Publish = new BCheckBox(rect, attribute_names[9], "Publish", NULL);
	AddChild(Publish);
	
	
	rect.Set(interfaceUnit*2, interfaceUnit*28, bounds.right - (interfaceUnit*2) - B_V_SCROLL_BAR_WIDTH, bounds.bottom - interfaceUnit);
	textOfArticle = new BTextView(rect, "Article", BRect(interfaceUnit, interfaceUnit, bounds.right - (interfaceUnit*4) - B_V_SCROLL_BAR_WIDTH, interfaceUnit), B_FOLLOW_ALL, B_PULSE_NEEDED | B_WILL_DRAW);
	scrollerForText = new BScrollView("scroller", textOfArticle, B_FOLLOW_LEFT | B_FOLLOW_TOP, 0, false, true, B_FANCY_BORDER);
	AddChild(scrollerForText);
	
	
	
	//rect.Set(bounds.right - StringWidth("Add") - StringWidth("Preview") - (interfaceUnit*7), bounds.bottom - (interfaceUnit*3), bounds.right - StringWidth("Add") - (interfaceUnit*5), bounds.bottom - interfaceUnit);
	//preview = new BButton(rect, "preview", "Preview", new BMessage(PREVIEW));
	//AddChild(preview);
	
	//rect.Set(bounds.right - StringWidth("Add") - (interfaceUnit*4), bounds.bottom - (interfaceUnit*3), bounds.right - (interfaceUnit*2), bounds.bottom - interfaceUnit);
	//add = new BButton(rect, "add", "Add", new BMessage(ADD));
	//add->MakeDefault(true);
	//AddChild(add);
	
}

/*

	TrackerBase Attributes
	
	Name:			Type:		Example:
	-----------------------------------------------------------
	Author			String		"Jake Latham"
	Category		String		?
	Title			String		"Team BOSC Kicks some more ass"
	URL				String		"http://rc5stats.distributed.net/"
	Email			String		"john@ubermensch.net"
	New				String		"y" or "n" (boolean)
	AdKey			String		?
	Project			String		?
	Publish			String		"y" or "n" (boolean)
	PPCDownload		String		"http://members.tripod.com/~random___boy/BeOS/becheckpoint.zip"
	X86Download		String		"http://members.tripod.com/~random___boy/BeOS/becheckpoint.zip"
	Source			String		"BOSC"
	Day				String		"Wednesday, August 26"
	
*/