/*
	
	PView.cpp
	
	John Wiggins. 1998
	
*/

#ifndef PVIEW_H
#include "PView.h"
#endif
#include <stdlib.h> // sprintf()
#include <OS.h> // threading

HelloView::HelloView(BRect rect, char *name)
	   	   : BView(rect, name, B_FOLLOW_ALL, B_WILL_DRAW | B_PULSE_NEEDED)
{
}


void HelloView::AttachedToWindow()
{
	SetHighColor(255,255,0); 
	SetViewColor(220,220,220);
	BRect 		rect;
	
	rect.Set(0, 0, 99, 99); // set the rect for the bitmap. I couldnt put the 
							// top left corner anywhere but (0,0). i dont know
							// why, although i could have missed something in
							// the BeBook
	foo_bitmap = new BBitmap(rect, B_COLOR_8_BIT); // construct
	rect.Set(0, 109, 144, 109 + B_H_SCROLL_BAR_HEIGHT);
	scroller = new BScrollBar(rect, "scroller", NULL, 6.0, 1004.0, B_HORIZONTAL);
	scroller->SetValue(6.0); // scrollbar starts out at 6.0
	scroller->SetSteps(1.0, 10.0); // arrows = 1.0, inside the scroller = 10.0
	AddChild(scroller); // add it to the view
	
	rect.Set(104, 5, 139, 34);
	box = new BBox(rect, "box"); // make the box that the pattern # goes in
	box->SetLabel("#"); // add the label
	AddChild(box); // add it to the view
	rect.Set(5, 10, 25, 25);
	poo = new BStringView(rect, "poo", NULL); // initialize the stringview
	box->AddChild(poo); // add it to the BBox. (derived from BView)
}

void HelloView::Pulse()
{
	// this code gets executed every 150,000 microseconds
	// the threading code somes straight out of the BeBook
	// i hand it a static member function, name it, give it a priority of
	// 10, and give it a pointer to this object
	my_thread = spawn_thread(thread_func, "blit", B_NORMAL_PRIORITY, (void *)this);
	resume_thread(my_thread); // execute while the view can do other stuff
	
	// im not quite sure, but this thread may be useless if code inside 
	// Pulse() is in its own thread anyways..... But then, i learned simple
	// threading in the process :)
}

int32 HelloView::thread_func(void *arg)
{
	HelloView *obj = (HelloView *)arg; // cast arg (which was *this) to my 
										// View's type
	return (obj->threadFunc());			// call the real function and return
										// whatever it returns
}

int32 HelloView::threadFunc()
{
	/* Machst du Spass hier */
	// my german is just as messy as my code
	uchar *picbuffer = (uchar *)foo_bitmap->Bits(); // get a pointer to the 
													// bitmap's first bit
	char scroller_string[4]; // make a string to hold the pattern #
	int value = (int)scroller->Value(); // find out the scrollers position
										// and cast it to int so we can %
	sprintf(scroller_string, "%d", value - 5); // dump the scroller's value
												// into a string
	for( int i=1; i<=10000; i++) // initialize the bitmap
	{
		srand(system_time()); // seed rand() to the system clock
		*picbuffer++ = rand() % value; // This is the whole app right here.
	}									// nifty huh?
										// the *picbuffer++ interates what yer 
										// pointing at. (Found that in Benoit's code)
	
	Window()->Lock(); // lock the looper
	((BStringView *)ChildAt(1)->ChildAt(0))->SetText((const char *)scroller_string);
						// ugh. Call BView's ChildAt() and cast the result to
						// BStringView so you can call it's SetText() function
	DrawBitmap(foo_bitmap); // draw the bitmap. (yay)
	Window()->Unlock(); // unlock and die
	return B_OK;
}