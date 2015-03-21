/* This is just a cheesy test app to figure out some ascii related foo */

#include <stdio.h>

#define transform(c) ((((c >= 65) && (c <= 90))) ? c : \
					(((c > 96) && (c < 123)) ? c - 32 : \
					(((c >= 32) && (c <= 64)) ? c : ' ')))

int main(void)
{
	const char foo[] = " !\"#$\%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	char target[256];
	int i;
	//for(i = 32; i <= 128; i++ )
	//{
	//	printf("%c", i);
	//}
	//printf("\n");
	//for(i = 65; i <= 90; i++ )
	//{
	//	printf("%c", i);
	//}
	printf("\n");
	for(i = 0; i < 96; i++ )
	{
		target[i] = transform(foo[i]);
	}	
	printf("foo : %s\n", foo);
	printf("target : %s\n", target);
	printf("(space) = %d\n", ' ');
	return 0;
}

/*
	Glyphs supported in Glyph.h

	 !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ  (32 - 90)
	 
	 glyphlist[transform(character) - 32]
*/