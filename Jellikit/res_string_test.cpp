#include <stdio.h>
#include <Application.h>
#include <ResourceStrings.h>
#include <malloc.h>
#include "ResStringDefs.h"

int main(void)
{
	BApplication app("application/x-vnd.prok-res-test");
	BResourceStrings *res_strings = new BResourceStrings();
	
	for(int i=0;i<STRING_END;i++)
		printf("String %d: %s\n", i, res_strings->FindString(i));
	return B_OK;
}