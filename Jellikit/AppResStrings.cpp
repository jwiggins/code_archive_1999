#include "AppResStrings.h"

AppResStrings::AppResStrings()
{
	BResourceStrings res_strings;
	StringList = new BList(STRING_END);
	const char *string_ptr = NULL;
	char *a_string = NULL;
	
	const char *default_string_table[STRING_END+1] =
	{
		"OK",
		"Cancel",
		"File",
		"New File",
		"Open File" B_UTF8_ELLIPSIS,
		"Close",
		"Save",
		"Save As" B_UTF8_ELLIPSIS,
		"About",
		"Quit",
		"Edit",
		"Add",
		"Remove",
		"Untitled",
		"Type",
		"string",
		"int64",
		"int32",
		"double",
		"float",
		"rect",
		"bool",
		"raw",
		"An attribute with the same name already exists\nfor this file. Would you like to replace it\nwith the one you are moving?\n\n",
		"Version",
		"The %s manager failed to initialize correctly.\nThat pretty much makes this program non-functional.\nI will quit now.",
		"Attribute",
		"add-on",
		"Name",
		"mime string",
		"Predefined",
		"other",
		NULL
	};
	
	for(int32 i=0;i<STRING_END;i++)
	{
		//printf("String %d: %s\n", i, res_strings.FindString(i));
		string_ptr = res_strings.FindString(i);
		if(string_ptr != NULL)
		{
			a_string = (char *)malloc(strlen(string_ptr) + 1);
			strcpy(a_string, string_ptr);
			StringList->AddItem((void *)a_string, i);
		}
		else
			StringList->AddItem((void *)default_string_table[i], i);
		
		a_string = NULL;
	}
}

AppResStrings::~AppResStrings()
{
	void *anItem;
	for(int32 i=0; (anItem = StringList->ItemAt(i)); i++)
		free(anItem);
	delete StringList;
}