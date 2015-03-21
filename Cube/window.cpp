/*
	
	window.cpp
	
	John Wiggins. generic app shell. 1998
	
*/

#ifndef _APPLICATION_H
#include <Application.h>
#endif

#ifndef WINDOW_H
#include "window.h"
#endif
#ifndef VIEW_H
#include "view.h"
#endif

Window::Window(BRect frame)
				: BWindow(frame, "Cube", B_TITLED_WINDOW,B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{

}

bool Window::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return(TRUE);
}

void Window::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
		case CUBE:
			((View *)ChildAt(0))->picasso->a.x= -50; 
			((View *)ChildAt(0))->picasso->a.y= 50; 
			((View *)ChildAt(0))->picasso->a.z= -50;
			((View *)ChildAt(0))->picasso->b.x= 50; 
			((View *)ChildAt(0))->picasso->b.y= 50; 
			((View *)ChildAt(0))->picasso->b.z= -50;
			((View *)ChildAt(0))->picasso->c.x= 50; 
			((View *)ChildAt(0))->picasso->c.y= -50; 
			((View *)ChildAt(0))->picasso->c.z= -50;
			((View *)ChildAt(0))->picasso->d.x= -50; 
			((View *)ChildAt(0))->picasso->d.y= -50; 
			((View *)ChildAt(0))->picasso->d.z= -50;
			((View *)ChildAt(0))->picasso->e.x= -50; 
			((View *)ChildAt(0))->picasso->e.y= 50; 
			((View *)ChildAt(0))->picasso->e.z= 50;
			((View *)ChildAt(0))->picasso->f.x= 50; 
			((View *)ChildAt(0))->picasso->f.y= 50; 
			((View *)ChildAt(0))->picasso->f.z= 50;
			((View *)ChildAt(0))->picasso->g.x= 50;
			((View *)ChildAt(0))->picasso->g.y= -50; 
			((View *)ChildAt(0))->picasso->g.z= 50;
			((View *)ChildAt(0))->picasso->h.x= -50;
			((View *)ChildAt(0))->picasso->h.y= -50; 
			((View *)ChildAt(0))->picasso->h.z= 50;
			break;
		case PYRAMID:
			((View *)ChildAt(0))->picasso->a.x= 0; 
			((View *)ChildAt(0))->picasso->a.y= 0; 
			((View *)ChildAt(0))->picasso->a.z= -50;
			((View *)ChildAt(0))->picasso->b.x= 0; 
			((View *)ChildAt(0))->picasso->b.y= 0; 
			((View *)ChildAt(0))->picasso->b.z= -50;
			((View *)ChildAt(0))->picasso->c.x= 0; 
			((View *)ChildAt(0))->picasso->c.y= 0; 
			((View *)ChildAt(0))->picasso->c.z= -50;
			((View *)ChildAt(0))->picasso->d.x= 0; 
			((View *)ChildAt(0))->picasso->d.y= 0; 
			((View *)ChildAt(0))->picasso->d.z= -50;
			((View *)ChildAt(0))->picasso->e.x= -50; 
			((View *)ChildAt(0))->picasso->e.y= 50; 
			((View *)ChildAt(0))->picasso->e.z= 50;
			((View *)ChildAt(0))->picasso->f.x= 50; 
			((View *)ChildAt(0))->picasso->f.y= 50; 
			((View *)ChildAt(0))->picasso->f.z= 50;
			((View *)ChildAt(0))->picasso->g.x= 50; 
			((View *)ChildAt(0))->picasso->g.y= -50; 
			((View *)ChildAt(0))->picasso->g.z= 50;
			((View *)ChildAt(0))->picasso->h.x= -50; 
			((View *)ChildAt(0))->picasso->h.y= -50; 
			((View *)ChildAt(0))->picasso->h.z= 50;
			break;
		case PARALLEL:
			((View *)ChildAt(0))->picasso->a.x= 0; 
			((View *)ChildAt(0))->picasso->a.y= 50; 
			((View *)ChildAt(0))->picasso->a.z= -50;
			((View *)ChildAt(0))->picasso->b.x= 50; 
			((View *)ChildAt(0))->picasso->b.y= 50; 
			((View *)ChildAt(0))->picasso->b.z= -50;
			((View *)ChildAt(0))->picasso->c.x= 0; 
			((View *)ChildAt(0))->picasso->c.y= -50; 
			((View *)ChildAt(0))->picasso->c.z= -50;
			((View *)ChildAt(0))->picasso->d.x= -50; 
			((View *)ChildAt(0))->picasso->d.y= -50; 
			((View *)ChildAt(0))->picasso->d.z= -50;
			((View *)ChildAt(0))->picasso->e.x= 0; 
			((View *)ChildAt(0))->picasso->e.y= 50; 
			((View *)ChildAt(0))->picasso->e.z= 50;
			((View *)ChildAt(0))->picasso->f.x= 50; 
			((View *)ChildAt(0))->picasso->f.y= 50; 
			((View *)ChildAt(0))->picasso->f.z= 50;
			((View *)ChildAt(0))->picasso->g.x= 0; 
			((View *)ChildAt(0))->picasso->g.y= -50; 
			((View *)ChildAt(0))->picasso->g.z= 50;
			((View *)ChildAt(0))->picasso->h.x= -50; 
			((View *)ChildAt(0))->picasso->h.y= -50; 
			((View *)ChildAt(0))->picasso->h.z= 50;
			break;
		default:
			BWindow::MessageReceived(msg);
			break;
	}
}