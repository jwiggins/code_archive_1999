#ifndef STUB_VIEW_H
#define STUB_VIEW_H

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
private:
bool on_off;
BMessageRunner *pulsar;
};

StubView::StubView(BRect frame)
: BView(frame, "StubView", B_FOLLOW_ALL, B_WILL_DRAW|B_PULSE_NEEDED|B_FRAME_EVENTS),on_off(true)
{
	SetViewColor(0,0,128,255);SetLowColor(0,0,128,255);SetHighColor(255,255,255,255);
	pulsar = NULL;
	frame.top = frame.bottom - 7.;
	frame.right = frame.left + 7.;
	AddChild(new BDragger(frame, this, B_FOLLOW_LEFT|B_FOLLOW_BOTTOM));
}

StubView::StubView(BMessage *msg)
: BView(msg),on_off(true)
{
	//printf("BMessage ctor\n");
	BRect frame = Bounds();
	SetViewColor(0,0,128,255);SetLowColor(0,0,128,255);SetHighColor(255,255,255,255);
	SetResizingMode(B_FOLLOW_NONE);
	pulsar = NULL;
	frame.top = frame.bottom - 7.;
	frame.right = frame.left + 7.;
	AddChild(new BDragger(frame, this, B_FOLLOW_LEFT|B_FOLLOW_BOTTOM));
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
}

void StubView::FrameResized(float , float new_height)
{
	SetFontSize(new_height/4.0);
	Invalidate();
}

#endif