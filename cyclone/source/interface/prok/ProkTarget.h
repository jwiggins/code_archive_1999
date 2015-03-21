#ifndef PROK_TARGET_H

#include "CyTarget.h"
#include "CyDispatcher.h"
#include "ChannelWindow.h"
#include <map.h>
#include "istring.h"
#include "UserListItem.h"
#include "MultiView.h"

#pragma export on
extern "C" BLooper *instantiate_dispatcher( BLooper *, CyEnvironment * );
#pragma export reset

class ProkDispatcher : public CyDispatcher {
public:
			ProkDispatcher( BLooper *connection, CyEnvironment *environment );
			CyTarget *MakeNewTarget( const istring &name, target_type type, uint32 level = LEVEL_NONE );
};

typedef map< istring, UserListItem*, less<istring>, allocator<UserListItem*> > userlist_t;

class ProkTarget : public CyTarget {
private:
				CycloneChannelWindow	*_win;
				userlist_t				mUserList;
				
public:
				ProkTarget(const istring &name, CyDispatcher *dispatcher, target_type type, uint32 level);
virtual 		~ProkTarget();
//  ---------------------------------------------------------
virtual	void	SetName(const istring &name) { _win->SetTitle(name.c_str());
												_win->SetName(name.c_str()); }
//  ---------------------------------------------------------
virtual void	SetTopic( const istring &topic );

//	---------------------------------------------------------
virtual void	AddUser( const istring &nick, const bool op = false, const bool v = false );
virtual void	RemoveUser( const istring &nick );

//	---------------------------------------------------------
virtual	void	SetUserOp( const istring &nick, bool o);
virtual	void	SetUserVoice( const istring &nick, bool v);

//	---------------------------------------------------------
void 			PutText( const istring &text, const text_run_array *text_runs );
	
virtual void	DoPrivMsg(	const istring &nick, const istring &target,
								const istring &text,
								const text_run_array *text_runs = 0 );

virtual void	DoNotice(	const istring &nick, const istring &target,
								const istring &text,
								const text_run_array *text_runs = 0 );
								
virtual void	DoMode(		const istring &nick, const istring &target,
								const istring &modes, const istring &params );
	
virtual void	DoJoin(		const istring &nick, const istring &userhost,
								const istring &channel );

virtual void	DoPart(		const istring &nick, const istring &channel );

virtual void	DoKick(		const istring &nick, const istring &channel,
								const istring &kicked, const istring &reason,
								const text_run_array *reason_runs = 0 );

virtual void	DoQuit(		const istring &nick, const istring &reason,
								const text_run_array *reason_runs = 0 );
	
virtual void	DoTopic(	const istring &nick, const istring &channel,
								const istring &topic,
								const text_run_array *topic_runs = 0 );

virtual void	DoNick(		const istring &nick, const istring &oldnick );

virtual void	DoError(	const istring &error );
	
virtual void	DoAction(	const istring &nick, const istring &target,
								const istring &text,
								const text_run_array *text_runs = 0 );
};

#define PROK_TARGET_H
#endif