#include <Application.h>
#include <Window.h>
#include <View.h>
#include <Dragger.h>
#include <MessageRunner.h>
#include <stdio.h>


#define APP_SIG "application/x-vnd.prok-buynowreplicant"

class _EXPORT StubView : public BView {
public:
_EXPORT	StubView(BRect frame);
_EXPORT	StubView(BMessage *msg);
_EXPORT	~StubView();

_EXPORT	virtual status_t Archive(BMessage *msg, bool deep) const;
_EXPORT static StubView * Instantiate(BMessage *msg);

_EXPORT	virtual void AttachedToWindow(void);
_EXPORT	virtual void Draw(BRect);
_EXPORT	virtual void Pulse(void);
_EXPORT	virtual	void FrameResized(float , float new_height);
_EXPORT	virtual	void MouseDown(BPoint p);
_EXPORT	virtual	void MouseUp(BPoint);
_EXPORT	virtual void MouseMoved(BPoint p, uint32 nTransit, const BMessage *);

enum dragger_type {
	DRAGGER_NO,
	DRAGGER_YES
};

_EXPORT void updateDragHilight(dragger_type t);
_EXPORT dragger_type dragType(BPoint p);

private:
bool on_off, is_replicant, mouse_is_down, we_be_dragging;
BMessageRunner *pulsar;
BPoint cache_point;
dragger_type drag_hilight;
};

class StubWindow : public BWindow {
public:
		StubWindow(void)
		: BWindow(BRect(100,100,149,149), "Stub", B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, 0)
		{
			SetPulseRate(1000 * 750); AddChild(new StubView(Bounds()));
		}
		
virtual bool QuitRequested() { be_app->PostMessage(B_QUIT_REQUESTED); return true; }
virtual void MessageReceived(BMessage *msg) { BWindow::MessageReceived(msg);}
};

class StubApp : public BApplication {
public:
		StubApp(const char *sig) : BApplication(sig)
		{
			(new StubWindow())->Show();
		};
};

StubView::StubView(BRect frame)
: BView(frame, "StubView", B_FOLLOW_ALL, B_WILL_DRAW|B_PULSE_NEEDED|B_FRAME_EVENTS),on_off(true),is_replicant(false),we_be_dragging(false), mouse_is_down(false)
{
	SetViewColor(0,0,128,255);SetLowColor(0,0,128,255);SetHighColor(255,255,255,255);
	pulsar = NULL;
	frame.top = frame.bottom - 7.;
	frame.right = frame.left + 7.;
	AddChild(new BDragger(frame, this, B_FOLLOW_LEFT|B_FOLLOW_BOTTOM));
}

StubView::StubView(BMessage *msg)
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

StubView::~StubView()
{
	if(pulsar)
		delete pulsar;
}

status_t StubView::Archive(BMessage *msg, bool deep) const
{	
	msg->AddString("class", "StubView");
  msg->AddString("add_on", APP_SIG);
	BView::Archive(msg,deep);
	//msg->PrintToStream();
	return B_OK;
}

StubView * StubView::Instantiate(BMessage *msg)
{
	//printf("Instantiate()\n");
	if(validate_instantiation(msg, "StubView")) 
		return (new StubView(msg));
	return ((StubView *)NULL);
}

void StubView::AttachedToWindow(void)
{
	if(!pulsar)
		pulsar = new BMessageRunner(BMessenger(this), new BMessage(B_PULSE), (1000*750));
}

void StubView::Pulse(void)
{
	on_off = !on_off;
	Invalidate();
}

void StubView::Draw(BRect)
{
	BFont font;	GetFont(&font);
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
		FillRect(BRect(corner,other), B_SOLID_LOW);
		
//		other.x -= 10.;
//		StrokeLine(other, corner);
//		other = corner; other.y -= 10.;
//		StrokeLine(corner, other);
	}
}

void StubView::FrameResized(float , float new_height)
{
	SetFontSize(new_height/4.0);
	Invalidate();
}

void StubView::MouseDown(BPoint p)
{
	SetMouseEventMask(B_POINTER_EVENTS);
	mouse_is_down = true;
	
	ConvertToScreen(&p);
	cache_point = p;
}

void StubView::MouseUp(BPoint)
{
	mouse_is_down = false;
	we_be_dragging = false;
}

void StubView::MouseMoved(BPoint p, uint32 nTransit, const BMessage *)
{
	if(is_replicant)
	{
		dragger_type t = dragType(p);
		
		if(!we_be_dragging && nTransit != B_EXITED_VIEW && t == DRAGGER_YES)
			updateDragHilight(DRAGGER_YES);
		
		if(we_be_dragging)
		{
			ConvertToScreen(&p);
			ResizeBy(p.x-cache_point.x, p.y-cache_point.y);
			cache_point = p;
		}
		else if(mouse_is_down && (t == DRAGGER_YES))
		{
			we_be_dragging = true;
			ConvertToScreen(&p);
			ResizeBy(p.x-cache_point.x, p.y-cache_point.y);
			cache_point = p;
		}
	}
}

void StubView::updateDragHilight(StubView::dragger_type t)
{
	drag_hilight = t;
	Invalidate();
}

StubView::dragger_type StubView::dragType(BPoint p)
{
	// p is still view relative at this point
	BRect b = Bounds();
	float b_width = b.Width(), b_height = b.Height();
	
	if((b_width - 10.) < p.x && p.x < b_width
			&& (b_height - 10.) < p.y && p.y < b_height)
			return DRAGGER_YES;
	else
		return DRAGGER_NO;
}

int main(void)
{
	StubApp app(APP_SIG);
	
	app.Run();
	return B_OK;
}