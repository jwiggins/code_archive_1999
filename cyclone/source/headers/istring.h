#pragma once

#include <bstring.h>


template <class charT>
class i_char_traits { };


null_template
class i_char_traits<char>
{
public:

    typedef char         char_type;
    typedef int          int_type;
    typedef streampos    pos_type;
    typedef streamoff    off_type;
    typedef fstate_t     state_type;

    // constraint member functions
    
    static void
    assign (char_type& c1, const char_type& c2) { c1 = c2; }

    static bool
    eq (const char_type& c1, const char_type& c2)
    {
    	return ( tolower(c1) == tolower(c2) );
    }

    static bool
    lt (const char_type& c1, const char_type& c2)
    {
    	return ( tolower(c1) < tolower(c2) );
    }

    //
    // speed-up functions
    //
    inline
    static int
    compare (const char_type* s1, const char_type* s2, size_t n) 
    {
        return strncasecmp( s1, s2, n );
    }
    
	static size_t
	length (const char_type* s) 
    {
        return ::strlen (s);
    }
    
	static char_type*
	copy (char_type* s1, const char_type* s2, size_t n) 
    {
        return (char_type*)::memcpy (s1, s2, n);
    }
    
	static const char_type*
	find (const char_type* s, int n, const char_type& a)
    {
        return (const char_type*)::memchr (s, a, n);
    }
    
	static char_type*
	move (char_type* s1, const char_type* s2, size_t n)
    {
        return (char_type*)::memmove(s1, s2, n);
        char_type* s = s1;
        char_type* tempstr = new char_type[n];
        copy (tempstr, s2, n);
        for (size_t i = 0; i < n; ++i, ++s1)
            assign (*s1, tempstr[i]);
        delete [] tempstr;
        return s;
    }
    
	static char_type*
	assign (char_type* s, size_t n, char_type a)
 	{
        return (char_type*)::memset (s, a, n);
	}   
 
    static int_type 
    not_eof (const int_type& c)
    {
        if (!eq (c, eof ()))
            return c;
        return ~c;
    }
    
    static char_type
    to_char_type (const int_type& c)
    {
        if (eq_int_type (c, to_int_type (c)))   
            return c;
        return char_type(0);
    }
    
    static int_type
    to_int_type (const char_type& c)
    {
#ifdef MSIPL_UCHAR
        return ((c+1) & 0xFF) - 1;
#else
        return (unsigned char)c;
#endif
    }
    
    static bool
    eq_int_type (const int_type& c1, const int_type& c2)
    {
        return (c1 == c2);
    }
        
    static state_type
	get_state (streampos pos)
	{
	    return pos.state();
	}

	static pos_type
	get_pos (pos_type pos, state_type state)
	{
	    return pos_type(pos.offset (), state);
	}

    static int_type
    eof ()
    {
        return EOF;
    }
    
    static char_type
    eos ()
    {
        return 0;
    }
    
    static char_type
    newline ()
    {
        return '\n';
    }
};


typedef basic_string< char, i_char_traits<char>, allocator<char> > istring;
