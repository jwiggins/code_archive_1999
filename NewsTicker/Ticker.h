#ifndef TICKER_H
#define TICKER_H

#include <View.h>
#include <string.h>
#include <stdio.h>

#include "MessageDefs.h"

#define LETTERWIDTH 5
#define LETTERHEIGHT 6
#define LETTERSPERROW 20
#define ROWLENGTH (LETTERSPERROW*LETTERWIDTH)

class Ticker : public BView {
public:
					Ticker(BRect frame);
virtual void			AttachedToWindow();
virtual void			MessageReceived(BMessage *msg);
virtual void			Pulse();
void					quasi_pulse();
private:
inline void			draw_dot(int , int , int , int , int);
char					buffer[21];
};

inline void Ticker::draw_dot(int maj_column, int min_column, int row, int shift, int value)
{

	// maj_column starts at 0 and goes to 19
	// min_column starts at 0 and goes to 4
	// row starts at 0 and goes to 5
	// shift oscilates between 0 and 4
	// value is either a 1 or a 0
	
	int place;
	BRect rect(0,0,3,3);
	place = ((row * ROWLENGTH)+(maj_column * LETTERWIDTH)+min_column) - shift;
	if((place >= 0) && (place <= (ROWLENGTH*LETTERHEIGHT)))
	{
		// draw it!
		rect.OffsetTo(((maj_column*LETTERWIDTH*5.0)+(min_column*5.0)-(shift*5.0)), (row*5.0)+5.0);
		if(value)
			FillEllipse(rect);
		else
			FillEllipse(rect, B_SOLID_LOW);
	}
}

#endif