#include <stdio.h>
#include <unistd.h>
int main(int argc, const char *argv[])
{
	printf("encrypted password = %s\n", crypt(argv[1], argv[2]));
	return 0;
}