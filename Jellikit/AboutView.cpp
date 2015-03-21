#include "AboutView.h"

AboutView::AboutView(BRect frame)
	:BView(frame, "Jellikit!", B_FOLLOW_ALL, B_WILL_DRAW)
{
	BRect bounds = Bounds(), rect;
	BFont font(be_plain_font);
	int32 interfaceUnit = ((int32)font.Size());
	font_height view_font_height;
	BButton *button;
	rgb_color black = {0,0,0,255};
	const char *string_ptr;
	
	GetFont(&font);
	font.SetSize(42.0);
	font.GetHeight(&view_font_height);
	// the rect enclosing the J in Jellikit
	magic_rect.Set(5, (Bounds().Height()/4)-view_font_height.ascent, 5+(font.StringWidth("J")), Bounds().Height()/4);
	
	string_ptr = ((AttrApp *)be_app)->res_strings->String(STRING_OK); // grab the "OK" string
	rect.Set(bounds.right - interfaceUnit - (StringWidth(string_ptr)*3), bounds.bottom - interfaceUnit*3, bounds.right - interfaceUnit, bounds.bottom - interfaceUnit);
	button = new BButton(rect, "button", string_ptr, new BMessage(B_QUIT_REQUESTED), B_FOLLOW_ALL, B_WILL_DRAW);
	button->MakeDefault(true);
	AddChild(button);
	string_ptr = NULL;
	
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	SetHighColor(black);
	SetLowColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	// build the version string
	string_ptr = ((AttrApp *)be_app)->res_strings->String(STRING_VERSION); // grab the "Version" string
	version_string = (char *)malloc(strlen(string_ptr) + strlen(JELLIKIT_VERSION) + 2);
	sprintf(version_string, "%s %s", string_ptr, JELLIKIT_VERSION);
}

AboutView::~AboutView()
{
	free(version_string);
}

void AboutView::MouseDown(BPoint point)
{
	key_info kinfo;
	uint32 buttons = 0;
	get_key_info(&kinfo);
	GetMouse(&point, &buttons, false);
	
	// muhahahahahah
	if(buttons & B_SECONDARY_MOUSE_BUTTON)
	{
		if(kinfo.modifiers & B_COMMAND_KEY)
		{
			if(kinfo.modifiers & B_SHIFT_KEY)
			{
				//printf("Shift key and commaned key are pressed.\n");
				if((kinfo.key_states[(0x26)>>3] & (1 << (7 - ((0x26)%8)))))
				{
					//printf("Tab key is pressed.\n");
					if(magic_rect.Contains(point))
					{
						//printf("Mouse click was inside magic_rect.\n");
						Window()->PostMessage(DO_EASTER_EGG);
					}
				}
			}
		}
	}
}

void AboutView::Draw(BRect update)
{
	BFont font;
	float cached_size;
	
	GetFont(&font);
	cached_size = font.Size();
	font.SetSize(42.0); // its the answer!
	SetFont(&font, B_FONT_SIZE);
	DrawString("Jellikit", BPoint(5, Bounds().Height()/4)); // big
	
	font.SetSize(cached_size);
	SetFont(&font, B_FONT_SIZE);
	MovePenBy((-PenLocation().x) + 10, 15);
	DrawString(version_string);
}