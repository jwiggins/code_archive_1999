/*
	
	UserListItem.h
	
	John Wiggins. Cyclone GUI. 1998
	
*/

#ifndef USER_LIST_ITEM_H
#define USER_LIST_ITEM_H

#ifndef _LIST_ITEM_H
#include <ListItem.h>
#endif

class UserListItem : public BListItem {

public:
				UserListItem(const char *nick, bool op, bool voice, uint32 level = 0, bool expanded = true);
				~UserListItem();
virtual	void	DrawItem(BView *owner, BRect itemRect, bool complete);
void			Op(bool o);
void			Voice(bool v);
const char		*Nick() const;

private:
char			*_nick;
bool			_op;
bool			_voice;

};

#endif
