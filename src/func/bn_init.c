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
size_t bn_init_s(bn_t *number, char *string)
{
	size_t length = strlen(string);
	bn_init(number, (length-1) / 8);

	ulong tmp = 1;	
	byte numaddval, straddval;
	ubyte *res;
	char *str;
	ubyte shift;

	if(*string == 'L')			/* Little endian */
	{
		str = string+1;
		shift = 7;
		straddval = 1;
	}
	else if(*string == 'B')		/* Big endian */
	{
		str = string+length-1;
		shift = 0;
		straddval = -1;
	}
	else
		return 0;

	if(*(ubyte *)&tmp == 1)		/* Little endian */
	{
		numaddval = 1;
		res = (ubyte *)number->num;
	}
	else						/* Big endian */
	{
		numaddval = -1;
		res = (ubyte *)number->num + number->size - 1;
	}

	--length;
	while(length > 0)
	{
		*res |= (*str - ASCII_ZERO) << shift;
		shift -= straddval;
		str += straddval;
		--length;

		if((length) % 8 == 0)
		{	
			res += numaddval;
			shift = (*string == 'L') ? 7 : 0;
		}
	}

	return number->size;
}
