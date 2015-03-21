// =============================================================================
//	CyTarget														05/10/98
//
//	History:
//			05/10/98 JDG    Creation; inherited rough code from MGB.
// =============================================================================

#pragma once

#include <set>
#include <Handler.h>
#include "istring.h"
#include "CyConstants.h"

#include "InterfaceDefs.h"

class CyDispatcher;

class CyTarget : public BHandler {

protected:
	typedef set< istring, less<istring>, allocator<istring> >	istringlist_t;
	
	istringlist_t	_users;
	target_type		_type;
	uint32			_level;
	istring			_name;
	CyDispatcher *	_dispatcher;

#if __POWERPC__ && CYCLONE_BUILDING_SHARED
#pragma export on
#endif
	
public:
					CyTarget(	const istring &name = "",
								CyDispatcher *dispatcher = 0,
								target_type type = PUBLIC,
								uint32 level = LEVEL_NONE );
	virtual			~CyTarget();
					
	virtual void 	SetName(const istring &name) {};

	void			DebugText(	const istring &text );

	virtual uint32	Level();
	target_type		Type();
	const istring &	Name();
	
	virtual void	MessageReceived( BMessage *msg );
	virtual void	HandleInterfaceMessage( BMessage *msg );
	virtual void	HandleServerCommand( BMessage *msg );
	virtual void	HandleServerNumeric( BMessage *msg );
	
	virtual void	SetTopic( const istring &topic ) {};
	
	virtual void	AddUser( const istring &nick, const bool op = false, const bool v = false );
	virtual void	RemoveUser( const istring &nick );
	bool			IsUserOnChannel( const istring &nick );
	void			AddUsers( const istring &nicks ); // internal use
	
	virtual void	SetUserOp( const istring &nick, bool o ) {};
	virtual void	SetUserVoice( const istring &nick, bool v ) {};
	
	virtual void	Exit();
	
	//	Virtual functions to be implemented by subclasses ----------------------
	
	virtual void	PutText(	const istring &text,
								const text_run_array *text_runs = 0 ) = 0;
								
	virtual void	DoAction(	const istring &nick, const istring &target,
								const istring &text,
								const text_run_array *text_runs = 0 ) = 0;

	virtual void	DoPrivMsg(	const istring &nick, const istring &target,
								const istring &text,
								const text_run_array *text_runs = 0 ) = 0;

	virtual void	DoNotice(	const istring &nick, const istring &target,
								const istring &text,
								const text_run_array *text_runs = 0 ) = 0;

	virtual void	DoMode(		const istring &nick, const istring &target,
								const istring &modes, const istring &params ) = 0;
	
	virtual void	DoJoin(		const istring &nick, const istring &userhost,
								const istring &channel ) = 0;

	virtual void	DoPart(		const istring &nick, const istring &channel ) = 0;
	
	virtual void	DoKick(		const istring &nick, const istring &channel,
								const istring &kicked, const istring &reason,
								const text_run_array *reason_runs = 0 ) = 0;

	virtual void	DoQuit(		const istring &nick, const istring &reason,
								const text_run_array *reason_runs = 0 ) = 0;
	
	virtual void	DoTopic(	const istring &nick, const istring &channel,
								const istring &topic,
								const text_run_array *topic_runs = 0 ) = 0;

	virtual void	DoNick(		const istring &newnick, const istring &oldnick ) = 0;

	virtual void	DoError(	const istring &error ) = 0;

#if __POWERPC__ && CYCLONE_BUILDING_SHARED
#pragma export reset
#endif

};
