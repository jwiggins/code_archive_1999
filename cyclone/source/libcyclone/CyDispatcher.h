// =============================================================================
//	CyDispatcher													05/11/98
//
//	History:
//			05/11/98 JDG    Creation; inherited rough code from MGB.
// =============================================================================


#ifndef CY_DISPATCHER_H

#include <ctype.h>
#include <map>
#include "istring.h"

#include "CyConstants.h"
#include "CyTarget.h"
#include "CyEnvironment.h"

class CyDispatcher : public BLooper {
public:
	CyEnvironment &	environment;

private:
	typedef map< istring, CyTarget *, less<istring>, allocator<CyTarget *> > targetmap_t;
	
	targetmap_t		_targets;
	BLooper *		_connection;
	CyTarget *		_console;

#if __POWERPC__ && CYCLONE_BUILDING_SHARED
#pragma export on
#endif

public:
					CyDispatcher( BLooper *connection,
								  CyEnvironment *environment );
								  
					~CyDispatcher();
	
	thread_id		Run();
	virtual bool	QuitRequested();
	void			MessageReceived( BMessage *msg );
	
	void			DispatchServerMessage( BMessage *msg );
	void			DispatchInterfaceMessage( BMessage *msg );

	virtual
	CyTarget *		MakeNewTarget( const istring &name, 
						target_type type, uint32 level = LEVEL_NONE ) = 0;
	
	void			SetTargetHandler( const istring &target, CyTarget *handler );
	void			RemoveTarget( const istring &target );
	
	virtual void	MakeConsole();
	virtual	void 	RemoveConsole();
	
	bool			PostToTarget( BMessage *msg, istring &target );
	bool			PostToLevel( BMessage *msg, uint32 level );
	void			PostToConsole( BMessage *msg );
	void			PostToAllTargets( BMessage *message );

	void			DebugText( const istring &text );

#if __POWERPC__ && CYCLONE_BUILDING_SHARED
#pragma export reset
#endif

private:
	targetmap_t::
	const_iterator	find_level( uint32 level );
};

#pragma export reset

#define CY_DISPATCHER_H
#endif

