#include "ControlWindow.h"

ControlWindow::ControlWindow()
: BWindow(BRect(100,100, 349, 194), "NetInput", B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_NOT_ZOOMABLE|B_NOT_RESIZABLE)
{
	BRect rect, bounds = Bounds();
	BView *background = new BView(bounds, "background", B_FOLLOW_ALL, 0);
	BTextControl *host_field, *port_field;
	BButton *connect_but, *disconnect_but;
	BCheckBox *lock_input;
	
	background->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(background);
	
	rect.Set(5.,5., bounds.Width() - 5., 20.);
	host_field = new BTextControl(rect, "host", "Host", NULL, new BMessage(TEXT_CONTROL));
	host_field->SetDivider(background->StringWidth("####"));
	background->AddChild(host_field);
	
	rect.Set(5.,30., bounds.Width() - 5., 45.);
	port_field = new BTextControl(rect, "port", "Port", NULL, new BMessage(TEXT_CONTROL));
	port_field->SetDivider(background->StringWidth("####"));
	background->AddChild(port_field);
	
	rect.Set(bounds.Width()*0.75, 60., bounds.Width() - 10., 80.);
	connect_but = new BButton(rect, "start", "Start", new BMessage(START_BUTTON));
	connect_but->MakeDefault(true);
	background->AddChild(connect_but);
	
	rect.Set(bounds.Width()*0.5, 60., (bounds.Width()*0.75) - 10., 80.);
	disconnect_but = new BButton(rect, "stop", "Stop", new BMessage(STOP_BUTTON));
	background->AddChild(disconnect_but);
	
	rect.Set(bounds.Width()*0.25, 60., (bounds.Width()*0.5) - 10., 80.);
	lock_input = new BCheckBox(rect,"locker", "Lock Input", new BMessage(LOCK_INPUT));
	background->AddChild(lock_input);
	
	// find the command port of our input server filter
	commandport = find_port("NetInputFilterPort:cmd");
}

bool ControlWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

void ControlWindow::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
		case START_BUTTON:
		{
			BMessage command_msg;
			BTextControl *textcontrol;
			int32 host_addr = INADDR_LOOPBACK; // default 127.0.0.1
			short host_port = 666; // default port 666
			const char *chr_ptr = NULL;
			hostent *the_host;
			
			// grab the host address
			textcontrol = (dynamic_cast<BTextControl *>(FindView("host")));
			if(textcontrol != NULL)
			{
				chr_ptr = textcontrol->Text();
				//printf("%s\n", chr_ptr);
				if(chr_ptr != NULL)
				{
					the_host = gethostbyname(chr_ptr);
					if(the_host == NULL)
					{
						//printf("gethostbyname() error. h_errno = %s\n", strerror(h_errno));
						uint32 addr = inet_addr(chr_ptr);
						the_host = gethostbyaddr((const char *)&addr, 4, AF_INET);
						
						if(the_host == NULL)
						{
							// feh
							//printf("gethostbyaddr() error. h_errno = %s\n", strerror(h_errno));
						}
						else
						{
							host_addr = *(uint32 *)the_host->h_addr;
						}
					}
					else
					{
						host_addr = *(uint32 *)the_host->h_addr;
					}
				}
			}
			
			chr_ptr = NULL; // set back to NULL
			
			// grab the host port
			textcontrol = (dynamic_cast<BTextControl *>(FindView("port")));
			if(textcontrol != NULL)
			{
				chr_ptr = textcontrol->Text();
				if(chr_ptr != NULL)
				{
					host_port = atoi(chr_ptr); // convert from ascii					
				}
			}
			
			printf("host_addr = %x\n", host_addr);
			printf("host_port = %x\n", host_port);
			
			// send the message of to the NetInputFilter
			command_msg.what = FILTER_START;
			command_msg.AddInt32("host_addr", host_addr);
			command_msg.AddInt16("host_port", host_port);
			// pull some crazy magic to send it
			call_device(commandport, &command_msg);
			
			break;
		}
		case STOP_BUTTON:
		{
			BMessage command_msg(FILTER_STOP);
			
			// pull some crazy magic to send it
			call_device(commandport, &command_msg);
			break;
		}
		case LOCK_INPUT:
		{
			BMessage command_msg(FILTER_LOCKOUT);
			BCheckBox *checkbox = (dynamic_cast<BCheckBox *>(FindView("locker")));
			
			if(checkbox != NULL)
			{
				command_msg.AddBool("IsLocked", (checkbox->Value()?true:false));
				// pull some crazy magic to send it
				call_device(commandport, &command_msg);
			}
			
			break;
		}
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}

status_t call_device(port_id devicePort, BMessage *event)
{
	//event->PrintToStream();

	status_t err = B_OK;
	if(devicePort < 0) return(devicePort);
	size_t size = event->FlattenedSize();
	char *buffer = (char*)malloc(size);
	if(buffer!=NULL){
		event->Flatten(buffer, size);
		if((err = write_port(devicePort, 0, buffer, size))!=B_OK){
			printf("Error finding writing to device port: (0x%x) %s\n", err, strerror(err));
		}
		free(buffer);
	}
	else
	{
		if(errno < 0) err = errno;
		if(!err) err = B_ERROR;
		printf("Failed to allocate space for device buffer: (0x%x) %s\n", err, strerror(err));
	}
	return(err);
}