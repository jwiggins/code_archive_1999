#ifndef MGB_TARGET_H

#include "CyTarget.h"
#include "CyDispatcher.h"
#include "CyChannelWindow.h"

#pragma export on
extern "C" BLooper *instantiate_dispatcher( BLooper *, CyEnvironment * );
#pragma export reset

// MgbDispatcher

class MgbDispatcher : public CyDispatcher
{
public:
	MgbDispatcher( BLooper *connection, CyEnvironment *environment );
	CyTarget *MakeNewTarget( const istring &name, target_type type, uint32 level = LEVEL_NONE );
};


// MgbTarget

class MgbTarget : public CyTarget
{
private:
	CyChannelWindow *_win;
public:
	MgbTarget(const istring &name, CyDispatcher *dispatcher, 
		target_type type, uint32 level);
	
	virtual ~MgbTarget();
	
	void PutText( const istring &text, const text_run_array *text_runs );
	
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

#define MGB_TARGET_H
#endif