/** 
 * @file bn_init.c
 * @brief Big number intitialization functions
 * @author Zambo-dev
 * @date 22/12/2021
 * @copyright Zambo-dev - 2022 | licensed under MIT
*/

#include "../include/bntl.h"


/**
 *	@brief Initialize a number setting the value at zero
 *	@param [in,out] Number Allocated number
 *	@param [in] Size Size in bytes
*/
size_t bn_init(bn_t *number, size_t size)
{
	size = (size % sizeof(ulong) == 0) ? size/sizeof(ulong) : size/sizeof(ulong)+1;
	number->size = size * sizeof(ulong);
	number->num = (size != 0) ? (ulong *)calloc(size, sizeof(ulong)) : NULL;

	return number->size;
}

/**
 *	@brief Initialize a number setting the value
 *	@param [in,out] Number Allocated number
 *	@param [in] Number Value
*/
size_t bn_init_n(bn_t *number, ulong value)
{
	number->size = sizeof(ulong);
	number->num = (ulong *)calloc(1, sizeof(ulong));
	*(number->num) |= value;

	return number->size;
}

/**
 * 	@brief Initialize number from a binary number string
 *	@param [in,out] Number Allocated number
 *	@param [in] String Char* number
*/
size_t bn_init_s(bn_t *number, char* string)
{
	size_t length = strlen(string);
	bn_init(number, length / 8);

	ulong tmp = 1;	
	byte addval;
	ubyte *res;

	if(*(ubyte *)&tmp == 1)		/* Little endian */
	{
		addval = 1;
		res = (ubyte *)number->num;
	}
	else						/* Big endian */
	{
		addval = -1;
		res = (ubyte *)number->num + number->size - 1;
	}

	char *str = string;
	ubyte idx = 7;

	while(length > 0)
	{
		*res |= (*str - ASCII_ZERO) << idx;

		--idx;
		--length;
		++str;

		if((length) % 8 == 0)
		{	
			res += addval;
			idx = 7;
		}
	}

	return number->size;
}