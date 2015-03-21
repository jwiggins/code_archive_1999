#ifndef DRAWVIEW_H
#include "drawView.h"
#endif
#include "Point.h"

drawView::drawView(BRect frame)
		: BView(frame, "picasso", B_FOLLOW_ALL, B_WILL_DRAW)
{
	// parallela-whatever
	//a.x= 0; a.y= 50; a.z= -50;
	//b.x= 50; b.y= 50; b.z= -50;
	//c.x= 0; c.y= -50; c.z= -50;
	//d.x= -50; d.y= -50; d.z= -50;
	//e.x= 0; e.y= 50; e.z= 50;
	//f.x= 50; f.y= 50; f.z= 50;
	//g.x= 0; g.y= -50; g.z= 50;
	//h.x= -50; h.y= -50; h.z= 50;
	
	// pyramid
	//a.x= 0; a.y= 0; a.z= -50;
	//b.x= 0; b.y= 0; b.z= -50;
	//c.x= 0; c.y= 0; c.z= -50;
	//d.x= 0; d.y= 0; d.z= -50;
	//e.x= -50; e.y= 50; e.z= 50;
	//f.x= 50; f.y= 50; f.z= 50;
	//g.x= 50; g.y= -50; g.z= 50;
	//h.x= -50; h.y= -50; h.z= 50;
	
	// cube
	a.x= -50; a.y= 50; a.z= -50;
	b.x= 50; b.y= 50; b.z= -50;
	c.x= 50; c.y= -50; c.z= -50;
	d.x= -50; d.y= -50; d.z= -50;
	e.x= -50; e.y= 50; e.z= 50;
	f.x= 50; f.y= 50; f.z= 50;
	g.x= 50; g.y= -50; g.z= 50;
	h.x= -50; h.y= -50; h.z= 50;
}

void drawView::AttachedToWindow()
{
	draw();
}

void drawView::draw()
{
	SetHighColor(0,0,0);
	FillRect(Bounds());
	
	// a1 is the alpha point, everything sets off of it
	// front square
	BPoint	a1(a.x+124,a.y+124),
			a2(b.x+124,b.y+124),
			a3(c.x+124,c.y+124),
			a4(d.x+124,d.y+124);
	// back square
	BPoint	b1(e.x+124,e.y+124),
			b2(f.x+124,f.y+124),
			b3(g.x+124,g.y+124),
			b4(h.x+124,h.y+124);
	// declared....
	// tell me what they is 
	/*
	printf("a1 ");
	a1.PrintToStream();
	printf("a2 ");
	a2.PrintToStream();
	printf("a3 ");
	a3.PrintToStream();
	printf("a4 ");
	a4.PrintToStream();
	printf("b1 ");
	b1.PrintToStream();
	printf("b2 ");
	b2.PrintToStream();
	printf("b3 ");
	b3.PrintToStream();
	printf("b4 ");
	b4.PrintToStream();
	*/
	// end tell
	// my color
	rgb_color color; 
	color.red = color.blue = 0;
	color.green = 255;
	// end color
	SetPenSize(1.0);
	//SetHighColor(0,255,0);
	//aRect.Set(99,99,199,199);
	BeginLineArray(12);
	//StrokeRect(aRect);
	// front square
	AddLine(a1,a2,color);
	AddLine(a2,a3,color);
	AddLine(a3,a4,color);
	AddLine(a4,a1,color);
	// back square
	AddLine(b1,b2,color);
	AddLine(b2,b3,color);
	AddLine(b3,b4,color);
	AddLine(b4,b1,color);
	// connecters
	AddLine(a1,b1,color);
	AddLine(a2,b2,color);
	AddLine(a3,b3,color);
	AddLine(a4,b4,color);
	EndLineArray();
	//aRect.Set(49,49,149,149);
	//StrokeRect(aRect);
	Sync();
}