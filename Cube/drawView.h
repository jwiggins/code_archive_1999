#ifndef DRAWVIEW_H
#define DRAWVIEW_H

#ifndef _VIEW_H
#include <View.h>
#endif
#include "Point.h"

class drawView : public BView {
public:
					drawView(BRect frame);
virtual	void		AttachedToWindow();
void				draw();
// points
three_d_point		a,b,c,d,e,f,g,h;
};

#endif
