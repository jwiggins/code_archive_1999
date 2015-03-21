#pragma once

#include <map>
#include <string>


#define CLIENT_INFORMATION		"CLIENT_INFORMATION"
#define CMDCHARS				"CMDCHARS"
#define REALNAME				"REALNAME"
#define USER_INFORMATION		"USER_INFORMATION"
#define NICK					"NICK"
#define HOSTNAME				"HOSTNAME"
#define SERVERHOST				"SERVERHOST"
#define SERVERPORT				"SERVERPORT"


// -----------------------------------------------------------------------------
//	CyEnvironment
//	The interface to an environment object.  This design allows shared libs
//	(add-ons) to access the implementation via the vtable.
// -----------------------------------------------------------------------------
class CyEnvironment {
public:
	virtual			~CyEnvironment() {};
	
	virtual istring	Get( const istring &key ) const = 0;
	virtual istring	operator[]( const istring &key ) const = 0;
	
	virtual bool	Exists( const istring &key ) const = 0;
	virtual bool	IsVisible( const istring &key ) const = 0;
	
	virtual void	Set( const istring &key, const istring &mapping,
						 bool visible = true ) = 0;
	virtual void	Unset( const istring &key ) = 0;
	
	virtual void	MoveToFirst() = 0;
	virtual bool	GetNext( istring *key, istring *mapping ) = 0;
};
