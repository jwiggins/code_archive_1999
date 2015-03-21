/*
	
	AddAttrWindow.cpp
	John Wiggins 1998
	
*/

#include "AddAttrWindow.h"

AddAttrWindow::AddAttrWindow(BRect frame, const char *title, BLooper *addon_manager, BLooper *attr_manager)
	: BWindow(frame, title, B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_NOT_ZOOMABLE|B_NOT_RESIZABLE|B_NOT_CLOSABLE), current_window_id(-1)
{
	BRect rect;
	BFont font(be_plain_font);
	int32 interfaceUnit = ((int32)font.Size());
	BView *background = new BView(Bounds(), "background", B_FOLLOW_ALL, B_WILL_DRAW);
	BTextControl *attribute_name;
	BMenu *attribute_types_menu;
	BMenuField *attribute_types_menu_field;
	BButton *ok_button, *cancel_button;
	BMessage *type_msg;
	const char *string_ptr;
	char *allocd_string;
	
	// the addon manager BMessenger
	AddonMessenger = new BMessenger(addon_manager);
	// the attribute manager BMessenger
	AttributeMessenger = new BMessenger(attr_manager);
	
	// the background
	background->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(background);
	// the "Name: " text entry field
	string_ptr = ((AttrApp *)be_app)->res_strings->String(STRING_NAME);
	allocd_string = (char *)malloc(strlen(string_ptr) + 3);
	sprintf(allocd_string, "%s: ", string_ptr);
	rect.Set(interfaceUnit, interfaceUnit, frame.Width() - interfaceUnit, interfaceUnit*3);
	attribute_name = new BTextControl(rect, "attr name", allocd_string, NULL, NULL);
	attribute_name->SetDivider(background->StringWidth(allocd_string));
	free(allocd_string);
	background->AddChild(attribute_name);
	// the types menu
	attribute_types_menu = new BMenu("");
	attribute_types_menu->SetRadioMode(true);
	attribute_types_menu->SetLabelFromMarked(true);
	type_msg = new BMessage(TYPE_MENU_CHANGE);
	type_msg->AddInt32("type", B_STRING_TYPE);
	type_msg->AddInt32("size", sizeof(char));
	string_ptr = ((AttrApp *)be_app)->res_strings->String(STRING_STRING);
	attribute_types_menu->AddItem(new BMenuItem(string_ptr, type_msg)); // string
	type_msg = new BMessage(TYPE_MENU_CHANGE);
	type_msg->AddInt32("type", B_MIME_STRING_TYPE);
	type_msg->AddInt32("size", sizeof(char));
	string_ptr = ((AttrApp *)be_app)->res_strings->String(STRING_MIME_STRING);
	attribute_types_menu->AddItem(new BMenuItem(string_ptr, type_msg)); // mime string
	type_msg = new BMessage(TYPE_MENU_CHANGE);
	type_msg->AddInt32("type", B_BOOL_TYPE);
	type_msg->AddInt32("size", sizeof(bool));
	string_ptr = ((AttrApp *)be_app)->res_strings->String(STRING_BOOL);
	attribute_types_menu->AddItem(new BMenuItem(string_ptr, type_msg)); // bool
	type_msg = new BMessage(TYPE_MENU_CHANGE);
	type_msg->AddInt32("type", B_INT32_TYPE);
	type_msg->AddInt32("size", sizeof(int32));
	string_ptr = ((AttrApp *)be_app)->res_strings->String(STRING_INT32);
	attribute_types_menu->AddItem(new BMenuItem(string_ptr, type_msg)); // int32
	type_msg = new BMessage(TYPE_MENU_CHANGE);
	type_msg->AddInt32("type", B_INT64_TYPE);
	type_msg->AddInt32("size", sizeof(int64));
	string_ptr = ((AttrApp *)be_app)->res_strings->String(STRING_INT64);
	attribute_types_menu->AddItem(new BMenuItem(string_ptr, type_msg)); // int64
	type_msg = new BMessage(TYPE_MENU_CHANGE);
	type_msg->AddInt32("type", B_DOUBLE_TYPE);
	type_msg->AddInt32("size", sizeof(double));
	string_ptr = ((AttrApp *)be_app)->res_strings->String(STRING_DOUBLE);
	attribute_types_menu->AddItem(new BMenuItem(string_ptr, type_msg)); // double
	type_msg = new BMessage(TYPE_MENU_CHANGE);
	type_msg->AddInt32("type", B_FLOAT_TYPE);
	type_msg->AddInt32("size", sizeof(float));
	string_ptr = ((AttrApp *)be_app)->res_strings->String(STRING_FLOAT);
	attribute_types_menu->AddItem(new BMenuItem(string_ptr, type_msg)); // float
	type_msg = new BMessage(TYPE_MENU_CHANGE);
	type_msg->AddInt32("type", B_RECT_TYPE);
	type_msg->AddInt32("size", sizeof(BRect));
	string_ptr = ((AttrApp *)be_app)->res_strings->String(STRING_RECT);
	attribute_types_menu->AddItem(new BMenuItem(string_ptr, type_msg)); // rect
	attribute_types_menu->ItemAt(0)->SetMarked(true);
	current_type = B_STRING_TYPE; // start out with string
	current_size = sizeof(char); // one byte
	// the container for the types menu
	rect.Set(interfaceUnit, interfaceUnit*3, frame.Width()/2, interfaceUnit*5);
	string_ptr = ((AttrApp *)be_app)->res_strings->String(STRING_TYPE);
	allocd_string = (char *)malloc(strlen(string_ptr) + 3);
	sprintf(allocd_string, "%s: ", string_ptr);
	attribute_types_menu_field = new BMenuField(rect, "type menu", allocd_string, attribute_types_menu);
	attribute_types_menu_field->SetDivider(background->StringWidth(allocd_string));
	free(allocd_string);
	background->AddChild(attribute_types_menu_field);
	// the cancel button
	string_ptr = ((AttrApp *)be_app)->res_strings->String(STRING_CANCEL);
	rect.Set(frame.Width()/2, frame.Height() - interfaceUnit*3, ((frame.Width()/2 + frame.Width()/4) - interfaceUnit), frame.Height() - interfaceUnit);
	cancel_button = new BButton(rect, "cancel", string_ptr, new BMessage(ADD_ATTR_CANCEL));
	background->AddChild(cancel_button);
	// the ok button
	string_ptr = ((AttrApp *)be_app)->res_strings->String(STRING_OK);
	rect.Set((frame.Width()/2 + frame.Width()/4), frame.Height() - interfaceUnit*3, (frame.Width() - interfaceUnit), frame.Height() - interfaceUnit);
	ok_button = new BButton(rect, "ok", string_ptr, new BMessage(ADD_ATTR_OK));
	ok_button->MakeDefault(true);
	background->AddChild(ok_button);
	
	// Ick. only way to start my message loop
	Show();
	Hide();
}

void AddAttrWindow::MessageReceived(BMessage *msg)
{
	//msg->PrintToStream();
	switch(msg->what)
	{
		case TYPE_MENU_CHANGE:
		{
			// user selected a new type from the type menu
			// msg contains:
			// "type" - type_code
			int32 type, size;
			if(msg->FindInt32("type", &type) == B_NO_ERROR)
				if(msg->FindInt32("size", &size) == B_NO_ERROR)
				{
					current_type = (type_code)type;
					current_size = (size_t)size;
				}
			//printf("current_type = \'%c%c%c%c\'\n",((current_type>>24) & 255),((current_type>>16) & 255),((current_type>>8) & 255),(current_type & 255));
			break;
		}
		case MAKE_NEW_ATTRIBUTE:
		{
			// msg contains:
			// "win id" - the id of the window that wants a new attribute - int32
			// action:
			// - Show() ourselves
			// - set current_window_id to "win id"
			int32 win_id;
			
			if(msg->FindInt32("win id", &win_id) == B_NO_ERROR)
			{
				current_window_id = win_id;
				
			}
			Show();
			break;
		}
		case ADD_ATTR_OK:
		{
			// send the data back to current_window_id
			// msg.what will equal ADD_NEW_ATTRIBUTE
			// add feilds:
			// "name" - name of the attribute
			// "attribute" - the attribute - (starts out empty)
			// "type" - the type_code of the attribute
			// "addon id" - the image_id of the addon that can handle the attribute (ask the addon manager)
			BTextControl *name_text_control = (BTextControl *)FindView("attr name");
			BMessage add_attribute(ADD_NEW_ATTRIBUTE);
			const char *name;
			char *attribute; // "attribute" - fill with 0s
			type_code type;
			
			type = current_type;
			
			// fill attribute
			attribute = new char [current_size];
			memset((void *)attribute, 0, current_size);
			
			if(name_text_control != NULL)
			{	
				name = name_text_control->Text();
				if(name != NULL)
				{
					// add stuff to the message
					add_attribute.AddInt32("type", type);
					add_attribute.AddInt32("win id", current_window_id);
					add_attribute.AddString("attr name", name);
					add_attribute.AddData("attribute", type, (void *)attribute, current_size, false);
					AttributeMessenger->SendMessage(&add_attribute);
				}
			}
			
			delete [] attribute;
			
			current_window_id = -1;
			Hide();
			break;
		}
		case ADD_ATTR_CANCEL:
		{
			// flush all views and Hide()
			((BTextControl *)FindView("attr name"))->SetText("");
			current_window_id = -1;
			Hide();
			break;
		}
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}