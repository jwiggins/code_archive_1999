/*
	
	window.cpp
	
	John Wiggins. generic app shell. 1998
	
*/

#include "NewsWindow.h"


NewsWindow::NewsWindow(BRect frame)
				: BWindow(frame, "Article Maker", B_TITLED_WINDOW,B_NOT_RESIZABLE)
{
	BMenuBar			*menubar;
	BMenu				*mainMenu;//, *editMenu;
	BRect				rect, bounds = Bounds();
	
	menubar = new BMenuBar(rect, "menubar");
	// first menu
	mainMenu = new BMenu("Article Maker");
	BMenuItem			*about;
	//mainMenu->AddItem(new BMenuItem("Open", new BMessage(OPEN), 'O', B_COMMAND_KEY));
	mainMenu->AddItem(new BMenuItem("Save", new BMessage(SAVE), 'S', B_COMMAND_KEY));
	about = new BMenuItem("About Article Maker", new BMessage(B_ABOUT_REQUESTED), 'A', B_COMMAND_KEY);
	about->SetTarget(be_app);
	mainMenu->AddItem(about);
	mainMenu->AddItem(new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED), 'Q', B_COMMAND_KEY));
	// second menu
	//editMenu = new BMenu("Edit");
	//editMenu->AddItem(new BMenuItem("Remove Previous Article", new BMessage(REMOVE), 'R', B_COMMAND_KEY));
	
	menubar->AddItem(mainMenu);
	//menubar->AddItem(editMenu);
	AddChild(menubar);
	
	saver = new BFilePanel(B_SAVE_PANEL, new BMessenger(this));
}

bool NewsWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return(TRUE);
}

void NewsWindow::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
		case SAVE:
			saver->Show();
			break;
		case B_SAVE_REQUESTED:
			// :)
			{
				if(strlen(((BTextView *)FindView("Article"))->Text()) > 0)
					msg->AddString("Article", ((BTextView *)FindView("Article"))->Text());
				for(int i=0;i<=9;i++)
				{
					if(i < 8)
						if(strlen(((BTextControl *)FindView(attribute_names[i]))->Text()) > 0)
							msg->AddString(attribute_names[i], ((BTextControl *)FindView(attribute_names[i]))->Text());
					else
						msg->AddString(attribute_names[i], (((BCheckBox *)FindView(attribute_names[i]))->Value() ? "y" : "n"));
				}
				
				msg->what = HERE_YA_GO__SAVE_THIS;
				be_app->PostMessage(msg);
			}
			break;
		case HERE__OPEN_THIS:
			{
				msg->PrintToStream();
				
				char *ptr;
				
				if(msg->HasString("Article"))
				{
					msg->FindString("Article", &ptr);
					((BTextView *)FindView("Article"))->SetText(ptr);
					//printf("%s\nptr = %s\n", strerror(err), ptr);
					ptr=NULL;
				}
				
				for(int i=0;i<=9;i++)
				{
					if(msg->HasString(attribute_names[i]))
					{
						msg->FindString(attribute_names[i], &ptr);
						if(i < 8)
							((BTextControl *)FindView(attribute_names[i]))->SetText(ptr);
						else
						{
							if(!strcmp(ptr, "y"))
								((BCheckBox *)FindView(attribute_names[i]))->SetValue(1);
							else
								((BCheckBox *)FindView(attribute_names[i]))->SetValue(0);
						}
						//printf("%s\nptr = %s\n", strerror(err), ptr);
						ptr=NULL;
					}
				}
			}
			break;
		default:
			BWindow::MessageReceived(msg);
			break;
	}
}