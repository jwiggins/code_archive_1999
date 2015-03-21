#include <Mime.h>
#include <Application.h>
#include <Message.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <Window.h>
#include <Rect.h>
#include <Font.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

enum {
BUILD_MIME_MENU =		'bdmm',
MIME_MENU_CHANGE =		'mmnu'
};

class MimeWindow : public BWindow {
public:
						MimeWindow();
						~MimeWindow();
virtual	bool			QuitRequested();
virtual	void			MessageReceived(BMessage *msg);

private:
void					BuildMimeMenu();

BMenu					*menu;

};

MimeWindow::MimeWindow()
	: BWindow(BRect(50, 50, 249, 99), "MimeMenu", B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_NOT_ZOOMABLE|B_NOT_RESIZABLE)
{
	Show();
	PostMessage(BUILD_MIME_MENU);
}

MimeWindow::~MimeWindow()
{
}

bool MimeWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return BWindow::QuitRequested();
}

void MimeWindow::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
		case MIME_MENU_CHANGE:
		{
			const char *string_ptr, *string_ptr2;
			if(msg->FindString("attr:name", &string_ptr) == B_OK)
			{
				SetTitle(string_ptr);
			}
			if(msg->FindString("attr:public_name", &string_ptr) == B_OK)
			{
				if(msg->FindString("short_desc", &string_ptr2) == B_OK)
				{
					char *menu_new_name = (char *)malloc(strlen(string_ptr) + strlen(string_ptr2) + 4);
					sprintf(menu_new_name, "%s : %s", string_ptr2, string_ptr);
					menu->Superitem()->SetLabel(menu_new_name);
					free(menu_new_name);
				}
			}
			break;
		}
		case BUILD_MIME_MENU:
		{
			BuildMimeMenu();
			break;
		}
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}

void MimeWindow::BuildMimeMenu()
{
	BMenuField *menu_field;
	BMenu *sub_menu;
	//BMenuItem *menu_item;
	BRect rect;
	BMimeType a_type;
	BMessage super_types, types, attr_info, *menu_item_msg;
	int32 i=0,j=0;
	const char *string_ptr;
	
	menu = new BMenu("menu");
	
	rect.Set(10, 10, 189, 30);
	menu_field = new BMenuField(rect, "mimemenu", "MIME menu", menu);
	menu_field->SetDivider(be_plain_font->StringWidth("MIME menu") + 5.);
	if(Lock())
	{
		AddChild(menu_field);
		Unlock();
	}
	UpdateIfNeeded();
	
	BMimeType::GetInstalledSupertypes(&super_types);
	while(super_types.FindString("super_types", i++, &string_ptr) == B_NO_ERROR)
	{
		BMimeType::GetInstalledTypes(string_ptr, &types);
		string_ptr = NULL;
		//types.PrintToStream();
		while(types.FindString("types", j++, &string_ptr) == B_NO_ERROR)
		{
			//printf("%s\n", string_ptr);
			if(a_type.SetTo(string_ptr) < B_NO_ERROR)
				continue;
			a_type.GetAttrInfo(&attr_info);
			
			if(attr_info.IsEmpty())
				continue;
			
			//attr_info.PrintToStream();
			
			const char *attr_name, *pub_attr_name;
			char short_description[B_MIME_TYPE_LENGTH+1]; // BeBook told me it wouldn't be any longer
			int32 attr_type, index=0;
			
			if(a_type.GetShortDescription(short_description) < B_NO_ERROR)
				sub_menu = new BMenu(string_ptr);
			else
				sub_menu = new BMenu(short_description);
			
			while(attr_info.FindString("attr:public_name", index, &pub_attr_name) == B_OK)
			{
				menu_item_msg = new BMessage(MIME_MENU_CHANGE);
				attr_info.FindString("attr:name", index, &attr_name);
				attr_info.FindInt32("attr:type", index, &attr_type);
				menu_item_msg->AddString("short_desc", short_description); // this
				menu_item_msg->AddString("attr:public_name", pub_attr_name); // plus this = new menu label
				menu_item_msg->AddString("attr:name", attr_name);
				menu_item_msg->AddInt32("attr:type", attr_type);
				sub_menu->AddItem(new BMenuItem(pub_attr_name, menu_item_msg));
				menu_item_msg = NULL;
				index++;
			}
			menu_field->Menu()->AddItem(sub_menu);
			attr_info.MakeEmpty();
			/*
			attr_info members:
			
			- "attr:public_name" : string : the people friendly name of the attribute
			- "attr:name" : string : the code friendly name of the attribute
			- "attr:type" : int32 : the type_code of the attribute
			*/
			//string_ptr = NULL;
		}
		types.MakeEmpty();
		j=0;
	}
}

int main(void)
{
	BApplication app("application/x-vnd.Prok-mimemenu");
	new MimeWindow();
	app.Run();
	return B_OK;
}