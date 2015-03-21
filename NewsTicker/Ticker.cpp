#include "Ticker.h"
#include "Glyph.h"

Ticker::Ticker(BRect frame)
: BView(frame, "Ticker", B_FOLLOW_ALL, B_PULSE_NEEDED)
{
	strcpy(buffer, "abcdefghijklmnopqrst");
}

void Ticker::AttachedToWindow()
{
	FillRect(Bounds(), B_SOLID_HIGH); // high color starts out as black
	Flush();
	SetLowColor(32,32,32,255); // dark grey - off
	SetHighColor(0,128,0,255); // darkish green - on
	for(int i=0;i<20;i++) // maj_column
		for(int j=0;j<5;j++) // min_column
			for(int k=0;k<6;k++) // row
				draw_dot(i,j,k,0/* no shift */, i % 2);
	Flush();
}

void	Ticker::MessageReceived(BMessage *msg)
{
	printf("tock\n");
	switch(msg->what)
	{
		case TICKER_PULSE:
		{
			quasi_pulse();
			printf("tick\n");
			break;
		}
		default:
		{
			BView::MessageReceived(msg);
			break;
		}
	}
}

void Ticker::Pulse()
{
	quasi_pulse();
}

#define transform(c) ((((c >= 65) && (c <= 90))) ? c : \
					(((c > 96) && (c < 123)) ? c - 32 : \
					(((c >= 32) && (c <= 64)) ? c : ' ')))

void	Ticker::quasi_pulse()
{
	static int shift=0;
	
	for(int i=0;i<20;i++) // maj_column
		for(int j=0;j<5;j++) // min_column
			for(int k=0;k<6;k++) // row
				draw_dot(i,j,k,0, ((glyphlist[transform(buffer[i])-32].bits[((k*5)+j)/8]) & ((0x80)>>(((k*5)+j) % 8))));
	Flush();
	
	if(shift == 4)
		shift = 0;
	else
		shift++;
}