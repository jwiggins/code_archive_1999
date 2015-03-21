#include "ProkTarget.h"


// -----------------------------------------------------------------------------
//	instantiate_dispatcher
// -----------------------------------------------------------------------------
BLooper *
instantiate_dispatcher( BLooper *connection, CyEnvironment *environment )
{
	return new ProkDispatcher( connection, environment );
}

// ProkDispatcher member functions

ProkDispatcher::ProkDispatcher( BLooper *connection, CyEnvironment *environment )
	: CyDispatcher(connection,environment)
{
}

CyTarget *ProkDispatcher::MakeNewTarget(  const istring &name, target_type type, uint32 level)
{
	return new ProkTarget(name,this,type,level);
}


// ProkTarget member functions

ProkTarget::ProkTarget(const istring &name, CyDispatcher *dispatcher, target_type type, uint32 level)
	: CyTarget(name,dispatcher,type,level)
{
	BRect						aRect;
	menu_info					m_info;
	int32						interfaceUnit;
	
	// the price we pay to be font sensitive...
	get_menu_info(&m_info);
	interfaceUnit = (int32)m_info.font_size;
	aRect.Set(100, 100, 649, 449 + interfaceUnit+8);
	
	_win = new CycloneChannelWindow(dispatcher,name.c_str(),this,type,aRect, (interfaceUnit+8));
	_win->Lock();
	_win->AddHandler(this);
	_win->Unlock();
	_win->Show();
}

ProkTarget::~ProkTarget()
{
	if (Looper())
	{
		Looper()->Quit();
	}
}

void ProkTarget::SetTopic(const istring &topic)
{
	_win->SetTopic(topic);
}

void ProkTarget::AddUser(const istring &nick, bool op, bool v)
{
	CyTarget::AddUser(nick,op,v); // important!
	
	bool					added = false;
	UserListItem*			item;
	BListView* 				list = static_cast<BListView*>(_win->FindView("_UserList"));
	userlist_t::iterator	i = mUserList.find(nick);
	
	
	if(i == mUserList.end()) {
		item = new UserListItem(nick.c_str(), op, v);
		mUserList[nick] = item;
		
		for(int32 j = 0; j < list->CountItems(); j++) {
			if(strcasecmp(item->Nick(), ((UserListItem*)(list->ItemAt(j)))->Nick()) < 1) {
				list->AddItem(item, j);
				added = true;
				return;
			}
		}
		if(!added) {
			list->AddItem(item);
		}
	}
	
	// the view will know the right thing to do
	MultiView	*view = (MultiView *)_win->FindView("_RootView");
	if(_win->Lock())
	{
		view->DrawUsers();
		_win->Unlock();
	}
}

void ProkTarget::RemoveUser(const istring &nick)
{
	CyTarget::RemoveUser(nick);
	
	UserListItem*			item;
	BListView* 				list = static_cast<BListView*>(_win->FindView("_UserList"));
	userlist_t::iterator	i = mUserList.find(nick);
	char					numUserString[6]; // find me a channel with > 99,999 users and i will change this

	if(i != mUserList.end()) {
		item = (*i).second;
		mUserList.erase(i);
		list->RemoveItem(item);
		delete item;
	}
	
	// the view will know the right thing to do
	MultiView	*view = (MultiView *)_win->FindView("_RootView");
	if(_win->Lock())
	{
		view->DrawUsers();
		_win->Unlock();
	}
}

void ProkTarget::SetUserOp( const istring &nick, bool o)
{
	// a lot glommed from mattl code
	UserListItem*			item;
	BListView* 				list = static_cast<BListView*>(_win->FindView("_UserList"));
	userlist_t::iterator	i = mUserList.find(nick);
	
	if(i != mUserList.end())
	{
		item = (*i).second;
		item->Op(o);
		list->InvalidateItem(list->IndexOf(item)); // redraw
	}
}

void ProkTarget::SetUserVoice( const istring &nick, bool v)
{
	// a lot glommed from mattl code
	UserListItem*			item;
	BListView* 				list = static_cast<BListView*>(_win->FindView("_UserList"));
	userlist_t::iterator	i = mUserList.find(nick);
	
	if(i != mUserList.end())
	{
		item = (*i).second;
		item->Voice(v);
		list->InvalidateItem(list->IndexOf(item)); // redraw
	}
}

void ProkTarget::PutText( const istring &text, const text_run_array *text_runs )
{
	_win->PrintText(text);
}

void ProkTarget::DoPrivMsg(const istring &nick, const istring &target,
							const istring &text,
							const text_run_array *text_runs)
{
	_win->PrintMessage(nick,text);
}

void
ProkTarget::DoAction(const istring &nick, const istring &target,
					const istring &text, const text_run_array *text_runs )
{
	_win->PrintAction(" " + nick,text);
}

void ProkTarget::DoNotice(const istring &nick, const istring &target,
							const istring &text,
							const text_run_array *text_runs)
{
	_win->PrintMessage(nick,text);
}

void ProkTarget::DoMode(const istring &nick, const istring &target,
							const istring &modes, const istring &params )
{
	// *** Mode change "+s" on channel #phonix by root
	//_win->PrintAction("** " + nick,istring("changed the mode ") + modes + " " + params);
	_win->PrintText("*** Mode change \"" + modes + " " + params + "\" by " + nick + "\n");
}


void ProkTarget::DoJoin(const istring &nick, const istring &userhost,
							const istring &channel )
{
	_win->PrintAction("** " + nick,istring("has joined the channel"));
}

void ProkTarget::DoPart(const istring &nick, const istring &channel )
{
	_win->PrintAction("** " + nick,istring("has left the channel"));
}

void ProkTarget::DoKick(const istring &nick, const istring &channel,
							const istring &kicked, const istring &reason,
							const text_run_array *reason_runs)
{
	_win->PrintAction("** " + kicked,istring("has been kicked by ") + nick + " (" + reason + ").");
}

void ProkTarget::DoQuit(const istring &nick, const istring &reason,
							const text_run_array *reason_runs)
{
	//_win->PrintAction("** " + nick,istring("has quit the network ")  + " (" + reason + ").");
	_win->PrintText("*** Signoff: " + nick + " (" + reason + ").\n");
}

void ProkTarget::DoTopic(const istring &nick, const istring &channel,
							const istring &topic,
							const text_run_array *topic_runs)
{
	_win->PrintAction("** " + nick,istring("has changed the topic to: ") + topic);
}


void ProkTarget::DoNick(const istring &nick, const istring &oldnick )
{
	_win->PrintAction("** " + oldnick,istring("is now known as ") + nick);
	if(_dispatcher->environment.Get(NICK) == nick)
	{
		_win->ChangeNick(nick.c_str());
	}
}


void ProkTarget::DoError(const istring &error )
{
	_win->PrintAction("** " + error,istring(""));
}
