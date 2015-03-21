#include "MgbTarget.h"

const BRect kWindowSiz(100,100,500,500);

// -----------------------------------------------------------------------------
//	instantiate_dispatcher
// -----------------------------------------------------------------------------
BLooper *
instantiate_dispatcher( BLooper *connection, CyEnvironment *environment )
{
	return new MgbDispatcher( connection, environment );
}

// MgbDispatcher member functions

MgbDispatcher::MgbDispatcher( BLooper *connection, CyEnvironment *environment )
	: CyDispatcher(connection,environment)
{
}

CyTarget *MgbDispatcher::MakeNewTarget( const istring &name, target_type type, uint32 level )
{
	return new MgbTarget(name,this,type,level);
}


// MgbTarget member functions

MgbTarget::MgbTarget(const istring &name, CyDispatcher *dispatcher,
						target_type type, uint32 level)
	: CyTarget(name,dispatcher,type,level)
{
	switch (type)
	{
		case PUBLIC:
		case PRIVATE:
		case CONSOLE:
		{
			_win = new CyChannelWindow(dispatcher,name.c_str(),this,type,kWindowSiz);
			_win->Lock();
			_win->AddHandler(this);
			_win->Unlock();
			_win->Show();
			break;
		}
		default:
			break;
	}
}

MgbTarget::~MgbTarget()
{
	if (Looper())
	{
		Looper()->Quit();
	}
}

void MgbTarget::PutText( const istring &text, const text_run_array *text_runs )
{
	_win->PrintText(text);
}

void MgbTarget::DoPrivMsg(const istring &nick, const istring &target,
							const istring &text,
							const text_run_array *text_runs)
{
	_win->PrintMessage(nick,text);
}

void
MgbTarget::DoAction(const istring &nick, const istring &target,
					const istring &text, const text_run_array *text_runs )
{
	_win->PrintAction(nick,text);
}

void MgbTarget::DoNotice(const istring &nick, const istring &target,
							const istring &text,
							const text_run_array *text_runs)
{
	_win->PrintMessage(nick,text);
}

void MgbTarget::DoMode(const istring &nick, const istring &target,
							const istring &modes, const istring &params )
{
	_win->PrintAction(nick,istring("changed the mode ") + modes + " " + params);
}


void MgbTarget::DoJoin(const istring &nick, const istring &userhost,
							const istring &channel )
{
	_win->PrintAction(nick,istring("joined the channel"));
}

void MgbTarget::DoPart(const istring &nick, const istring &channel )
{
	_win->PrintAction(nick,istring("left the channel"));
}

void MgbTarget::DoKick(const istring &nick, const istring &channel,
							const istring &kicked, const istring &reason,
							const text_run_array *reason_runs)
{
	_win->PrintAction(nick,istring("kicked ") + kicked + " (" + reason + ").");
}

void MgbTarget::DoQuit(const istring &nick, const istring &reason,
							const text_run_array *reason_runs)
{
	_win->PrintAction(nick,istring("quit the network ")  + " (" + reason + ").");
}

void MgbTarget::DoTopic(const istring &nick, const istring &channel,
							const istring &topic,
							const text_run_array *topic_runs)
{
	_win->PrintAction(nick,istring("set the topic to: ") + topic);
}


void MgbTarget::DoNick(const istring &nick, const istring &oldnick )
{
	_win->PrintAction(oldnick,istring("is now called ") + nick);
}


void MgbTarget::DoError(const istring &error )
{
	_win->PrintAction(error,istring(""));
}
