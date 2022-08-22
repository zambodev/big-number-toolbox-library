/**
 * @file bn_conv.c
 * @brief Convert big numbers to and from string
 * @author Zambo-dev
 * @date 22/12/2021
 * @copyright Zambo-dev - 2022 | licensed under MIT
*/


#include "../include/bntl.h"


/**
 *	@brief Convert a bn_t number into a string
 *	@param [in] number Initialized number
 *	@return char* string
*/
char* bntobs(bn_t *number)
{	
	char* result = (char*) malloc(number->size*8+1);
	byte* res = (byte*)result;
	byte* num = number->num;
	byte tmp;
	byte i;
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
		
			i++;
			res--;
		}
		
		res += i;

		num++;
		size--;
	}

	*res = '\0';
	
	return result;
}

