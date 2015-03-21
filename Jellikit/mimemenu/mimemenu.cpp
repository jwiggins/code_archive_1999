#include <Mime.h>
#include <Application.h>
#include <Message.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <Window.h>
#include <Rect.h>

enum {
BUILD_MIME_MENU =		'bdmm',
MENU_ITEM_CHANGED =		'menu'
};

class MimeWindow : public BWindow {
public:
						MimeWindow();
						~MimeWindow();
virtual	bool			QuitRequested();
virtual	void			MessageReceived(BMessage *msg);

private:
void					BuildMimeMenu();
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
		case MENU_ITEM_CHANGED:
		{
			const char *string_ptr;
			if(msg->FindString("attr:name", &string_ptr) == B_OK)
			{
				SetTitle(string_ptr);
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
	BMenu *menu, *sub_menu;
	//BMenuItem *menu_item;
	BRect rect;
	BMimeType a_type;
	BMessage super_types, types, attr_info, *menu_item_msg;
	int32 i=0,j=0;
	const char *string_ptr;
	
	menu = new BMenu("menu");
	menu->SetRadioMode(true);
	menu->SetLabelFromMarked(true);
	
	rect.Set(10, 10, 189, 30);
	menu_field = new BMenuField(rect, "mimemenu", "MIME menu", menu);
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
			a_type.SetTo(string_ptr);
			if(a_type.InitCheck() < B_NO_ERROR)
				continue;
			a_type.GetAttrInfo(&attr_info);
			
			if(attr_info.IsEmpty())
				continue;
			
			//attr_info.PrintToStream();
			
			const char *attr_name, *pub_attr_name;
			int32 attr_type, index=0;
			
			sub_menu = new BMenu(string_ptr);
			while(attr_info.FindString("attr:public_name", index++, &pub_attr_name) == B_OK)
			{
				menu_item_msg = new BMessage(MENU_ITEM_CHANGED);
				attr_info.FindString("attr:name", &attr_name);
				attr_info.FindInt32("attr:type", &attr_type);
				menu_item_msg->AddString("attr:name", attr_name);
				menu_item_msg->AddInt32("attr:type", attr_type);
				sub_menu->AddItem(new BMenuItem(pub_attr_name, menu_item_msg));
				sub_menu->SetRadioMode(true);
				menu_item_msg = NULL;
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