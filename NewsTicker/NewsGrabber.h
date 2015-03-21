#ifndef NEWS_GRABBER_H
#define NEWS_GRABBER_H

#include <Looper.h>
#include <Alert.h>
#include <string.h>
#include <time.h>
#include <net/socket.h>
#include <net/netdb.h>

class NewsGrabber : public BLooper {
public:
					NewsGrabber();
virtual void			MessageReceived(BMessage *msg);
private:
void					GrabNews(const char *SiteHost, const char *URI, char *buffer);
bool 				has_tcp_data_come_in(int socket, int seconds); 
void					do_error(const char *error);
};

#endif