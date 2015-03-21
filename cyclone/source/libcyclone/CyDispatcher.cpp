// =============================================================================
//	CyDispatcher													05/11/98
//
//	History:
//			05/11/98 JDG    Creation; inherited rough code from MGB.
// =============================================================================


#include "CyDispatcher.h"
#include "CyEnvironment.h"
#include "istring.h"

// -----------------------------------------------------------------------------
//	CyDispatcher ctor
// -----------------------------------------------------------------------------
CyDispatcher::CyDispatcher( BLooper *connection, CyEnvironment *environment )
	: _connection( connection ), environment( *environment ), _console( 0 ),
	  BLooper( "Dispatcher" )
{
}


// -----------------------------------------------------------------------------
//	CyDispatcher dtor
// -----------------------------------------------------------------------------
CyDispatcher::~CyDispatcher()
{
}


// -----------------------------------------------------------------------------
//	Run
// -----------------------------------------------------------------------------
thread_id
CyDispatcher::Run()
{
	MakeConsole();	
	return BLooper::Run();
}


// -----------------------------------------------------------------------------
//	QuitRequested
// -----------------------------------------------------------------------------
bool
CyDispatcher::QuitRequested()
{
	PostToAllTargets( &BMessage(B_QUIT_REQUESTED) );
	return true;
}


// -----------------------------------------------------------------------------
//	MessageReceived
// -----------------------------------------------------------------------------
void
CyDispatcher::MessageReceived( BMessage *msg )
{
	switch ( msg->what )
	{
		case CY_SERVER_MESSAGE:
			DispatchServerMessage( msg );
			break;
		
		case CY_INTERFACE_MESSAGE:
			DispatchInterfaceMessage( msg );
			break;
		
		case CY_CLIENT_MESSAGE:
		case CY_CLIENT_COMMAND:
			_connection->PostMessage( msg );
			break;
				
		
		default:
			BLooper::MessageReceived( msg );
			break;
	}	
}


// -----------------------------------------------------------------------------
//	DispatchInterfaceMessage
// -----------------------------------------------------------------------------
void
CyDispatcher::DispatchInterfaceMessage( BMessage *msg )
{
	CyTarget *	target = _console;
	targetmap_t::const_iterator	i;
	
	//	Is there an alternate preferred handler? -------------------------------
	for ( i = _targets.begin(); i != _targets.end(); ++i )
	{
		ASSERT( (*i).second != 0 );
		if ( (*i).second->Level() & LEVEL_GENERAL )
		{
			target = (*i).second;
			break;
		}
	}	
	
	ASSERT( target->Looper() != 0 );
	target->Looper()->PostMessage( msg, target );
}



// -----------------------------------------------------------------------------
//	DispatchServerMessage
// -----------------------------------------------------------------------------
void
CyDispatcher::DispatchServerMessage( BMessage *msg )
{
	char *chptr;
	int16 num;
	istring target;
	uint32 level = LEVEL_PUBLIC; // default assumption
	target_type type = PUBLIC;
	istring command;
	
	if ( msg->FindString(kCommandTag, &chptr) == B_OK )
	{
		command = chptr;
		
		//	all handlers need to be informed of this command:
		if ( (command == "QUIT") || (command == "NICK") )
		{
			PostToAllTargets( msg );
			return;
		}
		
		// NOTICE messages should not be targeted:
		else if (command == "NOTICE")
		{
			if ( !PostToLevel(msg,LEVEL_NOTICE) )
				PostToConsole(msg);
			return;
		}
		
		if ( msg->FindString(kParamTag, 0, &chptr) == B_OK )
			target = chptr;
		
		// private messages:
		if ( target == environment.Get(NICK) )
		{
			if ( msg->FindString(kNickTag, &chptr ) == B_OK )
			{
				istring	nick(chptr);
				target = nick;
				level = LEVEL_PRIVATE;
				type = PRIVATE;
			}
			else level = LEVEL_GENERAL; // server messages
		}
	}
	
	else if ( msg->FindInt16(kNumericTag,&num) == B_OK )
	{
		switch(num)
		{
			case RPL_CHANNELMODEIS:
			case RPL_NOTOPIC:
			case RPL_TOPIC:
			case RPL_INVITING:
			case RPL_BANLIST:
			case RPL_ENDOFBANLIST:
			case ERR_CANNOTSENDTOCHAN:
			case ERR_KEYSET:
			case ERR_CHANOPRIVSNEEDED:
			case RPL_ENDOFNAMES:
			case 333: // topic set by ___ at ____
				level = LEVEL_PUBLIC;
				if ( msg->FindString(kParamTag, 1, &chptr) == B_OK )
					target = chptr;
				break;
			
			case ERR_USERONCHANNEL:
			case RPL_NAMREPLY:
				level = LEVEL_PUBLIC;
				if ( msg->FindString(kParamTag, 2, &chptr) == B_OK )
					target = chptr;
				break;
			
		}
	}
	
	if ( !PostToTarget(msg,target) )
		if ( !PostToLevel(msg,level) )
		{
			// create a new target?
			if ( (command == "JOIN") || (type == PRIVATE) )
			{
				CyTarget *handler = MakeNewTarget( target,type );
				SetTargetHandler( target,handler );
				handler->Looper()->PostMessage( msg,handler );
			}
			else PostToConsole(msg);
		}
}

// -----------------------------------------------------------------------------
//	PostToTarget -- by name of target (nick or channel name)
// -----------------------------------------------------------------------------
bool
CyDispatcher::PostToTarget( BMessage *msg, istring &target )
{
	targetmap_t::const_iterator	i;
	
	//	A target handler is available ------------------------------
	if ( (i = _targets.find(target)) != _targets.end() )
	{
		ASSERT( (*i).second != 0 );
		ASSERT( (*i).second->Looper() != 0 );
		(*i).second->Looper()->PostMessage( msg, (*i).second );
		return true;
	}
	else return false;
}


// -----------------------------------------------------------------------------
//	PostToLevel -- by level (public, private, NOTICE, general)
// -----------------------------------------------------------------------------
bool
CyDispatcher::PostToLevel( BMessage *msg, uint32 level )
{
	targetmap_t::const_iterator	i;
	
	//	A level handler is available ---------------------------------------
	if ( (i = find_level(level)) != _targets.end() )
	{
		ASSERT( (*i).second != 0 );
		ASSERT( (*i).second->Looper() != 0 );
		(*i).second->Looper()->PostMessage( msg, (*i).second );
		return true;
	}
	else return false;
}


// -----------------------------------------------------------------------------
//	PostToConsole
// -----------------------------------------------------------------------------
void
CyDispatcher::PostToConsole( BMessage *msg )
{
	if (_console)
	{
		BLooper *looper = _console->Looper();
		if ( looper )
			looper->PostMessage( msg, _console );
	}
}


// -----------------------------------------------------------------------------
//	PostToAllTargets
// -----------------------------------------------------------------------------
void
CyDispatcher::PostToAllTargets( BMessage *msg )
{
	BLooper *looper;
	
	PostToConsole(msg);
		
	targetmap_t::const_iterator i = _targets.begin();
	for ( ; i != _targets.end(); ++i )
		if ( looper = ((*i).second)->Looper() )
			looper->PostMessage( msg, (*i).second );
}


// -----------------------------------------------------------------------------
//	SetTargetHandler -- adds a CyTarget * to the target map by name
// -----------------------------------------------------------------------------
void
CyDispatcher::SetTargetHandler( const istring &target, CyTarget *handler )
{
	Lock();
	_targets[ target ] = handler;
	DebugText( istring("Target handler set: ") + target );
	Unlock();
}


// -----------------------------------------------------------------------------
//	RemoveTarget
// -----------------------------------------------------------------------------
void
CyDispatcher::RemoveTarget( const istring &target )
{
	Lock();
	targetmap_t::iterator i = _targets.find( target );
	
	if ( i != _targets.end() )
	{
		_targets.erase( i );
		DebugText( istring("Target handler removed: ") + target );
	}
	else
	{
		DebugText( istring("Target handler unknown: ") + target );
	}
	
	Unlock();
}


// -----------------------------------------------------------------------------
//	MakeConsole
// -----------------------------------------------------------------------------
void CyDispatcher::MakeConsole()
{
	if (!_console)
		_console = MakeNewTarget("*console*", CONSOLE );
}


// -----------------------------------------------------------------------------
//	RemoveConsole
// -----------------------------------------------------------------------------
void CyDispatcher::RemoveConsole()
{
	_console = 0;
}


// -----------------------------------------------------------------------------
//	find_level -- returns an iterator for the first target matching the level
// -----------------------------------------------------------------------------
CyDispatcher::targetmap_t::const_iterator
CyDispatcher::find_level( uint32 level )
{
	targetmap_t::const_iterator i;
	for ( i = _targets.begin(); i != _targets.end(); ++i )
	{
		ASSERT( (*i).second != 0 );
		if ( (*i).second->Level() & level )
			return i;
	}

	return _targets.end();
}


// -----------------------------------------------------------------------------
//	DebugText -- goes to James Bond, our debug window
// -----------------------------------------------------------------------------
void
CyDispatcher::DebugText( const istring &text )
{
	BMessage msg( CY_DEBUG_MESSAGE );
	msg.AddString( kTextTag, text.c_str() );
	_connection->PostMessage( &msg, _connection );
}
