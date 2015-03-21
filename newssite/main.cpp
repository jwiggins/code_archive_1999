/*
	
	main.cpp
	
	John Wiggins. generic app shell. 1998
	
*/

#include "main.h"

main()
{	
	NewsApp *myApplication;

	myApplication = new NewsApp();
	myApplication->Run();
	
	delete(myApplication);
	return(0);
}

NewsApp::NewsApp()
		: BApplication("application/x.vnd-Prok.ArticleMaker")
{
	NewsWindow			*aWindow;
	NewsView			*aView;
	BRect				aRect;
	menu_info			m_info;
	int32				menubarheight=0;
	
	// Get the height of a menu given the current menu settings
	get_menu_info(&m_info);
	menubarheight = (int32)m_info.font_size + 8;
	
	// set up a rectangle and instantiate a new window
	aRect.Set(100, 24, 539, 449 + menubarheight);
	aWindow = new NewsWindow(aRect);
	
	// set up a rectangle and instantiate a new view
	// view rect should be same size as window rect but with left top at (0, 0)
	aRect.OffsetTo(B_ORIGIN);
	aRect.top += menubarheight;
	aView = new NewsView(aRect, "main view");
	
	// add view to window
	aWindow->AddChild(aView);
	
	// set the cwd to the app's home directory
	SetCurrentDirectory();
	
	// make window visible
	aWindow->Show();
}

NewsApp::~NewsApp()
{
}


/////////////////////////////////////////////////////////////////////////////////////////////
//
//  QuitRequested
///////////////////
bool NewsApp::QuitRequested()
{
	return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
//  AboutRequested
///////////////////

void NewsApp::AboutRequested()
{
	(new BAlert("about", 
	"Article Maker v1.0\nBy John Wiggins 1998.\n\n"
	"A perfect complement to TrackerBase by Scot Hacker.\n",
	 "neat"))->Go();
}


/////////////////////////////////////////////////////////////////////////////////////////////
//
//  RefsReceived
///////////////////

void NewsApp::RefsReceived(BMessage *msg)
{
	BEntry		entry;
	BPath		path;
	entry_ref	file_ref;
	status_t	err;
	
	if(msg->FindRef("refs", &file_ref) == B_OK)
	{
		entry.SetTo(&file_ref);
		if(entry.InitCheck() == B_NO_ERROR)
		{
			if(entry.GetPath(&path) == B_NO_ERROR)
			{
				BFile file(path.Path(), B_READ_ONLY);
				BMessage *parcel = new BMessage(HERE__OPEN_THIS);
				off_t filesize=0;
				char *filebuffer;
				
				if(file.InitCheck() == B_NO_ERROR)
				{
					file.GetSize(&filesize);
					
					filebuffer = new char [ filesize + 1 ];
					if(file.ReadAt(0, (void *)filebuffer, filesize) == filesize)
						parcel->AddString("Article", filebuffer);
					delete [] filebuffer;
					filebuffer = NULL;
					
					attr_info a_info;
					for(int i=0;i<=9;i++)
					{
						if(file.GetAttrInfo(attribute_names[i], &a_info) == B_NO_ERROR)
						{
							char *buffer = new char [ a_info.size + 1 ];
							if(file.ReadAttr(attribute_names[i], B_STRING_TYPE, 0, (void *)buffer, a_info.size) == a_info.size)
								parcel->AddString(attribute_names[i], buffer);
							delete [] buffer;
							buffer = NULL;
						}
					}
					FindWindow("Article Maker")->PostMessage(parcel);
				}
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////
//
//  MessageReceived
///////////////////

void NewsApp::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
		case HERE_YA_GO__SAVE_THIS:
		{
			msg->PrintToStream();
			Save(msg);
		}
		default:
			BApplication::MessageReceived(msg);
			break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
//  SetCurrentDirectory
///////////////////

void NewsApp::SetCurrentDirectory() 
{
	app_info info; 
	BPath path; 
	be_app->GetAppInfo(&info); 
	BEntry entry(&info.ref); 
	entry.GetPath(&path); 
	path.GetParent(&path); 
	chdir(path.Path());
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
//  FindWindow
///////////////////

BWindow * NewsApp::FindWindow(const char *title)
{
	BWindow *win = NULL;
	int32 i = 0;
	while(win = be_app->WindowAt(i++))
	{
		if(win->Lock())
		{
			if((win->Title() != NULL) && !strcmp(win->Title(),title)/* strcmp is screwy */)
			{
				win->Unlock();
				return win;
			}
			win->Unlock();
		}
	}
	return win;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
//  Save
///////////////////

status_t NewsApp::Save(BMessage *msg)
{
	entry_ref directory;
	char *name, *ptr; // the name of the file
	BPath path;
	status_t err = B_NO_ERROR;

	msg->FindRef("directory", &directory);
	msg->FindString("name", &name);
	
	BEntry entry(&directory);
	entry.GetPath(&path);
	path.Append(name);
	
	BFile file(path.Path(), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
	
	if((err = file.InitCheck()) < B_NO_ERROR)
		return err;
	
	if(msg->HasString("Article"))
	{
		msg->FindString("Article", &ptr);
		if((err = file.WriteAt(0, (void *)ptr, strlen(ptr)+1)) < B_NO_ERROR)
			return err;
		//printf("%s\nptr = %s\n", strerror(err), ptr);
		ptr=NULL;
	}
	
	if((err = file.WriteAttr(attribute_names[10], B_MIME_TYPE, 0, (void *)"text/trackerbase", 17)) < B_NO_ERROR)
		return err;
	
	for(int i=0;i<=9;i++)
	{
		if(msg->HasString(attribute_names[i]))
		{
			msg->FindString(attribute_names[i], &ptr);
			if((err = file.WriteAttr(attribute_names[i], B_STRING_TYPE, 0, (void *)ptr, strlen(ptr)+1)) < B_NO_ERROR)
				return err;
			//printf("%s\nptr = %s\n", strerror(err), ptr);
			ptr=NULL;
		}
	}
	
	return B_NO_ERROR;	
}
/*

	Writing the files:
	
	file->WriteAt(...); // text
	file->WriteAttr("*", B_STRING_TYPE, 0, (void *)buf, strlen(buf)+1); // various attributes
	
	// either
	file->WriteAttr("BEOS:TYPE", B_MIME_TYPE, 0, (void *)"text/trackerbase", 17);	
	// OR
	BNodeInfo	nodeinfo(file);
	nodeinfo->SetType("text/trackerbase");
	
*/