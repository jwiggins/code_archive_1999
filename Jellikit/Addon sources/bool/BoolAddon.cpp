#include "BoolAddon.h"

AttrAddon * Instantiate(BRect frame, const char *name, uint32 resizeMask, uint32 flags, BMessage *msg, status_t *ret)
{
	return new BoolAddon(frame,name,resizeMask,flags,msg,ret);
}

BoolAddon::BoolAddon(BRect frame, const char *name, uint32 resizeMask, uint32 flags, BMessage *msg, status_t *ret)
			: AttrAddon(frame,name,resizeMask,flags,msg,ret)
{
	//printf("BoolAddon: Instantiate.\n");
	BFont font(be_plain_font);
	int32 interfaceUnit = ((int32)font.Size()), width = frame.Width();
	
	// get "typecode", "data", & "datasize" out of the message and use
	// the data to fill in some stuff
	BRect rect;
	int32 datasize, typecode;
	ssize_t size_from_message;
	void *data; // for some reason, FindData hates for this to be NULL
	bool value;
	status_t err;
	if(msg->FindInt32("datasize", &datasize) == B_NO_ERROR)
	{
		//printf("BoolAddon: grabbed datasize\n");
		if(msg->FindInt32("typecode", &typecode) == B_NO_ERROR)
		{
			current_type = typecode; // kludge!!! 
			//printf("BoolAddon: grabbed typecode. typecode = %u\n", typecode);
			if((err = msg->FindPointer("data", &data)) == B_NO_ERROR)
			{
				//printf("BoolAddon: grabbed data\n");
				//printf("BoolAddon: all data harvested from message.\n");
				// everything's just dandy!
				memcpy((void *)&value, data, 1); // copy the byte
			}
			else
			{
				//printf("error = %s\n", strerror(err));
				*ret = B_ERROR;
				return;
			}
		}
	}
	//msg->PrintToStream();
	
	switch(current_type)
	{
		case B_BOOL_TYPE:
		{
			rect.Set(interfaceUnit, interfaceUnit, width - interfaceUnit, interfaceUnit<<1);
			BCheckBox *checkbox = new BCheckBox(rect, "Bool", "Bool", NULL);
			((BControl*)checkbox)->SetValue((int32)value); // in case of bool, data is 0 or 1
			
			initial_value = value;
			//printf("BoolAddon : AddChild(checkbox);\n");
			AddChild(((BView*)checkbox));
			break;
		}
		default:
		{
			// this shouldn't happen. but then hell _could_ freeze over.
			rect.Set(interfaceUnit, interfaceUnit, width - interfaceUnit, interfaceUnit<<1);
			BStringView *stringview = new BStringView(rect, "Bad Addon Instantiation", "Bad Addon Instantiation");
			AddChild(stringview);
			break;
		}
	}
	
#ifndef B_BEOS_VERSION_4
	SetViewColor(216,216,216,255);
#else
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
#endif
	//printf("BoolAddon : done!\n");
	*ret = B_NO_ERROR;
}

BoolAddon::~BoolAddon()
{
	// do nothing
}

status_t BoolAddon::GetData(BMessage *msg)
{
	// add feild "data" to message
	status_t err;
	switch(current_type)
	{
		case B_BOOL_TYPE:
		{
			BCheckBox *checkbox = (BCheckBox *)FindView("Bool");
	
			if(checkbox != NULL)
			{
				bool value = (bool)((BControl*)checkbox)->Value();
				err = msg->AddData("data", current_type, (void *)&value, sizeof(bool));
			}
			else
			{
				err = B_ERROR;
			}
			break;
		}
		default:
		{
			err = B_ERROR;
			break;
		}
	}
	return err;
}

status_t BoolAddon::ChangeData(BMessage *msg)
{
	// msg contains feild "data"
	status_t err;
	switch(current_type)
	{
		case B_BOOL_TYPE: // almost not worth checking
		{
			BCheckBox *checkbox = (BCheckBox *)FindView("Bool");
			if(checkbox != NULL)
			{
				char *data;
				ssize_t size;
				if((err = msg->FindData("data", current_type, (void**)&data, &size)) == B_NO_ERROR)
				{
					((BControl*)checkbox)->SetValue(((int32)*data));
					initial_value = (bool)*data;
				}
				//err = B_NO_ERROR;
			}
			else
			{
				err = B_ERROR;
			}
			break;
		}
		default:
		{
			err = B_ERROR;
			break;
		}
	}
	return err;
}
bool BoolAddon::IsDirty() const
{
	BCheckBox *checkbox = (BCheckBox *)FindView("Bool");
	if(checkbox != NULL)
	{
		if(initial_value == ((BControl*)checkbox)->Value())
			return false;
		else
			return true;
	}
	return false; // better to lie than corrupt :)
}