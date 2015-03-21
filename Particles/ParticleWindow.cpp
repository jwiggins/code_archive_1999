/*
	
	StarWindow.cpp
	
	by Pierre Raynaud-Richard.
	
	Copyright 1998 Be Incorporated, All Rights Reserved.
	
*/

#ifndef _APPLICATION_H
#include <Application.h>
#endif

#ifndef PARTICLE_WINDOW_H
#include "ParticleWindow.h"
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <AppFileInfo.h>
#include <FindDirectory.h>
#include <File.h>
#include <Path.h>

status_t get_file_version_info(	directory_which	dir,
								char			*filename,
								version_info	*info);
// return the version_info of a file, described by its
// name and its generic folder (in find_directory syntax).
status_t get_file_version_info(	directory_which	dir,
								char			*filename,
								version_info	*info) {
	BPath 			path;
	BFile			file;
	status_t		res;
	BAppFileInfo	appinfo;

	// find the directory
	if ((res = find_directory(dir, &path)) != B_NO_ERROR)
		return res;

	// find the file
	path.Append(filename);
	file.SetTo(path.Path(), O_RDONLY);
	if ((res = file.InitCheck()) != B_NO_ERROR)
		return res;

	// get the version_info
	if ((res = appinfo.SetTo(&file)) != B_NO_ERROR)
		return res;
	return appinfo.GetVersionInfo(info, B_APP_VERSION_KIND);
}

ParticleWindow::ParticleWindow(BRect frame, const char *name)
: BDirectWindow(frame, name, B_TITLED_WINDOW, 0)
{
	BView			*view;
	int32			i;
	version_info	vi;

	// check if we need the special trick to work around the bug in
	// the R3 directwindow protocol. This bug only affects the
	// B_BUFFER_RESET flag : the app_server forget to set it when
	// showing a previously hidden window. If you don't care about
	// the B_BUFFER_RESET flag, you're not concerned by that work
	// around.
	need_r3_buffer_reset_work_around = false;
	if (get_file_version_info(B_BEOS_SERVERS_DIRECTORY, "app_server", &vi) == B_NO_ERROR)
		if ((vi.major == 1) && (vi.middle == 3) && (vi.minor == 0))
			need_r3_buffer_reset_work_around = true;
	
	// allocate the star struct array
	particle_count_max = 1024*5;
	particle_count = 0;
	particle_list = (particle *)malloc(sizeof(particle)*particle_count_max);

	// initialise the default state of the star array
	for (i=0; i<particle_count_max; i++) {
	
		uint32 rand_max = 0xffffffff;
		// seed the random number generator
		srandom(system_time());
		
		particle_list[i].init_velocity = -((double)((rand_max>>1)+(random()%(rand_max>>1)))/rand_max)*4.666; // magic number
		particle_list[i].gravity = -(((double)((rand_max>>1)+(random()%(rand_max>>1)))/rand_max))*0.777; // more magic
		
		// make the particle initialy invisible and fixed, but at a random moment in time
		particle_list[i].last_draw = INVALID;
		particle_list[i].timeval = random() & 64;
		particle_list[i].x = 0; // this gets figured out at drawtime
		particle_list[i].y = 0; // same here
		particle_list[i].cos_z_theta = cos(random() % 360); // grab an angle
	}	

	// allocate the semaphore used to synchronise the star animation drawing access.
	drawing_lock = create_sem(0, "star locker");

	// spawn the particle animation thread (we have better set the force quit flag to
	// false first).
	kill_my_thread = false;
	my_thread = spawn_thread(ParticleWindow::ParticleAnimation, "ParticleAnimation",
							 B_DISPLAY_PRIORITY, (void*)this);
	resume_thread(my_thread);
	
	// add a view in the background to insure that the content area will
	// be properly erased in black. This erase mechanism is not synchronised
	// with the star animaton, which means that from time to time, some
	// stars will be erreneously erased by the view redraw. But as every
	// single star is erased and redraw every frame, that graphic glitch
	// will last less than a frame, and that just in the area being redraw
	// because of a window resize, move... Which means the glitch won't
	// be noticeable. The other solution for erasing the background would
	// have been to do it on our own (which means some serious region
	// calculation and handling all color_space). Better to use the kit
	// to do it, as it gives us access to hardware acceleration...
	frame.OffsetTo(0.0, 0.0);
	view = new BView(frame, "foofoo", B_FOLLOW_ALL, B_WILL_DRAW);
	
	// The only think we want from the view mechanism is to
	// erase the background in black. Because of the way the
	// star animation is done, this erasing operation doesn't
	// need to be synchronous with the animation. That the
	// reason why we use both the direct access and the view
	// mechanism to draw in the same area of the StarWindow.
	// Such thing is usualy not recommended as synchronisation
	// is generally an issue (drawing in random order usualy
	// gives remanent incorrect result).	
	// set the view color to be black (nicer update).
	view->SetViewColor(0, 0, 0);
	AddChild(view);
	
	// Add a shortcut to switch in and out of fullscreen mode.
	AddShortcut('f', B_COMMAND_KEY, new BMessage('full'));	
	
	// As we said before, the window shouldn't get wider than 2048 in any
	// direction, so those limits will do.
	SetSizeLimits(40.0, 2000.0, 40.0, 2000.0);
	
	// If the graphic card/graphic driver we use doesn't support directwindow
	// in window mode, then we need to switch to fullscreen immediatly, or
	// the user won't see anything, as long as it doesn't used the undocumented
	// shortcut. That would be bad behavior...
	if (!BDirectWindow::SupportsWindowMode())
	{
		//be_app->PostMessage(B_QUIT_REQUESTED);
		SetFullScreen(true);
	}
}

ParticleWindow::~ParticleWindow()
{
	long		result;

	// force the drawing_thread to quit. This is the easiest way to deal
	// with potential closing problem. When it's not practical, we
	// recommand to use Hide() and Sync() to force the disconnection of
	// the direct access, and use some other flag to guarantee that your
	// drawing thread won't draw anymore. After that, you can pursue the
	// window destructor and kill your drawing thread...
	kill_my_thread = true;
	wait_for_thread(my_thread, &result);
	
	// Free window resources. As they're used by the drawing thread, we
	// need to terminate that thread before freeing them, or we could crash.
	delete_sem(drawing_lock);
	free(particle_list);
}

bool ParticleWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return(true);
}

void ParticleWindow::MessageReceived(BMessage *message)
{
	switch(message->what) {
	// Switch between full-screen mode and windowed mode.
	case 'full' :
		SetFullScreen(!IsFullScreen());
		break;
	default :
		BDirectWindow::MessageReceived(message);
		break;
	}
}

void ParticleWindow::DirectConnected(direct_buffer_info *info)
{
	//printf("direct connected\n");
	// you need to use that mask to read the buffer state.
	switch (info->buffer_state & B_DIRECT_MODE_MASK) {
	// start a direct screen connection.
	case B_DIRECT_START :
		SwitchContext(info);	// update the direct screen infos.
		release_sem(drawing_lock);	// unblock the animation thread.
		break;
	// stop a direct screen connection.
	case B_DIRECT_STOP :
		acquire_sem(drawing_lock);	// block the animation thread.
		break;
	// modify the state of a direct screen connection.
	case B_DIRECT_MODIFY :
		acquire_sem(drawing_lock);	// block the animation thread.
		SwitchContext(info);	// update the direct screen infos.
		release_sem(drawing_lock);	// unblock the animation thread.
		break;
	default :
		break;
	}
}

// This function update the internal graphic context of the StarWindow
// object to reflect the infos send through the DirectConnected API.
// It also update the state of stars (and erase some of them) to
// insure a clean transition during resize. As this function is called
// in DirectConnected, it's a bad idea to do any heavy drawing (long)
// operation. But as we only update the stars (the background will be
// updated a little later by the view system), it's not a big deal.
void ParticleWindow::SwitchContext(direct_buffer_info *info)
{
	particle		*s;
	int32			x, y, deltax, deltay;
	int32			i, j, window_area, cx, cy;
	int32			particle_count_new;
	clipping_rect	*r;

	// calculate the new star count, depending the size of the window frame.
	// we do that because we want to keep the star count proportionnal to
	// the size of the window, to keep an similar overall star density feeling
	window_area = (info->window_bounds.right-info->window_bounds.left+1)*
				  (info->window_bounds.bottom-info->window_bounds.top+1);
	// max out beyond 1M pixels.
	if (window_area > (1<<20))
		window_area = (1<<20);
	particle_count_new = (particle_count_max*(window_area>>10))>>10;
	if (particle_count_new > particle_count_max)
		particle_count_new = particle_count_max;
		
	// set the position of the new center of the window (in case of move or resize)
	cx = (info->window_bounds.right+info->window_bounds.left+1)/2;
	cy = (info->window_bounds.bottom+info->window_bounds.top+1)/2;
	
	// update to the new clipping region. The local copy is kept in 
	// screen coordinates
	clipping_bound.left = info->clip_bounds.left;
	clipping_bound.right = info->clip_bounds.right;
	clipping_bound.top = info->clip_bounds.top;
	clipping_bound.bottom = info->clip_bounds.bottom;
	// the clipping count is bounded (see comment in header file).
	clipping_list_count = info->clip_list_count;
	if (clipping_list_count > MAX_CLIPPING_RECT_COUNT)
		clipping_list_count = MAX_CLIPPING_RECT_COUNT;
	for (i=0; i<clipping_list_count; i++) {
		clipping_list[i].left = info->clip_list[i].left;
		clipping_list[i].right = info->clip_list[i].right;
		clipping_list[i].top = info->clip_list[i].top;
		clipping_list[i].bottom = info->clip_list[i].bottom;
	}
	
	// update the new rowbyte
	row_bytes = info->bytes_per_row/(info->bits_per_pixel/8);
	
	// update the screen bases (only one of the 3 will be really used).
	// this is the top corner of the screen. all my calculations are screen relative
	draw_ptr8 = (uint8*)info->bits;
	draw_ptr16 = (uint16*)info->bits;
	draw_ptr32 = (uint32*)info->bits;

	// cancel the erasing of all stars if the buffer has been reset.
	// Because of a bug in the R3 direct window protocol, B_BUFFER_RESET is not set
	// whew showing a previously hidden window. The second test is a reasonnable
	// way to work around that bug...
	if ((info->buffer_state & B_BUFFER_RESET) ||
		(need_r3_buffer_reset_work_around &&
		 ((info->buffer_state & (B_DIRECT_MODE_MASK|B_BUFFER_MOVED)) == B_DIRECT_START))) {
		s = particle_list;
		for (i=0; i<particle_count_max; i++) {
			s->last_draw = INVALID;
			s++;
		}
	}
	// in the other case, update the stars that will stay visible.
	else {
		// calculate the delta vector due to window resize or move.
		deltax = cx_old - (cx - info->window_bounds.left);
		deltay = cy_old - (cy - info->window_bounds.top);
		// check all the stars previously used.
		s = particle_list;
		for (i=0; i<particle_count; i++) {
			// if the star wasn't visible before, then no more question.
			if (s->last_draw == INVALID)
				goto not_defined;
			// convert the old position into the new referential.
			x = s->x += deltax;
			y = s->y += deltay;
			// check if the old position is still visible in the new clipping
			if ((x < clipping_bound.left) || (x > clipping_bound.right) ||
				(y < clipping_bound.top) || (y > clipping_bound.bottom))
				goto invisible;
			if (clipping_list_count == 1)
				goto visible;
			r = clipping_list;
			for (j=0; j<clipping_list_count; j++) {
				if ((x >= r->left) && (x <= r->right) &&
					(y >= r->top) && (y <= r->bottom))
					goto visible;
				r++;
			}
			goto invisible;
			
			// if it's still visible...
		visible:
			if (i >= particle_count_new) {
				// ...and the star won't be used anylonger, then we erase it. 
				if (pixel_depth == 32)
					draw_ptr32[s->last_draw] = 0;
				else if (pixel_depth == 16)
					draw_ptr16[s->last_draw] = 0;
				else
					draw_ptr8[s->last_draw] = 0;
			}
			goto not_defined;
				
			// if the star just became invisible and it was because the
			// context was modified and not fully stop, then we need to erase
			// those stars who just became invisible (or they could leave
			// artefacts in the drawing area in some cases). This problem is
			// a side effect of the interaction between a complex resizing
			// case (using 2 B_DIRECT_MODIFY per step), and the dynamic
			// star count management we are doing. In most case, you never
			// have to erase things going out of the clipping region...
		invisible:
			if ((info->buffer_state & B_DIRECT_MODE_MASK) == B_DIRECT_MODIFY) {
				if (pixel_depth == 32)
					draw_ptr32[s->last_draw] = 0;
				else if (pixel_depth == 16)
					draw_ptr16[s->last_draw] = 0;
				else
					draw_ptr8[s->last_draw] = 0;
			}
			// and set its last position as invalid.
			s->last_draw = INVALID;
		not_defined:
			s++;
		}
		
		// initialise all the new star (the ones which weren't used but
		// will be use after that context update) to set their last position
		// as invalid.
		s = particle_list+particle_count;
		for (i=particle_count; i<particle_count_new; i++) {
			s->last_draw = INVALID;
			s++;
		}
	}
	
	// update the window origin offset.
	window_offset = row_bytes*(cy-info->window_bounds.top) + (cx-info->window_bounds.left);
	
	// set the pixel_depth and the pixel data, from the color_space.
	switch (info->pixel_format) {
	case B_RGBA32 :
	case B_RGB32 :
		pixel_depth = 32;
		((uint8*)&pixel32)[0] = 0x20; 
		((uint8*)&pixel32)[1] = 0xff; 
		((uint8*)&pixel32)[2] = 0x20; 
		((uint8*)&pixel32)[3] = 0xff;
		break;
	case B_RGB16 :
		pixel_depth = 16;
		((uint8*)&pixel16)[0] = 0xe0;
		((uint8*)&pixel16)[1] = 0x07;
		break;
	case B_RGB15 :
	case B_RGBA15 :
		pixel_depth = 16;
		((uint8*)&pixel16)[0] = 0xe0;
		((uint8*)&pixel16)[1] = 0x03;
		break;
	case B_CMAP8 :
		pixel_depth = 8;
		pixel8 = 52;
		break;
	case B_RGBA32_BIG :
	case B_RGB32_BIG :
		pixel_depth = 32;
		((uint8*)&pixel32)[3] = 0x20; 
		((uint8*)&pixel32)[2] = 0xff; 
		((uint8*)&pixel32)[1] = 0x20; 
		((uint8*)&pixel32)[0] = 0xff;
		break;
	case B_RGB16_BIG :
		pixel_depth = 16;
		((uint8*)&pixel16)[1] = 0xe0;
		((uint8*)&pixel16)[0] = 0x07;
		break;
	case B_RGB15_BIG :
	case B_RGBA15_BIG :
		pixel_depth = 16;
		((uint8*)&pixel16)[1] = 0xe0;
		((uint8*)&pixel16)[0] = 0x03;
		break;
	}
	
	// set the new star count.
	particle_count = particle_count_new;
	
	// save a copy of the variables used to calculate the move of the window center
	cx_old = cx - info->window_bounds.left;
	cy_old = cy - info->window_bounds.top;
}

// This is the thread doing the star animation itself. It would be easy to
// adapt to do any other sort of pixel animation.
long ParticleWindow::ParticleAnimation(void *data)
{
	particle		*s;
	int32			x, y, cx,cy;
	uint32			i, j;
	bigtime_t		time;
	ParticleWindow	*w;
	clipping_rect	*r;
	
	// receive a pointer to the StarWindow object.
	w = (ParticleWindow*)data;
	
	
	// loop, frame after frame, until asked to quit.
	while (!w->kill_my_thread) {
	
		//printf("begin drawing loop\n");
		// we want a frame to take at least 16 ms.
		time = system_time()+ 16*1000;
		srandom(time);
		
		// get the right to do direct screen access.
		acquire_sem(w->drawing_lock);
		//printf("drawing loop: past semaphore, time to draw.\n");
		
		// grab the mouse coordinate for some sure-fire madness ;o)~
		BView *view = w->FindView("foofoo");
		if(view->LockLooper())
		{
			BPoint point;
			uint32 foofoo;
			view->GetMouse(&point, &foofoo,true);
			view->ConvertToScreen(&point);
			view->UnlockLooper();
			cx = (int32)point.x; cy = (int32)point.y;
		}
		
		// go through the array of star, for all currently used star.
		s = w->particle_list;
		for (i=0; i<w->particle_count; i++) {
			//printf("drawing particle %d\n", i);
		
			// calculate the center
			//cx = (w->clipping_bound.right+w->clipping_bound.left+1)/2;
			//cy = (w->clipping_bound.bottom+w->clipping_bound.top+1)/2;
			//printf("cx=%d, cy=%d\n", cx,cy);
			
			// move the particle			
			// find y and x
			// (s->gravity/2)*(s->timeval*s->timeval) * 1.85 is magic
			y = s->y = (int32)(cy + (int32)((s->gravity/2)*(s->timeval*s->timeval)*1.94) + ((s->init_velocity - (s->gravity*s->timeval)) * s->timeval));
			x = s->x = (int32)(cx + (int32)(s->timeval * s->cos_z_theta)); // 3d rotation
				
			// interate timeval
			s->timeval++;
				
			if(y > (w->clipping_bound.bottom-1))
			{
				//printf("particle is going to be erased and reset.\n");
				goto erase_and_reset; // reset if the particle has "fallen" far enough
			}
			
			// sanity check
			if(x < w->clipping_bound.left)
				goto erase_and_reset;
			if(x > w->clipping_bound.right)
				goto erase_and_reset;
			if(y < w->clipping_bound.top)
				goto erase; // invisible + erase last position
			
			
			// erase the previous position, if necessary
			if (s->last_draw != INVALID) {
				if (w->pixel_depth == 32)
					w->draw_ptr32[s->last_draw] = 0;
				else if (w->pixel_depth == 16)
					w->draw_ptr16[s->last_draw] = 0;
				else
					w->draw_ptr8[s->last_draw] = 0;
			}
			// check if the new position is visible in the current clipping
			if ((x < w->clipping_bound.left) || (x > w->clipping_bound.right) ||
				(y < w->clipping_bound.top) || (y > w->clipping_bound.bottom))
				goto invisible;
			
			if (w->clipping_list_count == 1) {
		visible:
				// if it's visible, then draw it.
				s->last_draw = /*w->window_offset +*/ w->row_bytes*y + x;
				//printf("drawing loop: bout to write particle. init_velocity=%f x=%d, y=%d, last_draw=%d\n",s->init_velocity,s->x,s->y,s->last_draw);
				if (w->pixel_depth == 32)
					w->draw_ptr32[s->last_draw] = w->pixel32;
				else if (w->pixel_depth == 16)
					w->draw_ptr16[s->last_draw] = w->pixel16;
				else
					w->draw_ptr8[s->last_draw] = w->pixel8;
				goto loop;
			}
			// handle complex clipping cases
			r = w->clipping_list;
			for (j=0; j<w->clipping_list_count; j++) {
				if ((x >= r->left) && (x <= r->right) &&
					(y >= r->top) && (y <= r->bottom))
					goto visible;
				r++;
			}
		erase_and_reset:
			if (w->clipping_list_count == 1) {
		erase_s:
				// erase it.
				if(s->last_draw != INVALID)
				{
					if (w->pixel_depth == 32)
						w->draw_ptr32[s->last_draw] = 0;
					else if (w->pixel_depth == 16)
						w->draw_ptr16[s->last_draw] = 0;
					else
						w->draw_ptr8[s->last_draw] = 0;
				}
				
				s->last_draw = INVALID;
				s->x = 0;
				s->y = 0;
				s->timeval = 1; // start at 1 just to keep life away from the origin
				goto loop;
			}
			// handle complex clipping cases
			r = w->clipping_list;
			for (j=0; j<w->clipping_list_count; j++) {
				if ((x >= r->left) && (x <= r->right) &&
					(y >= r->top) && (y <= r->bottom))
					goto erase_s;
				r++;
			}
		invisible:
			// if not visible, register the fact that the star wasn't draw.
			s->last_draw = INVALID;
		erase:
			// erase it.
			if(s->last_draw != INVALID)
			{
				if (w->pixel_depth == 32)
					w->draw_ptr32[s->last_draw] = 0;
				else if (w->pixel_depth == 16)
					w->draw_ptr16[s->last_draw] = 0;
				else
					w->draw_ptr8[s->last_draw] = 0;
			}
		loop:
			s++;
			//printf("end draw loop\n");
		}		
		
		// release the direct screen access
		release_sem(w->drawing_lock);
		//printf("drawing loop: frame done. semaphore released\n");
		
		// snooze for whatever time is left from the initial allocation done
		// at the beginning of the loop.
		time -= system_time();
		if (time > 0)
			snooze(time);
	}	
	return 0;
}