/**
 * @file bn_util.c
 * @brief Utility functions for handling big numbers
 * @author Zambo-dev
 * @date 22/12/2021
 * @copyright Zambo-dev - 2022 | licensed under MIT
 */

#include "../include/bntl.h"

/**
 * 	@brief Free number's memory
 *	@param [in,out] number Initialized number
 */
void bn_free(bn_t *number)
{
	if (number->num != NULL)
	{
		free(number->num);
		number->num = NULL;
	}
	number->size = 0;
}

/**
 *	@brief Revers number's bytes
 *	@param [in,out] number Initialized number
 */
void bn_rev(bn_t *number)
{
	if (number->num == NULL) return;

	ubyte *start = (ubyte *)number->num;
	ubyte *end = (ubyte *)number->num + number->size - 1;
	ubyte tmp;
	size_t size = number->size / 2;

	while (size > 0)
	{
		tmp = *start;
		*start = *end;
		*end = tmp;

		++start;
		--end;
		--size;
	}
}

/**
 * 	@brief Copy value from number to result reallocating result memory
 *	@param [in,out] result Initialized number
 *	@param [in] number Initialized number
 */
void bn_cpy(bn_t *result, bn_t *number)
{
	if (number->num == NULL) return;
	if (result->num == NULL)
		bn_init(result, number->size);
	else
	{
		/* Reallocate number to the right size */
		ulong *tmp;
		if ((tmp = realloc(result->num, number->size)) == NULL)
			return;

		result->num = tmp;
		result->size = number->size;
	}

	size_t size = number->size;

	ulong *res = result->num;
	ulong *num = number->num;
	size /= sizeof(ulong);

	while (size > 0)
	{
		*res = *num;
		++res;
		++num;
		--size;
	}
}

/**
 * 	@brief Copy value from number to result not reallocating result memory
 *	@param [in,out] result Initialized number
 *	@param [in] number Initialized number
 * 	@note Result can either be bigger or smaller than number, if is bigger the spare is set to 0, if is smaller only the available bytes are set
 */
void bn_hcpy(bn_t *result, bn_t *number)
{
	if (number->num == NULL) return;
	if (result->num == NULL)
		bn_init(result, number->size);

	ubyte *res, *num;
	byte inc;

	#if(defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN)
		res = (ubyte *)result->num;
		num = (ubyte *)number->num;
		inc = 1;
	#elif(defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN)
		res = (ubyte *)result->num + result->size - 1;
		num = (ubyte *)number->num + number->size - 1;
		inc = -1;
	#else
		#error "Unsupported architecture!"
	#endif


	size_t num_size = number->size;
	size_t res_size = result->size;

	while (num_size > 0 && res_size > 0)
	{
		*res = *num;
		res += inc;
		num += inc;
		--num_size;
		--res_size;
	}

	while (res_size > 0)
	{
		*res = 0;
		res += inc;
		--res_size;
	}
}

/**
 * 	@brief Copy n bytes of value from number to result
 *	@param [in,out] result Initialized number
 *	@param [in] number Initialized number
 *	@param [in] size Byte size
 */
void bn_ncpy(bn_t *result, bn_t *number, size_t size)
{
	if (number->num == NULL) return;
	if (result->num == NULL)
		bn_init(result, size);

	ubyte *res, *num;
	byte inc;

	#if(defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN)
		res = (ubyte *)result->num;
		num = (ubyte *)number->num;
		inc = 1;
	#elif(defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN)
		res = (ubyte *)result->num + result->size - 1;
		num = (ubyte *)number->num + number->size - 1;
		inc = -1;
	#else
		#error "Unsupported architecture!"
	#endif

	size_t res_size = result->size;
	if(size > number->size)
		size = (number->size > res_size) ? res_size : number->size;

	while (size > 0)
	{
		*res = *num;
		res += inc;
		num += inc;
		--size;
		--res_size;
	}

	while (res_size > 0)
	{
		*res = 0;
		res += inc;
		--res_size;
	}
}

/**
 * 	@brief Extend a number by n bytes
 *	@param [in,out] result Initialized number
 *	@param [in] number Initialized number
 *	@param [in] size Byte size
 */
void bn_ext(bn_t *number, size_t bytes)
{
	bytes += sizeof(ulong) - (bytes % sizeof(ulong));
	if (number->num == NULL)
		bn_init(number, bytes);
	else
	{
		bn_t tmp;
		bn_init(&tmp, number->size + bytes);
		bn_hcpy(&tmp, number);
		bn_cpy(number, &tmp);
		bn_free(&tmp);
	}
}

/**
 * 	@brief Shrink unused bytes in front of the value
 *	@param [in,out] number Initialized number
 */
void bn_srk(bn_t *number)
{
	if (number->num == NULL)
		return;

	ulong *num;
	byte add;
	size_t size, old_size;

	#if(defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN)
		num = number->num;
		add = 1;
	#elif(defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN)
		num = number->num + number->size - 1;
		add = -1;
	#else
		#error "Unsupported architecture!"
	#endif

	old_size = size = number->size / sizeof(ulong);
	
	while (--size > 1)
	{
		if ((*num & ULONG_MAX) != 0)
			break;

		num += add;
	}
	
	/* Resize number */
	if (size != old_size)
	{ 
		bn_t tmp;
		bn_init(&tmp, size);
		bn_ncpy(&tmp, number, size);
		bn_cpy(number, &tmp);
		bn_free(&tmp);
	}
}

/**
 * 	@brief Shift value left by one bit
 *	@param [in,out] number Initialized number
 */
void bn_sl(bn_t *number, ulong val)
{
	if (number == NULL || number->num == NULL)
		return;
 
	ulong tmp, carry = 0, shift, *start, *end;
	byte inc;
	size_t size;
	
	#if(defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN)
		start = number->num;
		end = number->num + number->size/sizeof(ulong) - 1;
		inc = 1;
	#elif(defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN)
		start = number->num  + number->size/sizeof(ulong) - 1;
		end = number->num;
		inc = -1;
	#else
		#error "Unsupported architecture!"
	#endif

	/* Full byte to be shifted */
	shift = val/(sizeof(ulong)*8);
	
	if(shift > 0)
	{
		/* Rest bits to be shifted */
		val -= shift*(sizeof(ulong)*8);
		/* Bytes offset to be copyed */
		size = number->size/sizeof(ulong);
		
		if(size > shift)
		{
			size -= shift;
			tmp = shift;
		}
		else if(size <= shift)
		{
			tmp = size;
			size = 0;
		};

		while(size > 0)
		{
			*end = *(end-(inc*shift));
			
			/* Decrement iterators */
			end -= inc;
			--size;
		}
		while(tmp > 0)
		{
			*end = 0;
			end -= inc;
			--tmp;
		}
	}

	size = number->size/sizeof(ulong);

	while(size > 0 && val > 0)
	{
		/* Store the carry for the next chunk */
		tmp = (*start & (ULONG_MAX << (sizeof(ulong)*8-val))) >> (sizeof(ulong)*8-val);
		/* Shift the chunk */
		*start <<= val;
		/* Insert carry bytes in the shifteed space */
		*start |= carry;
		
		carry = tmp;
		
		tmp = 0;
		start += inc;
		--size;
	}
}

/**
 * 	@brief Shift value right by one bit
 *	@param [in,out] number Initialized number
 * 	@note Require initialized number
 */
void bn_sr(bn_t *number, ulong val)
{
	if (number == NULL || number->num == NULL)
		return;
 
	ulong tmp, carry = 0, shift, *start, *end;
	byte inc;
	size_t size;
	
	#if(defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN)
		start = number->num;
		end = number->num + number->size/sizeof(ulong) - 1;
		inc = 1;
	#elif(defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN)
		start = number->num  + number->size/sizeof(ulong) - 1;
		end = number->num;
		inc = -1;
	#else
		#error "Unsupported architecture!"
	#endif

	/* Full byte to be shifted */
	shift = val/(sizeof(ulong)*8);
	
	if(shift > 0)
	{
		/* Rest bits to be shifted */
		val -= shift*(sizeof(ulong)*8);
		/* Bytes offset to be copyed */
		size = number->size/sizeof(ulong);
		
		if(size > shift)
		{
			size -= shift;
			tmp = shift;
		}
		else if(size <= shift)
		{
			tmp = size;
			size = 0;
		};

		while(size > 0)
		{
			*start = *(start+(inc*shift));
			
			/* Decrement iterators */
			start += inc;
			--size;
		}
		while(tmp > 0)
		{
			*start = 0;
			start += inc;
			--tmp;
		}
	}

	size = number->size/sizeof(ulong);

	while(size > 0 && val > 0)
	{
		/* Store the carry for the next chunk */
		tmp = (*end & (ULONG_MAX >> (sizeof(ulong)*8-val))) << (sizeof(ulong)*8-val);
		/* Shift the chunk */
		*end >>= val;
		/* Insert carry bytes in the shifteed space */
		*end |= carry;
		
		carry = tmp;
		
		tmp = 0;
		end -= inc;
		--size;
	}

}

/**
 * 	@brief Compare two numbers
 *	@param [in] number1 Initialized number
 *	@param [in] number2 Initialized number
 *	@param [in,out] result Single byte initialized
 * 	@note If they're equal result is set to 0, if number1 is greater, result is set to 1, if number2 is greater, result is set to 2
 */
void bn_comp(bn_t *number1, bn_t *number2, ubyte *result)
{
	size_t size1 = number1->size/sizeof(ulong);
	size_t size2 = number2->size/sizeof(ulong);
	ulong *num1, *num2;
	byte inc;

	#if(defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN)
		num1 = number1->num + size1 - 1;
		num2 = number2->num + size2 - 1;
		inc = -1;
	#elif(defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN)
		num1 = number1->num;
		num2 = number2->num;
		inc = 1;
	#else
		#error "Unsupported architecture!"
	#endif

	while(*num1 == 0)
	{
		--size1;
		num1 += inc;
	}
	while(*num2 == 0)
	{
		--size2;
		num2 += inc;
	}

	if(size1 > size2)
	{
		*result = 1;
		return;
	}
	else if(size1 < size2)
	{
		*result = 2;
		return;
	}

	while(size1 > 0)
	{
		if(*num1 > *num2)
		{
			*result = 1;
			return;
		}
		else if(*num1 < *num2)
		{
			*result = 2;
			return;
		}

		num1 += inc;
		num2 += inc;
		--size1;
	}	

	*result = 0;
}