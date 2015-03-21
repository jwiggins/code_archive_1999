// ============================================================================= 
//   CyGlobals.h                                            Created: 04/09/98 
// 
//   I don't have the real headers yet
// 
//   History:
//      04/25/98  mnl    added k*Func, CY_DEBUG_MESSAGE, and CY_CLIENT_MESSAGE
//      04/25/98  mnl    added k*Tag
//      04/09/98  mnl    conception 
// =============================================================================

#ifndef CY_GLOBALS_H_

enum {
	CY_SEND_INPUT		= 'SeIn',
	CY_SERVER_MESSAGE	= 'SeMe',
	CY_CLIENT_COMMAND	= 'ClCo',
	CY_CLIENT_MESSAGE	= 'ClMe',
	CY_DEBUG_MESSAGE	= 'Debg'
};

// Parser Add-ons, client side
static const char *kClientEditingFunc			= "cl_edit";		// pointer
static const char *kClientFormattingFunc		= "cl_format";		// pointer
static const char *kClientCommandFunc			= "cl_cmd";			// pointer
static const char *kClientCommandName			= "cl_cmdname";		// string
static const char *kClientText					= "cl_text";		// string

// Parser Add-ons, server side
static const char *kServerEditingFunc			= "sv_edit";		// pointer
static const char *kServerFormattingFunc		= "sv_format";		// pointer
static const char *kServerTriggerFunc			= "sv_trig";		// pointer
static const char *kServerText					= "sv_text";		// string

// Message fields
static const char *kRawTag						= "raw";			// string
static const char *kPrefixTag					= "prefix";			// string
static const char *kCommandTag					= "command";		// string
static const char *kNumericTag					= "numeric";		// uint8
static const char *kParamTag					= "param";			// string(s)
static const char *kTimeTag						= "time";			// int64
static const char *kCmdLineTag     				= "commandline";	// string
static const char *kDebugTag 					= "debug";			// string
static const char *kTargetTag					= "target";			// string

// Application Signature
static const char *kApplicationSig 	= "application/x-vnd.cyclone-ui";

#define CY_GLOBALS_H_ 1
#endif