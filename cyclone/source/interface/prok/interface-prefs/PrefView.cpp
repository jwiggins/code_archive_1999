/*
	
	PrefView.cpp

*/

#ifndef PREF_VIEW_H
#include "PrefView.h"
#endif
#ifndef _COLOR_CONTROL_H
#include <ColorControl.h>
#endif

PrefView::PrefView(BRect rect, char *name)
	   	   : BView(rect, name, B_FOLLOW_ALL, B_WILL_DRAW)
{
	SetHighColor(255,255,0);
	SetViewColor(220,220,220);
}


void PrefView::AttachedToWindow()
{
	BColorControl	*colorPicker;
	
	colorPicker = new BColorControl(BPoint(5,5), B_CELLS_32x8,
									1., "_ColorPicker");
	
	AddChild(colorPicker);
}


void PrefView::Draw(BRect)
{
}
