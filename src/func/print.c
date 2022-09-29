#include "../include/bntl.h"


/* Print a number into stdout formatting in (rows of 8, space seprarated, bytes) */
void bn_print(bn_t *number)
{
	ulong tmp = 1;
	if(*(ubyte *)&tmp == 1)		/* Little endian */
		printf("LE ");
	else
		printf("BE ");

	char* string = bntobs(number);
	for(int i=0; i < strlen(string); i++)
	{
		printf("%c", string[i]);
		if( ((i+1) % 8) == 0)
			printf(" ");
		if( ((i+1) % 64) == 0)
			printf("\n   ");
	}
	printf("\n");

	fflush(stdout);
}
