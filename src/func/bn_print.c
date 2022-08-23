/** 
 * @file bn_print.c
 * @brief Print big number value
 * @author Zambo-dev
 * @date 22/12/2021
 * @copyright Zambo-dev - 2022 | licensed under MIT
*/


#include "../include/bntl.h"


/**
 * 	@brief Print a bn_t number into stdout formatting in (rows of 8, space seprarated, bytes)
 *	@param [in,out] number Initialized number
*/
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
