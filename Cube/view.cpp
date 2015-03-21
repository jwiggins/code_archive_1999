/*
	
	view.cpp
	
	John Wiggins. generic app shell. 1998
	
*/

#ifndef VIEW_H
#include "view.h"
#endif
#ifndef DRAWVIEW_H
#include "drawView.h"
#endif

View::View(BRect rect, char *name)
	   	   : BView(rect, name, B_FOLLOW_ALL, B_WILL_DRAW)
{
}

View::~View()
{
	foo->RemoveChild(picasso);
	delete picasso;
	delete foo;
}

void View::AttachedToWindow()
{
	foo = new BBitmap(Bounds(), B_COLOR_8_BIT, true);
	picasso = new drawView(foo->Bounds());
	foo->AddChild(picasso);
}


void View::Draw(BRect)
{
	DrawBitmap(foo);
}

void View::MouseDown(BPoint point)
{
	mouse_thread = spawn_thread(thread_start, "picasso", B_DISPLAY_PRIORITY, (void *)this);
	resume_thread(mouse_thread);
	//printf("Spawning picasso\n");
}

int32 View::threadFunc()
{
	uint32 buttons =0;
	bigtime_t start, finish;
	Window()->Lock();
	GetMouse(&current, &buttons, true);
	Window()->Unlock();
	previous = current; //get a copy of the point
	while( buttons )
	{
		start = system_time();
		//printf("\nstart loop\n");

		double n[3], dr, denom, cos_theta, sin_theta, w;
		int  dx, dy;
		double Rmat[4][4];
 		static double Radius=75.0;
 		
 		//current -= previous; // find the change
		dx = (current.x - previous.x); dy = (current.y - previous.y); // init dy and dx
		// debug
		//printf("dx = %d\n dy = %d\n", dx, dy);
		// end debug
 		dr = sqrt((double)(dx*dx + dy*dy)); // distance traveled in 2d
 		//printf("dr = %d\n", (int)dr);
 		if((dx==0) && (dy==0) && (dr==0))
 		{
 			goto SLEEPY_TIME; // uhh, the mouse aint movin, so we get a break.
 		}
 		
     	denom = sqrt(Radius*Radius + dr*dr);  
     	cos_theta = Radius/denom;
     	sin_theta = dr/denom;
     	// i am pretty sure n[] is the normal vector
     	n[0] = (double)(dy)/dr;   // Change sign for right-handed coord system. 
     	n[1] = -(double)(dx)/dr;
    	n[2] = 0.0;
    	
    	// make the rotation matrix
    	Rmat[0][0] = ((1-cos_theta)*(n[0]*n[0])) + cos_theta;
    	Rmat[0][1] = ((1-cos_theta)*(n[0]*n[1])) + sin_theta*n[2];
    	Rmat[0][2] = ((1-cos_theta)*(n[0]*n[2])) - sin_theta*n[1];
    	Rmat[0][3] = 0;
    	Rmat[1][0] = ((1-cos_theta)*(n[0]*n[1])) - sin_theta*n[2];
    	Rmat[1][1] = ((1-cos_theta)*(n[1]*n[1])) + cos_theta;
    	Rmat[1][2] = ((1-cos_theta)*(n[1]*n[2])) + sin_theta*n[0];
    	Rmat[1][3] = 0;
    	Rmat[2][0] = ((1-cos_theta)*(n[0]*n[2])) + sin_theta*n[1];
    	Rmat[2][1] = ((1-cos_theta)*(n[1]*n[2])) - sin_theta*n[0];
    	Rmat[2][2] = ((1-cos_theta)*(n[2]*n[2])) + cos_theta;
    	Rmat[2][3] = 0;
    	Rmat[3][0] = 0;
    	Rmat[3][1] = 0;
    	Rmat[3][2] = 0;
    	Rmat[3][3] = 1;
    	
    	// make copies of all points (ick.. i know)
    	three_d_point	tempa, tempb, tempc, tempd,
    				tempe, tempf, tempg, temph;
    	tempa.x = picasso->a.x; tempa.y = picasso->a.y; tempa.z = picasso->a.z;
   		tempb.x = picasso->b.x; tempb.y = picasso->b.y; tempb.z = picasso->b.z;
   		tempc.x = picasso->c.x; tempc.y = picasso->c.y; tempc.z = picasso->c.z;
   		tempd.x = picasso->d.x; tempd.y = picasso->d.y; tempd.z = picasso->d.z;
   		tempe.x = picasso->e.x; tempe.y = picasso->e.y; tempe.z = picasso->e.z;
  	 	tempf.x = picasso->f.x; tempf.y = picasso->f.y; tempf.z = picasso->f.z;
  	 	tempg.x = picasso->g.x; tempg.y = picasso->g.y; tempg.z = picasso->g.z;
  	 	temph.x = picasso->h.x; temph.y = picasso->h.y; temph.z = picasso->h.z;
  	 	// end copy
 	  	// translate all eight points with this matrix
 	  	//a
  	 	picasso->a.x = (tempa.x * Rmat[0][0]) + (tempa.y * Rmat[1][0]) + (tempa.z * Rmat[2][0]) + Rmat[3][0];
  	 	picasso->a.y = (tempa.x * Rmat[0][1]) + (tempa.y * Rmat[1][1]) + (tempa.z * Rmat[2][1]) + Rmat[3][1];
  	 	picasso->a.z = (tempa.x * Rmat[0][2]) + (tempa.y * Rmat[1][2]) + (tempa.z * Rmat[2][2]) + Rmat[3][2];
  	 	w = (tempa.x * Rmat[0][3]) + (tempa.y * Rmat[1][3]) + (tempa.z * Rmat[2][3]) + Rmat[3][3];
		if (w != 0.0) { picasso->a.x /= w;  picasso->a.y /= w;  picasso->a.z /= w; }
		//b
  		picasso->b.x = (tempb.x * Rmat[0][0]) + (tempb.y * Rmat[1][0]) + (tempb.z * Rmat[2][0]) + Rmat[3][0];
  	 	picasso->b.y = (tempb.x * Rmat[0][1]) + (tempb.y * Rmat[1][1]) + (tempb.z * Rmat[2][1]) + Rmat[3][1];
  	 	picasso->b.z = (tempb.x * Rmat[0][2]) + (tempb.y * Rmat[1][2]) + (tempb.z * Rmat[2][2]) + Rmat[3][2];
  	 	w = (tempb.x * Rmat[0][3]) + (tempb.y * Rmat[1][3]) + (tempb.z * Rmat[2][3]) + Rmat[3][3];
  		if (w != 0.0) { picasso->b.x /= w;  picasso->b.y /= w;  picasso->b.z /= w; }
 		//c
  	 	picasso->c.x = (tempc.x * Rmat[0][0]) + (tempc.y * Rmat[1][0]) + (tempc.z * Rmat[2][0]) + Rmat[3][0];
  	 	picasso->c.y = (tempc.x * Rmat[0][1]) + (tempc.y * Rmat[1][1]) + (tempc.z * Rmat[2][1]) + Rmat[3][1];
  	 	picasso->c.z = (tempc.x * Rmat[0][2]) + (tempc.y * Rmat[1][2]) + (tempc.z * Rmat[2][2]) + Rmat[3][2];
  	 	w = (tempc.x * Rmat[0][3]) + (tempc.y * Rmat[1][3]) + (tempc.z * Rmat[2][3]) + Rmat[3][3];
		if (w != 0.0) { picasso->c.x /= w;  picasso->c.y /= w;  picasso->c.z /= w; }
		//d
  	 	picasso->d.x = (tempd.x * Rmat[0][0]) + (tempd.y * Rmat[1][0]) + (tempd.z * Rmat[2][0]) + Rmat[3][0];
 	 	picasso->d.y = (tempd.x * Rmat[0][1]) + (tempd.y * Rmat[1][1]) + (tempd.z * Rmat[2][1]) + Rmat[3][1];
  	 	picasso->d.z = (tempd.x * Rmat[0][2]) + (tempd.y * Rmat[1][2]) + (tempd.z * Rmat[2][2]) + Rmat[3][2];
 	  	w = (tempd.x * Rmat[0][3]) + (tempd.y * Rmat[1][3]) + (tempd.z * Rmat[2][3]) + Rmat[3][3];
		if (w != 0.0) { picasso->d.x /= w;  picasso->d.y /= w;  picasso->d.z /= w; }
 	  	//e
 	  	picasso->e.x = (tempe.x * Rmat[0][0]) + (tempe.y * Rmat[1][0]) + (tempe.z * Rmat[2][0]) + Rmat[3][0];
	   	picasso->e.y = (tempe.x * Rmat[0][1]) + (tempe.y * Rmat[1][1]) + (tempe.z * Rmat[2][1]) + Rmat[3][1];
 	  	picasso->e.z = (tempe.x * Rmat[0][2]) + (tempe.y * Rmat[1][2]) + (tempe.z * Rmat[2][2]) + Rmat[3][2];
 	  	w = (tempe.x * Rmat[0][3]) + (tempe.y * Rmat[1][3]) + (tempe.z * Rmat[2][3]) + Rmat[3][3];
		if (w != 0.0) { picasso->e.x /= w;  picasso->e.y /= w;  picasso->e.z /= w; }
 	  	//f
 	  	picasso->f.x = (tempf.x * Rmat[0][0]) + (tempf.y * Rmat[1][0]) + (tempf.z * Rmat[2][0]) + Rmat[3][0];
 	  	picasso->f.y = (tempf.x * Rmat[0][1]) + (tempf.y * Rmat[1][1]) + (tempf.z * Rmat[2][1]) + Rmat[3][1];
  	 	picasso->f.z = (tempf.x * Rmat[0][2]) + (tempf.y * Rmat[1][2]) + (tempf.z * Rmat[2][2]) + Rmat[3][2];
 	  	w = (tempf.x * Rmat[0][3]) + (tempf.y * Rmat[1][3]) + (tempf.z * Rmat[2][3]) + Rmat[3][3];		if (w != 0.0) { picasso->f.x /= w;  picasso->f.y /= w;  picasso->f.z /= w; }
   	 	//g
  	 	picasso->g.x = (tempg.x * Rmat[0][0]) + (tempg.y * Rmat[1][0]) + (tempg.z * Rmat[2][0]) + Rmat[3][0];
  	 	picasso->g.y = (tempg.x * Rmat[0][1]) + (tempg.y * Rmat[1][1]) + (tempg.z * Rmat[2][1]) + Rmat[3][1];
  	 	picasso->g.z = (tempg.x * Rmat[0][2]) + (tempg.y * Rmat[1][2]) + (tempg.z * Rmat[2][2]) + Rmat[3][2];
  	 	w = (tempg.x * Rmat[0][3]) + (tempg.y * Rmat[1][3]) + (tempg.z * Rmat[2][3]) + Rmat[3][3];
		if (w != 0.0) { picasso->g.x /= w;  picasso->g.y /= w;  picasso->g.z /= w; }
  	 	//h
  	 	picasso->h.x = (temph.x * Rmat[0][0]) + (temph.y * Rmat[1][0]) + (temph.z * Rmat[2][0]) + Rmat[3][0];
 	  	picasso->h.y = (temph.x * Rmat[0][1]) + (temph.y * Rmat[1][1]) + (temph.z * Rmat[2][1]) + Rmat[3][1];
  	 	picasso->h.z = (temph.x * Rmat[0][2]) + (temph.y * Rmat[1][2]) + (temph.z * Rmat[2][2]) + Rmat[3][2];
  	 	w = (temph.x * Rmat[0][3]) + (tempa.y * Rmat[1][3]) + (temph.z * Rmat[2][3]) + Rmat[3][3];
		if (w != 0.0) { picasso->h.x /= w;  picasso->h.y /= w;  picasso->h.z /= w; }
  	 	// end translate
   	
    	// draw
    	if(foo->Lock())
		{
			//printf("drawing...\n");
			picasso->draw();
			foo->Unlock();
			Window()->Lock();
			DrawBitmap(foo);
			Window()->Unlock();
		}
SLEEPY_TIME:
 		finish = system_time();
 		//printf("end loop\n\n");
 		snooze((20 * 1000) - (finish - start)); // sleep for what time we have left
 		previous = current;
		Window()->Lock();
		GetMouse(&current, &buttons, true);
		Window()->Unlock();
	}
}

int32 View::thread_start(void *arg)
{
	View *obj = (View *)arg;
	return (obj->threadFunc());
}

void View::translate()
{
	
}