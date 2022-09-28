#include "../include/bntl.h"


/* Convert a number into a string */
char* bntobs(bn_t *number)
{	
	char* result = (char*) malloc(number->size*8+1);
	ubyte *res = (ubyte *)result;
	ubyte *num = (ubyte *)number->num;
	ubyte tmp;
	ubyte i;
	size_t size = number->size;

	memset(result, ASCII_ZERO, size*8+1);

	while(size > 0)
	{
		i = 1;
		res += 7;
		tmp = *num;

		while(tmp > 0)
		{
			*res = (tmp % 2) + ASCII_ZERO;
			tmp >>= 1;
		
			++i;
			--res;
		}
		
		res += i;

		++num;
		--size;
	}

	*res = '\0';
	
	return result;
}

