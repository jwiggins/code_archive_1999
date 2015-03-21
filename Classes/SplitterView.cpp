#include "SplitterView.h"

SplitterView::SplitterView(BRect frame, const char *name, int32 menu_bottom)
			: BView(frame, name, B_FOLLOW_ALL_SIDES, B_WILL_DRAW|B_FRAME_EVENTS), percentage(.5), _menu_bottom(menu_bottom)
{
	SetHighColor(216,216,216,255);
	SetViewColor(B_TRANSPARENT_32_BIT);
}

void SplitterView::Draw(BRect updateRect)
{
	// Fill with (216,216,216) grey and paint a couple dots in the middle
	BRect bounds = Bounds();
	rgb_color high_color = HighColor(), dot_color, border_color;
	dot_color.red = 160;
	dot_color.green = 160;
	dot_color.blue = 160;
	dot_color.alpha = 255;
	border_color.red = 40;
	border_color.green = 40;
	border_color.blue = 40;
	border_color.alpha = 255;

	
	// normal update
	FillRect(updateRect);
	SetHighColor(border_color);
	StrokeLine(BPoint(bounds.left,bounds.top),BPoint(bounds.left,bounds.bottom));
	StrokeLine(BPoint(bounds.right,bounds.top),BPoint(bounds.right,bounds.bottom));
	SetHighColor(high_color);

	// draw the dots if they need it
	if(updateRect.Intersects(dot_rect_one))
	{
		FillRect(dot_rect_one);
		SetHighColor(dot_color);
		StrokeArc(dot_rect_one, 0, 360);
		SetHighColor(high_color);
	}
	if(updateRect.Intersects(dot_rect_two))
	{
		FillRect(dot_rect_two);
		SetHighColor(dot_color);
		StrokeArc(dot_rect_two, 0, 360);
		SetHighColor(high_color);
	}
	Flush();
}

void SplitterView::FrameResized(float width, float height)
{
	height-=_menu_bottom;
	dot_rect_one.Set(1, (height/2) - 5, width-1, (height/2) - 2);
	dot_rect_two.Set(1, (height/2) + 2, width-1, (height/2) + 5);
}

void SplitterView::FrameMoved(BPoint point)
{
	float height = (Bounds().Height() - _menu_bottom);
	ConvertFromParent(&point);
	dot_rect_one.Set(point.x + 1, (height/2) - 5, point.x + 5, (height/2) - 2);
	dot_rect_two.Set(point.x + 1, (height/2) + 2, point.x + 5, (height/2) + 5);
}

void SplitterView::MouseDown(BPoint)
{	
	resume_thread(spawn_thread(thread_start, "mousewatcher thread", B_NORMAL_PRIORITY, (void *)this));
}

int32 SplitterView::thread_start(void *arg)
{
	return ((SplitterView *)arg)->MouseWatcher();
}

int32 SplitterView::MouseWatcher()
{
	BWindow *window = Window();
	BRect	window_bounds;
	BPoint	mouse_pos, previous;
	uint32	buttons=1;
	
	if(window->Lock())
	{
		window_bounds = window->Bounds();
		GetMouse(&mouse_pos, &buttons, true);
		ConvertToParent(&mouse_pos);
		Window()->Unlock();
		previous = mouse_pos;
	}
	
	while (buttons)
	{
		if(mouse_pos.x != previous.x)
		{
			// we have movement
			if((mouse_pos.x < (window_bounds.right - 20)) && (mouse_pos.x > (window_bounds.left + B_V_SCROLL_BAR_WIDTH)))
			{
				// didnt move past our bounds
				// report to the window
				BMessage msg(SPLITTER_MOVED);
				if(msg.AddFloat("bar position", mouse_pos.x) == B_OK)
					window->PostMessage(&msg);
			}
		}
		snooze(75 * 1000);
		if(window->Lock())
		{
			previous = mouse_pos;
			GetMouse(&mouse_pos, &buttons, true);
			ConvertToParent(&mouse_pos);
			Window()->Unlock();
		}
	}
	
	return B_OK;
}