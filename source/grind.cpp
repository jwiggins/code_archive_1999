#include <OS.h>
#include <Application.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

struct pointer {
        struct pointer  *p;
};
struct pointer futile = { &futile };
uint z;

void grind( uint n)
{
	struct pointer *p = &futile;
    for (uint i=0; i<n; ++i)
    {
		p = p->p; p = p->p; p = p->p; p = p->p;
		p = p->p; p = p->p; p = p->p; p = p->p;
		p = p->p; p = p->p; p = p->p; p = p->p;
		p = p->p; p = p->p; p = p->p; p = p->p;
		p = p->p; p = p->p; p = p->p; p = p->p;
		p = p->p; p = p->p; p = p->p; p = p->p;
		p = p->p; p = p->p; p = p->p; p = p->p;
		p = p->p; p = p->p; p = p->p; p = p->p;
    }
	z += p == 0;
}

bigtime_t       gtime;
uint            gcount;

struct L: BLooper {
        L( ) {
                n = 0;
                Run( );
                PostMessage( 'g');
        }
        void MessageReceived( BMessage *m) {
                set_thread_priority( find_thread( 0), B_LOW_PRIORITY/5);
                while (TRUE) {
                        grind( gcount);
                        ++n;
                }
        };
        uint    n;
};
struct C: BView {
	C( BRect r): BView( r, 0, B_FOLLOW_ALL, B_WILL_DRAW|B_PULSE_NEEDED)
	{
		lastt = system_time( );
		memset( lastn, 0, sizeof lastn);
		system_info si;
		get_system_info( &si);
		for (lcount=0; lcount<si.cpu_count; ++lcount)
			ltab[lcount] = new L( );
	}
	void Pulse( )
	{
		bigtime_t t = system_time( );
		float f = 0;
		for (uint i=0; i<lcount; ++i)
		{
			f += 100. * (ltab[i]->n-lastn[i]) /((float)(t-lastt)/gtime);
			lastn[i] = ltab[i]->n;
		}
		lastt = t;
		f /= lcount;
		graph( f);
	}
	virtual void graph( float) = 0;
	bigtime_t	lastt;
	uint		lastn[20],lcount;
	L			*ltab[20];
};
struct V0: C {
	V0( BRect r): C( r) { }
	void graph( float f)
	{
		printf( "%f\n", f);
	}
};
struct V1: C {
	V1( BRect r): C( r)
	{
		x = r.right;
	}
	void graph( float f)
	{
		ScrollBy( 1, 0);
		StrokeLine( BPoint( x, 100), BPoint( x, 100-(f+.5)));
		++x;
	}
	uint    x;
};
struct V2: C {
	V2( BRect r): C( r)
	{
		memset( rec, 0, sizeof rec);
		nrec = 0;
	}
	void graph( float f)
	{
		if (nrec == 0)
			rec[nrec] = f;
		else
			rec[nrec] = f + rec[nrec-1];
		++nrec;
		for (uint i=0; ; ++i)
		{
			uint j = 1 << i;
			if (j >= nrec)
				break;
			float y = (rec[nrec-1]-rec[nrec-1-j]) / j;
			FillRect( BRect( i*8, 100-y, i*8+6, 100));
			SetHighColor( 255, 255, 255);
			FillRect( BRect( i*8, 0, i*8+6, 100-1-y));
			SetHighColor( 0, 0, 0);
		}
	}
	void Draw( BRect)
    {
		SetHighColor( 255, 0, 0);
		for (uint x=0; x<100; x+=20)
			FillRect( BRect( 0, x, 100, x+10-1));
	}
	float   rec[33000];
	uint    nrec;
};
struct W: BWindow
{
	W( BRect r): BWindow( r, "cpuload", B_TITLED_WINDOW, B_NOT_ZOOMABLE)
	{
		AddChild( new V1( r));
		MoveBy( 100, 100);
		SetPulseRate( 1000000);
		Show( );
	}
	bool QuitRequested( )
	{
		be_app->PostMessage( 'q');
		return (TRUE);
	}
};

struct A: BApplication {
	A( ): BApplication( "application/x-cpuload")
	{
		Run( );
	}
	void ReadyToRun( )
	{
		calibrate( );
		new W( BRect( 0, 0, 100, 100));
	}
	void calibrate( )
	{
		gcount = 10;
		while (TRUE)
		{
			gtime = 0;
			for (uint i=0; i<100; ++i)
			{
				bigtime_t t0 = system_time( );
				grind( gcount);
				bigtime_t t = system_time( ) - t0;
				if (gtime==0 || t<gtime)
					gtime = t;
			}
			if (gtime > 800)
				break;
			gcount *= 1.1;
		}
		printf("gtime=%Ld gcount=%d MHz=%f\n", gtime, gcount,gcount*32.*3/gtime);
	}
	void MessageReceived( BMessage *m)
	{
		if (m->what == 'q')
			Quit( );
	}
};
main()
{
	A a;
	return (z);
}