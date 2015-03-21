#include <Application.h>
#include <Window.h>
#include <View.h>
#include <Dragger.h>
#include <MessageRunner.h>
#include <stdio.h>


#define APP_SIG "application/x-vnd.prok-buynowreplicant"

enum dragger_type {
	DRAGGER_NO,
	DRAGGER_YES
};

/*********************** BNView ****************************/
class _EXPORT BNView : public BView {
public:
_EXPORT BNView(BRect frame);
_EXPORT BNView(BMessage *msg);
_EXPORT ~BNView();

_EXPORT virtual status_t Archive(BMessage *msg, bool deep) const;
_EXPORT static BNView * Instantiate(BMessage *msg);

_EXPORT virtual void AttachedToWindow(void);
_EXPORT virtual void Draw(BRect);
_EXPORT virtual void Pulse(void);
_EXPORT virtual void FrameResized(float , float new_height);
_EXPORT virtual void MouseDown(BPoint p);
_EXPORT virtual void MouseUp(BPoint);
_EXPORT virtual void MouseMoved(BPoint p, uint32 nTransit, const BMessage *);

_EXPORT void updateDragHilight(dragger_type t);
_EXPORT dragger_type dragType(BPoint p);

private:
bool on_off, is_replicant, mouse_is_down, we_be_dragging;
BMessageRunner *pulsar;
BPoint drag_cache_point, mouse_down_offset;
dragger_type drag_hilight;
};

/*********************** BNWindow ****************************/
class BNWindow : public BWindow {
public:
	BNWindow(void);

virtual bool QuitRequested();
virtual void MessageReceived(BMessage *msg);
};

/*********************** BNApp ****************************/
class BNApp : public BApplication {
public:
	BNApp(const char *sig);
};

/*********************** BNView ****************************/

BNView::BNView(BRect frame)
: BView(frame, "BNView", B_FOLLOW_ALL, B_WILL_DRAW|B_PULSE_NEEDED|B_FRAME_EVENTS),on_off(true),is_replicant(false),we_be_dragging(false), mouse_is_down(false)
{
	SetViewColor(0,0,128,255);SetLowColor(0,0,128,255);SetHighColor(255,255,255,255);
	pulsar = NULL;
	frame.top = frame.bottom - 7.;
	frame.right = frame.left + 7.;
	AddChild(new BDragger(frame, this, B_FOLLOW_LEFT|B_FOLLOW_BOTTOM));
}

BNView::BNView(BMessage *msg)
: BView(msg),on_off(true),is_replicant(true),we_be_dragging(false),mouse_is_down(false)
{
	//printf("BMessage ctor\n");
	BRect frame = Bounds();
	SetViewColor(0,0,128,255);SetLowColor(0,0,128,255);SetHighColor(255,255,255,255);
	SetResizingMode(B_FOLLOW_NONE);
	pulsar = NULL;
	frame.top = frame.bottom - 7.;
	frame.right = frame.left + 7.;
	AddChild(new BDragger(frame, this, B_FOLLOW_LEFT|B_FOLLOW_BOTTOM));
	
	drag_hilight = DRAGGER_NO;
}

BNView::~BNView()
{
	if(pulsar)
		delete pulsar;
}

status_t BNView::Archive(BMessage *msg, bool deep) const
{
	msg->AddString("class", "BNView");
	msg->AddString("add_on", APP_SIG);
	
	BView::Archive(msg,deep);
	//msg->PrintToStream();
	return B_OK;
}

BNView * BNView::Instantiate(BMessage *msg)
{
	//printf("Instantiate()\n");
	if(validate_instantiation(msg, "BNView"))
		return (new BNView(msg));
	return ((BNView *)NULL);
}

void BNView::AttachedToWindow(void)
{
	if(!pulsar)
		pulsar = new BMessageRunner(BMessenger(this), new BMessage(B_PULSE), (1000*750));
}

void BNView::Pulse(void)
{
	on_off = !on_off;
	Invalidate();
}

void BNView::Draw(BRect)
{
	BFont font;     GetFont(&font);
	float x_coord = (Bounds().Width() - StringWidth("Buy Now"))/2.;
	float y_coord = (Bounds().Height()/2.) + (font.Size()/2.);
	
	if(on_off)
		DrawString("Buy Now", BPoint(x_coord, y_coord));
	else
		FillRect(Bounds(), B_SOLID_LOW);
	
	if(drag_hilight == DRAGGER_YES)
	{
		BPoint corner = Bounds().RightBottom(), other;
		
		other = corner;
		other.x -= 10.; other.y -= 10.;
		
		SetLowColor(255,0,0,255);
		//BRect(corner,other)
		FillRect(BRect(other,corner), B_SOLID_LOW);
		
		//other.x -= 10.;
		//StrokeLine(other, corner);
		//other = corner; other.y -= 10.;
		//StrokeLine(corner, other);
	}
}

void BNView::FrameResized(float , float new_height)
{
	SetFontSize(new_height/4.0);
	Invalidate();
}

void BNView::MouseDown(BPoint p)
{
	SetMouseEventMask(B_POINTER_EVENTS);
	mouse_is_down = true;
	
	mouse_down_offset = p - Bounds().RightBottom();
	
	ConvertToScreen(&p);
	drag_cache_point = p;
}

void BNView::MouseUp(BPoint)
{
	mouse_is_down = false;
	we_be_dragging = false;
	updateDragHilight(DRAGGER_NO);
}

void BNView::MouseMoved(BPoint p, uint32 nTransit, const BMessage *)
{
	if(is_replicant)
	{
		dragger_type t = dragType(p);
		BRect b(Bounds());
		float b_width = b.Width(), b_height = b.Height();
		
		if(t != drag_hilight)
			updateDragHilight(t);
		
		if(we_be_dragging || (mouse_is_down && (t == DRAGGER_YES)))
		{
			float width_delta, height_delta;
			bool close_to_corner;
					
			we_be_dragging = true;
			
			ConvertToScreen(&p);
			width_delta = p.x - drag_cache_point.x;
			height_delta = p.y - drag_cache_point.y;
			//printf("width_delta = %5.2f, height_delta = %5.2f\n",width_delta,height_delta);
			
			ConvertToScreen(&b);
			//b.PrintToStream();
			close_to_corner = (((b.right + mouse_down_offset.x) < p.x)
												|| ((b.bottom + mouse_down_offset.x) < p.y));
			
			if(close_to_corner)
			{
				if(b_width > 20. && b_height > 20. || width_delta > 0. || height_delta > 0.)
					ResizeBy(width_delta, height_delta);
				else if(b_width > 20. || width_delta > 0.)
					ResizeBy(width_delta,0);
				else if(b_height > 20. || height_delta > 0.)
					ResizeBy(0,height_delta);
			}
			drag_cache_point = p;
		}
	}
}

void BNView::updateDragHilight(dragger_type t)
{
	drag_hilight = t;
	Invalidate();
}

dragger_type BNView::dragType(BPoint p)
{
	// p is still view relative at this point
	BRect b = Bounds();
	float b_width = b.Width(), b_height = b.Height();
	
	if((b_width - 10.) < p.x && p.x < b_width
		&& (b_height - 10.) < p.y && p.y < b_height)
		return DRAGGER_YES;
	else
		return DRAGGER_NO;
	
	return DRAGGER_NO;
}

/*********************** BNWindow ****************************/

BNWindow::BNWindow(void)
: BWindow(BRect(100,100,149,149), "Stub", B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, 0)
{
	SetPulseRate(1000 * 750);
	AddChild(new BNView(Bounds()));
}

bool BNWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
void BNWindow::MessageReceived(BMessage *msg)
{
	BWindow::MessageReceived(msg);
}

/*********************** BNApp ****************************/

BNApp::BNApp(const char *sig)
: BApplication(sig)
{
	(new BNWindow())->Show();
}

/*********************** main ****************************/
int main(void)
{
	BNApp app(APP_SIG);
	
	app.Run();
	return B_OK;
}