#ifndef SPLITTER_VIEW_H
#define SPLITTER_VIEW_H

#include <View.h>
#include <Rect.h>
#include "MessageDefs.h"

class SplitterView : public BView {
public:
						SplitterView(BRect frame, const char *name, int32 menu_bottom);
virtual	void			Draw(BRect updateRect);
virtual	void			FrameResized(float width,float height);
virtual	void			FrameMoved(BPoint point);
virtual	void			MouseDown(BPoint);
void					SetPercentage(float num) { percentage = num; }
float					Percentage() const { return percentage; }

// mouse poller stuff
int32					MouseWatcher();
static int32			thread_start(void *arg);		

private:
BRect					dot_rect_one, dot_rect_two;
float					percentage;
int32					_menu_bottom;
};

#endif