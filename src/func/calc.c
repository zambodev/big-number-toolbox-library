/**
 * @file bn_calc.c
 * @brief Calculus functions for big numbers struct
 * @author Zambo-dev
 * @date 22/12/2021
 * @copyright Zambo-dev - 2022 | licensed under MIT
*/


#include "../include/bntl.h"

/**
 *	@brief Sum two bn_t numbers storing the result into number1
 *	@param [in,out] number1 Initialized number
 *	@param [in] number2  Initialized number
*/
void bn_add(bn_t *destn, bn_t *number1, bn_t *number2)
{	
	if(number1->size == 0 || number2->size == 0) return;

	ulong tmp = 0, carry = 0, shift, low, high, *dst, *num1, *num2;
	byte inc;
	size_t size, size1, size2;

	/* Extends result if allocated space isn't enough */
	if(destn->size < number2->size)
		bn_ext(destn, destn, number2->size - destn->size);

	size = destn->size/sizeof(ulong);
	size1 = number1->size/sizeof(ulong);
	size2 = number2->size/sizeof(ulong);
	shift = sizeof(ulong)*8/2;

	#if(defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN)
		num1 = number1->num;
		num2 = number2->num;
		dst = destn->num;
		inc = 1;
	#elif(defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN)
		num1 = number1->num + size1 - 1;
		num2 = number2->num + size2 - 1;
		dst = destn->num + size - 1;
		inc = -1;
	#else
		#error "Unsupported architecture!"
	#endif

	low = ULONG_MAX >> shift;
	high = ULONG_MAX << shift;

	while(size1 > 0 && (size2 > 0 || carry != 0))
	{
		tmp = (size2 > 0) ? (*num2 & low) : 0;
		tmp += (*num1 & low) + carry;
		carry = (tmp & high) >> shift;

		tmp ^= tmp & high;
		*dst ^= *dst & low;
		*dst |= tmp;		

		tmp = (size2 > 0) ? ((*num2 & high) >> shift) : 0;
		tmp += ((*num1 & high) >> shift) + carry;			

		carry = (tmp & high) >> shift;

		tmp <<= shift;
		*dst ^= *dst & high;
		*dst |= tmp;	

		num1 += inc;
		dst += inc;
		--size1;
		--size;

		if(size2 > 0)
		{
			num2 += inc;
			--size2;
		}

		if(size == 0 && carry != 0)
		{
			bn_ext(destn, destn, sizeof(ulong));
			size += sizeof(ulong);
		}
	}

}


/**
 *	@brief Sub two bn_t numbers storing the result into number1
 *	@param [in,out] number1 Initialized number
 *	@param [in] number2 Initialized number
*/
void bn_sub(bn_t *number1, bn_t *number2)
{	
	if(number1->num == NULL || number2->num == NULL) return;

	ulong tmp, carry = 1, carry2 = 0, shift, low, high, store, *num1, *num2;
	byte inc;
	size_t size1, size2;

	size1 = number1->size/sizeof(ulong);
	size2 = number2->size/sizeof(ulong);
	shift = sizeof(ulong)*8/2;

	if(size1 < size2) return;

	#if(defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN)
		num1 = number1->num;
		num2 = number2->num;
		inc = 1;
	#elif(defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN)
		num1 = number1->num + size1 - 1;
		num2 = number2->num + size2 - 1;
		inc = -1;
	#else
		#error "Unsupported architecture!"
	#endif

	low = ULONG_MAX >> shift;
	high = ULONG_MAX << shift;

	while(size1 > 0 && (size2 > 0 || carry != 0))
	{
		store = (size2 > 0) ? ~(*num2) : 0;
		tmp = (store & low) + carry;
		carry = (tmp & high) >> shift;
		store ^= store & low;
		store |= tmp & low;;	

		tmp = ((store & high) >> shift);
		tmp += carry;
		carry = (tmp & high) >> shift;

		tmp <<= shift;
		store ^= store & high;
		store |= tmp;

		tmp = (*num1 & low);
		tmp += (store & low) + carry2;
		carry2 = (tmp & high) >> shift;
		*num1 ^= *num1 & low;
		*num1 |= tmp & low;

		tmp = (*num1 & high) >> shift;
		tmp += ((store & high) >> shift) + carry2;

		carry2 = (tmp & high) >> shift;
		tmp <<= shift;

		*num1 ^= *num1 & high;
		*num1 |= tmp;

		num1 += inc;
		--size1;
		if(size2 > 0)
		{
			num2 += inc;
			--size2;
		}
	}

}


/**
 *	@brief Multiply two bn_t numbers storing the result into number1
 *	@param [in,out] number1 Initialized number
 *	@param [in] number2 Initialized number
*/
void bn_mul(bn_t *number1, bn_t *number2)
{

}


/**
 *	@brief Divide two bn_t numbers storing the result into number1 and the rest into rest
 *	@param [in,out] number1 Initialized number
 *	@param [in] number2 Initialized number
 *	@param [in,out] rest Initialized number
*/
void bn_div(bn_t *number1, bn_t *number2, bn_t *rest)
{

}



