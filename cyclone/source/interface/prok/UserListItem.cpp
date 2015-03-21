#include "UserListItem.h"
#include "CyConstants.h"

UserListItem::UserListItem(const char *nick, bool op, bool voice, uint32 level, bool expanded)
			: BListItem(level, expanded) // construct with default args
{
	_nick = new char [ strlen(nick) + 1 ];
	strcpy(_nick, nick);
	_op = op;
	_voice = voice;
}

UserListItem::~UserListItem()
{
	delete [] _nick;
}

void UserListItem::DrawItem(BView *owner, BRect itemRect, bool complete)
{
	if (IsSelected() || complete)
	{ 
		if( IsSelected() ) // shaded
			owner->SetHighColor(kBeListSelectGreyColor);
		else // unshaded
			owner->SetHighColor(owner->ViewColor());
		// now fill with delight :)
		owner->FillRect(itemRect);
		owner->SetHighColor(kBlackColor); // set it back
	}
	// now draw the text
	owner->MovePenTo(itemRect.left + 2, itemRect.bottom - 2);
	if(_op) // op
	{
		owner->SetHighColor(kRedColor); // op color
		owner->DrawString("@");
	}
	else
		owner->DrawString(" ");
	if(_voice) // voiced
	{
		owner->SetHighColor(kGreenColor); // voice color
		owner->DrawString("+");
	}
	else
		owner->DrawString(" ");
	owner->SetHighColor(kBlackColor); // set it back
	owner->DrawString("  ");
	owner->DrawString(_nick);
}

void UserListItem::Op(bool o)
{
	_op = o;
}

void UserListItem::Voice(bool v)
{
	_voice = v;
}

const char *UserListItem::Nick() const
{ return _nick; }