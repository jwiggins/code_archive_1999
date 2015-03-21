// =============================================================================
//	CyTarget														05/10/98
//
//	History:
//			05/10/98 JDG    Creation; inherited rough code from MGB.
// =============================================================================

#include "CyTarget.h"
#include "CyConstants.h"
#include "CyDispatcher.h"


// -----------------------------------------------------------------------------
//	CyTarget ctor
// -----------------------------------------------------------------------------
CyTarget::CyTarget( const istring &name, CyDispatcher *dispatcher, 
	target_type type, uint32 level )
	: _name( name ), _dispatcher( dispatcher ), _level(level), _type(type)
{
	if (_type == PRIVATE)
	{
		AddUser(name);
		AddUser(_dispatcher->environment.Get(NICK));
	}
}


// -----------------------------------------------------------------------------
//	CyTarget dtor
// -----------------------------------------------------------------------------
CyTarget::~CyTarget()
{
}


// -----------------------------------------------------------------------------
//	DebugText -- shaken, not stirred
// -----------------------------------------------------------------------------
void
CyTarget::DebugText( const istring &text )
{
	_dispatcher->DebugText( text );
}



// -----------------------------------------------------------------------------
//	Level
// -----------------------------------------------------------------------------
uint32
CyTarget::Level()
{
	return _level;
}

// -----------------------------------------------------------------------------
//	Type
// -----------------------------------------------------------------------------
target_type
CyTarget::Type()
{
	return _type;
}


// -----------------------------------------------------------------------------
//	Name
// -----------------------------------------------------------------------------
const istring &
CyTarget::Name()
{
	return _name;
}


// -----------------------------------------------------------------------------
//	MessageReceived
// -----------------------------------------------------------------------------
void
CyTarget::MessageReceived( BMessage *msg )
{
	switch ( msg->what )
	{
		case CY_INTERFACE_MESSAGE:
			HandleInterfaceMessage( msg );
			break;

		case CY_SERVER_MESSAGE:
			if ( msg->HasInt16(kNumericTag) )
				HandleServerNumeric( msg );
			else if ( msg->HasString(kCommandTag) )
				HandleServerCommand( msg );
			break;
		
		default:
			BHandler::MessageReceived( msg );
			break;
	}
}



// -----------------------------------------------------------------------------
//	HandleInterfaceMessage
// -----------------------------------------------------------------------------
void
CyTarget::HandleInterfaceMessage( BMessage *msg )
{
	char *chptr;
	uint32 index = 0;
	for ( ; msg->FindString(kTextTag, index, &chptr) == B_OK; ++index )
		PutText( istring("*** ") + chptr + "\n" );
}


// -----------------------------------------------------------------------------
//	HandleServerNumeric
// -----------------------------------------------------------------------------
void
CyTarget::HandleServerNumeric( BMessage *msg )
{
	int16	numeric;
	if ( msg->FindInt16(kNumericTag, &numeric) != B_OK ) return;
	
	char *	chptr;
	
	switch (numeric)
	{
		case RPL_NAMREPLY:
			if ( msg->FindString(kParamTag,3,&chptr) == B_OK )
				AddUsers(istring(chptr));
			break;
			
		case RPL_TOPIC:
			if ( msg->FindString(kParamTag,2,&chptr) == B_OK )
				SetTopic(istring(chptr));
			break;
		
		case RPL_ENDOFNAMES:
		case 333: // topic set by ___ at ____.
			break;
			
		case RPL_INVITING:
		case RPL_BANLIST:
		
		default:
		{
			PutText( "*** " );
			uint32	index = 1;
			for ( ; msg->FindString(kParamTag, index, &chptr) == B_OK; ++index )
			{
				PutText( istring(chptr) + " " );
			}
			PutText( "\n" );
			break;
		}
	}
}


// -----------------------------------------------------------------------------
//	HandleServerCommand -- long-assed function
// -----------------------------------------------------------------------------
void
CyTarget::HandleServerCommand( BMessage *msg )
{
	char *chptr;
	if ( msg->FindString(kCommandTag, &chptr) != B_OK ) return;
	istring command( chptr );
	
	istring	nick, target;
	
	if ( msg->FindString(kNickTag, &chptr) == B_OK ) nick = chptr;
	else if ( msg->FindString(kPrefixTag, &chptr) == B_OK ) nick = chptr;
	
	if ( msg->FindString(kParamTag, 0, &chptr) == B_OK )	target = chptr;
	
	//	PRIVMSG ------------------------------------------------------------
	//	:mgb!mgb@ppp44.pm3a.wport.com PRIVMSG #bedevtalk :Hi.
	if ( command == "PRIVMSG" )
	{
		if ( msg->FindString(kParamTag, 1, &chptr) != B_OK ) return;
		DoPrivMsg( nick, target, chptr );
	}

	//	ACTION ? -----------------------------------------------------------
	else if ( command == "ACTION" )
	{
		if ( msg->FindString(kParamTag, 1, &chptr) != B_OK ) return;
		DoAction( nick, target, chptr );
	}
	
	//	JOIN ---------------------------------------------------------------
	//	:Avatar22!jason@jeff.CES.CWRU.Edu JOIN :#bedevtalk
	else if ( command == "JOIN" )
	{
		if ( msg->FindString(kPrefixTag, &chptr) != B_OK ) return;
		istring userhost( chptr );
		userhost = userhost.substr( userhost.find_first_of("!") + 1 );
		DoJoin( nick, userhost, target );
		
		// don't add "me" - it'll be added by AddUsers():
		
		if ( nick != _dispatcher->environment.Get("NICK") )
			AddUser(nick);
	}
	
	//	PART ---------------------------------------------------------------
	//	:Avatar22!jason@jeff.CES.CWRU.Edu PART #bedevtalk
	else if ( command == "PART" )
	{
		DoPart( nick, target );
		
		if ( IsUserOnChannel(nick) )
			RemoveUser(nick);
		
		// did I just leave this channel?:
		
		if ( (nick == _dispatcher->environment.Get("NICK"))
				&& (_name == target) )
		{
			Exit();
			delete this;
		}
	}
	
	//	QUIT ---------------------------------------------------------------
	//	:Avatar22!jason@jeff.CES.CWRU.Edu QUIT :brb
	else if ( command == "QUIT" )
	{
		if ( IsUserOnChannel(nick) )
		{
			DoQuit( nick, target );
			RemoveUser(nick);
		}
	}
	
	//	NICK -------------------------------------------------------------------
	//	:Avatar22_!jason@jeff.CES.CWRU.Edu NICK :Avatar22
	else if ( command == "NICK" )
	{
		if ( IsUserOnChannel(nick) )
		{
			if ( msg->FindString(kParamTag, &chptr) != B_OK ) return;
			istring newnick(chptr);
			
			DoNick( newnick, nick );
			
			RemoveUser(nick);
			AddUser(newnick);
			
			if ( _type == PRIVATE )
				if ( nick == _name )
				{
					_dispatcher->RemoveTarget(_name);
					_name = newnick;
					SetName(_name);
					_dispatcher->SetTargetHandler( _name, this );
				}
		}
	}
	
	//	MODE ---------------------------------------------------------------
	//	:CyTest!~jason@b64094.STUDENT.CWRU.Edu MODE #bedevtalk +o Avatar22 
	//	:CyTest MODE CyTest :+i
	else if ( command == "MODE" )
	{
		if ( msg->FindString(kParamTag, 1, &chptr) != B_OK ) return;
		istring modes( chptr );
		istring params( "" );
		
		int len = modes.length(), n = 2;
		bool set = false, param = false;
		
		for ( int i = 0; i < len; i++ )
		{
			if ( modes[i] == '+' ) set = true;
			else if ( modes[i] == '-' ) set = false;
			else {
				
				if ( (modes[i] == 'o') || (modes[i] == 'v')
					|| (modes[i] == 'b') || (modes[i] == 'k') )
				{
					if ( msg->FindString(kParamTag, n++, &chptr) != B_OK )
						chptr = "";
					params += " ";
					params += chptr;
				}
				else chptr = "";
				
				if ( modes[i] == 'o' )
					SetUserOp( chptr,set );
				else if ( modes[i] == 'v' )
					SetUserVoice( chptr,set );
			}
		}
		DoMode( nick, target, modes, params );
	}
	
	//	KICK ---------------------------------------------------------------
	//	:Avatar22!jason@jeff.CES.CWRU.Edu KICK #bedevtalk Avatar22 :Avatar22
	else if ( command == "KICK" )
	{
		if ( msg->FindString(kParamTag, 1, &chptr) != B_OK ) return;
		istring kicked( chptr );
		if ( msg->FindString(kParamTag, 2, &chptr) != B_OK ) return;		
		DoKick( nick, target, kicked, chptr );
		
		if ( IsUserOnChannel(kicked) )
			RemoveUser(kicked);

		if ( _type == PUBLIC )
			if ( kicked == _dispatcher->environment.Get("NICK") )
			{
				Exit();
				delete this;
			}
	}
	
	//	TOPIC --------------------------------------------------------------
	//	:mgb!mgb@ppp44.pm3a.wport.com TOPIC #bedevtalk :test		
	else if ( command == "TOPIC" )
	{
		if ( msg->FindString(kParamTag, 1, &chptr) != B_OK ) return;
		DoTopic( nick, target, chptr );
		SetTopic( istring(chptr) );
	}
	
	//	NOTICE ------------------------------------------------------------
	//	:Avatar22!jason@jeff.CES.CWRU.Edu NOTICE cytest :test
	else if ( command == "NOTICE" )
	{
		if ( msg->FindString(kParamTag, 1, &chptr) != B_OK ) return;
		DoNotice( nick, target, chptr );
	}
	
	//	ERROR ------------------------------------------------------------------
	//	ERROR :Closing Link: b64094.STUDENT.CWRU.Edu (blah)
	else if ( command == "ERROR" )
	{
		DoError( target );
	}
}

// -----------------------------------------------------------------------------
//	AddUser
// -----------------------------------------------------------------------------
void
CyTarget::AddUser( const istring &nick, const bool op, const bool v )
{
	_users.insert(nick);
}


// -----------------------------------------------------------------------------
//	AddUsers
// "nicks" contains a list of nicknames from a RPL_NAMREPLY message.
// The names are separated by whitespace. They may be prefixed by @ (operator) 
// or + (voiced), but not both.
// -----------------------------------------------------------------------------
void
CyTarget::AddUsers( const istring &nicks )
{
	istring::size_type a = 0, b = 0;
	istring temp, nick;
	bool op, v;
	
	while (a < nicks.size() )
	{
		op = v = false;
		
		temp = nicks.substr(a);
		b = temp.find(' ');
		
		if ( temp[0] == '@' ) // op
		{
			nick = temp.substr(1,b-1);
			op = true;
		}
		else if ( temp[0] == '+' ) // voiced
		{
			nick = temp.substr(1,b-1);
			v = true;
		}
		else nick = temp.substr(0,b);
		
		if (!IsUserOnChannel(nick))
			AddUser(nick,op,v);
		
		a += (b+1);
	}
}


// -----------------------------------------------------------------------------
//	IsUserOnChannel
// -----------------------------------------------------------------------------
bool
CyTarget::IsUserOnChannel( const istring &nick )
{
	return ( _users.find(nick) != _users.end() );
}


// -----------------------------------------------------------------------------
//	RemoveUser
// -----------------------------------------------------------------------------
void CyTarget::RemoveUser( const istring &nick )
{
	_users.erase(nick);
}


// -----------------------------------------------------------------------------
//	Exit
// -----------------------------------------------------------------------------
void CyTarget::Exit()
{
	if ( Type() == CONSOLE )
		_dispatcher->RemoveConsole();
	else
		_dispatcher->RemoveTarget(_name);
}