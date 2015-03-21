#ifndef ATTR_ADD_ON_H
#define ATTR_ADD_ON_H

#include <Archivable.h>
#include <View.h>
#include <Rect.h>
#include <SupportDefs.h>

// Instantiate Message contains:
//	- "typecode" - type_code of data to be edited.
//	- "data" - void * to data you should edit
//	- "datasize" - int32 telling you how big the data is (in bytes)

/* example export. add yer own type_codes
extern "C" 
{
	_EXPORT BArchivable* Instantiate(BMessage *);
	_EXPORT type_code TypeCode[] = { (type_code)NULL };
}
*/

// Should be a virtual base class, but I'm not sure I can do that
// since I inherit from BView
// BView inherits from BHandler. BHandeler inherits from BArchivable.
class AttrAddon : public BView {
	public:
					AttrAddon(BRect frame, const char *name, uint32 resizeMask, uint32 flags, BMessage *msg)
					:BView(frame,name,resizeMask,flags) {};
virtual				~AttrAddon() {}; // cleanup
virtual	status_t	GetData(BMessage *msg){return B_ERROR;};
virtual	status_t	ChangeData(BMessage *msg){return B_ERROR;};
virtual	bool		IsDirty() const {return false;};

};

#endif
/*
extern "C" 
{
	extern _EXPORT void Instantiate(BView *view, type_code type, void *data, int32 len);
	extern _EXPORT void ChangeData(BView *view, type_code type, void *data, int32 len);
	extern _EXPORT void GetData(BView *view, type_code type, void *buffer, int32 *len);
	extern _EXPORT void Cleanup();
	_EXPORT type_code TypeCode[] = { B_MIME_STRING_TYPE, B_STRING_TYPE, (type_code)NULL };
}
*/