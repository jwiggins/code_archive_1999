#ifndef MESSAGE_DEFS_H
#define MESSAGE_DEFS_H

enum {
GET_NEWS				= 'getn', /* request for news */
GOT_NEWS				= 'gotn', /* response to GET_NEWS */
SHOW_ABOUT				= 'abut',
SHOW_LINKS				= 'link',
TICKER_PULSE			= 'tick',
};

#endif