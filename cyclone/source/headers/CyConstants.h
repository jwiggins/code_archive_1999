// ============================================================================= 
//	CyConstants.h											Created: 04/29/98
//
//	Application constants definitions.
//
//	History:
//			04/49/98 JDG    Creation.
//			05/05/98 JDG	Removed static for single copy; disabled linker
//							warning in makefile with -msg nodup.
// =============================================================================


#ifndef __CY_CONSTANTS__
#define __CY_CONSTANTS__


const char *kCycloneSignature				= "application/x-vnd.cyclone-app";
const char *kCycloneVersion					= "0.2a/PPC";
const char *kCycloneAppString				= "Cyclone for BeOS";

const char *kRawTag							= "raw";			// string
const char *kPrefixTag						= "prefix";			// string
const char *kNickTag						= "nick";			// string
const char *kNameTag						= "name";			// string
const char *kHostTag						= "host";			// string
const char *kCommandTag						= "command";		// string
const char *kNumericTag						= "numeric";		// int8
const char *kCommandEnumTag					= "cmdenum";		// int32
const char *kParamTag						= "param";			// string
const char *kTimeTag						= "time";			// int64
const char *kSourceTag						= "source";			// string
const char *kTargetTag						= "target";			// string
const char *kDebugTag						= "debug";			// string
const char *kClientTextTag					= "cl_text";		// string

const char *kTextTag						= "text";			// string

const char *kClientEditingFunc				= "cl_edit";		// pointer
const char *kClientFormattingFunc			= "cl_format";		// pointer
const char *kClientCommandFunc				= "cl_cmd";			// pointer
const char *kClientCommandName				= "cl_cmdname";		// string
const char *kClientUnknownCommandFunc		= "cl_unkcmd";		// string

const char *kServerEditingFunc				= "sv_edit";		// pointer
const char *kServerFormattingFunc			= "sv_format";		// pointer
const char *kServerTriggerFunc				= "sv_trig";		// pointer
const char *kServerText						= "sv_text";		// string
const char *kCyConnectionLooper				= "connectlooper";	// pointer
const char *kCyEnvironment					= "environment";	// pointer

const char *kParserCommandAddonDir			= "commands/";
const char *kParserAddonConfigSymbol		= "configure";

const char *kInterfaceAddonImage			= "interface_addon";
const char *kInterfaceLooperSymbol			= "instantiate_dispatcher";

const uint32 LEVEL_NONE 		= 1;
const uint32 LEVEL_PUBLIC		= 2;
const uint32 LEVEL_PRIVATE		= 4;
const uint32 LEVEL_NOTICE		= 8;
const uint32 LEVEL_GENERAL		= 16;
const uint32 LEVEL_ALL			= LEVEL_NONE|LEVEL_PUBLIC|LEVEL_NOTICE|LEVEL_GENERAL;

const rgb_color
	kEchoColor					= { 128, 0, 128, 255 },
	kActionColor				= { 255, 0, 0, 255 },
	kBeBackgroundGreyColor 		= { 216, 216, 216, 255 },
	kBeInactiveControlGreyColor	= { 240, 240, 240, 255 },
	kBeFocusBlueColor 			= { 0, 0, 229, 255 },
	kBeHighlightColor 			= { 255, 255, 255, 255 },
	kBeShadowColor				= { 152, 152, 152, 255 },
	kBeDarkShadowColor 			= { 108, 108, 108, 255 },
	kBeLightShadowColor 		= { 194, 194, 194, 255 },
	kBeButtonGreyColor 			= { 232, 232, 232, 255 },
	kBeInactiveGreyColor 		= { 127, 127, 127, 255 },
	kBeListSelectGreyColor 		= { 178, 178, 178, 255 },
	kBeTitleBarYellowColor 		= { 255, 203, 0, 255 },
	kBlackColor 				= { 0, 0, 0, 255 },
	kWhiteColor 				= { 255, 255, 255, 255 },
	kRedColor 					= { 255, 0, 0, 255 },
	kGreenColor 				= { 0, 167, 0, 255 },
	kLightGreenColor 			= { 90, 240,90,	255 },
	kBlueColor 					= { 49, 61, 225, 255 },
	kLightBlueColor 			= { 64, 162,255, 255 },
	kPurpleColor 				= { 144, 64, 221, 255 },
	kLightPurpleColor 			= { 166, 74, 255, 255 },
	kLavenderColor 				= { 193, 122, 255, 255 },
	kYellowColor 				= { 255, 203, 0, 255 },
	kOrangeColor 				= { 255, 163, 0, 255 },
	kFleshColor 				= { 255, 231, 186, 255 },
	kTanColor 					= { 208, 182, 121, 255 },
	kBrownColor 				= { 154, 110, 45, 255 },
	kLightMetallicBlueColor 	= { 143, 166, 240, 255 },
	kMedMetallicBlueColor 		= { 75, 96, 154, 255 },
	kDarkMetallicBlueColor 		= { 78, 89, 126, 255 };
	
const uint32
	CY_DEBUG_MESSAGE		= 'CyDB',
	CY_INTERFACE_MESSAGE	= 'CyIF',
	CY_SERVER_MESSAGE		= 'CySR',
	CY_CLIENT_MESSAGE		= 'CyCM',
	CY_CLIENT_COMMAND		= 'CyCC',
	CY_DISPLAY_ERROR		= 'CyEr',
	CY_PRIVMSG				= 'CyPr',
	CY_JOIN					= 'CyJo',
	CY_ACTION				= 'CyAC',
	CY_SEND_INPUT 			= 'CySI';

enum server_reply
{
	ERR_NOSUCHNICK			= 401,
	ERR_NOSUCHSERVER		= 402,
	ERR_NOSUCHCHANNEL		= 403,
	ERR_CANNOTSENDTOCHAN	= 404,
	ERR_TOOMANYCHANNELS		= 405,
	ERR_WASNOSUCHNICK		= 406,
	ERR_TOOMANYTARGETS		= 407,
	ERR_NOORIGIN			= 409,
	ERR_NORECIPIENT			= 411,
	ERR_NOTEXTTOSEND		= 412,
	ERR_NOTOPLEVEL			= 413,
	ERR_WILDTOPLEVEL		= 414,
	ERR_UNKNOWNCOMMAND		= 421,
	ERR_NOMOTD				= 422,
	ERR_NOADMININFO			= 423,
	ERR_FILEERROR			= 424,
	ERR_NONICKNAMEGIVEN		= 431,
	ERR_ERRONEUSNICKNAME	= 432,
	ERR_NICKNAMEINUSE		= 433,
	ERR_NICKCOLLISION		= 436,
	ERR_USERNOTINCHANNEL	= 441,
	ERR_NOTONCHANNEL		= 442,
	ERR_USERONCHANNEL		= 443,
	ERR_NOLOGIN				= 444,
	ERR_SUMMONDISABLED		= 445,
	ERR_USERSDISABLED		= 446,
	ERR_NOTREGISTERED		= 451,
	ERR_NEEDMOREPARAMS		= 461,
	ERR_ALREADYREGISTRED	= 462,
	ERR_NOPERMFORHOST		= 463,
	ERR_PASSWDMISMATCH		= 464,
	ERR_YOUREBANNEDCREEP	= 465,
	ERR_KEYSET				= 467,
	ERR_CHANNELISFULL		= 471,
	ERR_UNKNOWNMODE			= 472,
	ERR_INVITEONLYCHAN		= 473,
	ERR_BANNEDFROMCHAN		= 474,
	ERR_BADCHANNELKEY		= 475,
	ERR_NOPRIVILEGES		= 481,
	ERR_CHANOPRIVSNEEDED	= 482,
	ERR_CANTKILLSERVER		= 483,
	ERR_NOOPERHOST			= 491,
	ERR_UMODEUNKNOWNFLAG	= 501,
	ERR_USERSDONTMATCH		= 502,
	
	RPL_NONE				= 300,
	RPL_USERHOST			= 302,
	RPL_ISON				= 303,
	RPL_AWAY				= 301,
	RPL_UNAWAY				= 305,
	RPL_NOWAWAY				= 306,
	RPL_WHOISUSER			= 311,
	RPL_WHOISSERVER			= 312,
	RPL_WHOISOPERATOR		= 313,
	RPL_WHOISIDLE			= 317,
	RPL_ENDOFWHOIS			= 318,
	RPL_WHOISCHANNELS		= 319,
	RPL_WHOWASUSER			= 314,
	RPL_ENDOFWHOWAS			= 369,
	RPL_LISTSTART			= 321,
	RPL_LIST				= 322,
	RPL_LISTEND				= 323,
	RPL_CHANNELMODEIS		= 324,
	RPL_NOTOPIC				= 331,
	RPL_TOPIC				= 332,
	RPL_INVITING			= 341,
	RPL_SUMMONING			= 342,
	RPL_VERSION				= 351,
	RPL_WHOREPLY			= 352,
	RPL_ENDOFWHO			= 315,
	RPL_NAMREPLY			= 353,
	RPL_ENDOFNAMES			= 366,
	RPL_LINKS				= 364,
	RPL_ENDOFLINKS			= 365,
	RPL_BANLIST				= 367,
	RPL_ENDOFBANLIST		= 368,
	RPL_INFO				= 371,
	RPL_ENDOFINFO			= 374,
	RPL_MOTDSTART			= 375,
	RPL_MOTD				= 372,
	RPL_ENDOFMOTD			= 376,
	RPL_YOUREOPER			= 381,
	RPL_REHASHING			= 382,
	RPL_TIME				= 391,
	RPL_USERSSTART			= 392,
	RPL_USERS				= 393,
	RPL_ENDOFUSERS			= 394,
	RPL_NOUSERS				= 395,
	
	RPL_TRACELINK			= 200,
	RPL_TRACECONNECTING		= 201,
	RPL_TRACEHANDSHAKE		= 202,
	RPL_TRACEUNKNOWN		= 203,
	RPL_TRACEOPERATOR		= 204,
	RPL_TRACEUSER			= 205,
	RPL_TRACESERVER			= 206,
	RPL_TRACENEWTYPE		= 208,
	RPL_TRACELOG			= 261,
	RPL_STATSLINKINFO		= 211,
	RPL_STATSCOMMANDS		= 212,
	RPL_STATSCLINE			= 213,
	RPL_STATSNLINE			= 214,
	RPL_STATSILINE			= 215,
	RPL_STATSKLINE			= 216,
	RPL_STATSYLINE			= 218,
	RPL_ENDOFSTATS			= 219,
	RPL_STATSLLINE			= 241,
	RPL_STATSUPTIME			= 242,
	RPL_STATSOLINE			= 243,
	RPL_STATSHLINE			= 244,
	RPL_UMODEIS				= 221,
	RPL_LUSERCLIENT			= 251,
	RPL_LUSEROP				= 252,
	RPL_LUSERUNKNOWN		= 253,
	RPL_LUSERCHANNELS		= 254,
	RPL_LUSERME				= 255,
	RPL_ADMINME				= 256,
	RPL_ADMINLOC1			= 257,
	RPL_ADMINLOC2			= 258,
	RPL_ADMINEMAIL			= 259,
	
	// Reserved numerics
	RPL_TRACECLASS			= 209,
	RPL_STATSQLINE			= 217,
	RPL_SERVICEINFO			= 231,
	RPL_ENDOFSERVICES		= 232,
	RPL_SERVICE				= 233,
	RPL_SERVLIST			= 234,
	RPL_SERVLISTEND			= 235,
	RPL_WHOISCHANOP			= 316,
	RPL_KILLDONE			= 361,
	RPL_CLOSING				= 362,
	RPL_CLOSEEND			= 363,
	RPL_INFOSTART			= 373,
	RPL_MYPORTIS			= 384,
	ERR_YOUWILLBEBANNED		= 466,
	ERR_BADCHANMASK			= 476,
	ERR_NOSERVICEHOST		= 492
};

#endif	// __CY_CONSTANTS__
