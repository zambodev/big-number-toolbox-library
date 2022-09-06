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
void bn_sum(bn_t *number1, bn_t *number2)
{	
	if(number1->num == NULL || number2->num == NULL) return;

	ulong tmp = 1, carry = 0, shift, low, high, *num1, *num2;
	byte inc;
	size_t size1, size2;

	/* Extends result if allocated space isn't enough */
	if(number1->size < number2->size)
		bn_ext(number1, number2->size - number1->size);
	
	size1 = number1->size/sizeof(ulong);
	size2 = number2->size/sizeof(ulong);
	shift = sizeof(ulong)*8/2;

	/* Start from the higher value */
	if(*(ubyte *)&tmp == 1) 		/* Little endian */
	{
		num1 = number1->num;
		num2 = number2->num;
		inc = 1;
	}
	else							/* Big endian */
	{
		num1 = number1->num + size1 - 1;
		num2 = number2->num + size2 - 1;
		inc = -1;
	}

	tmp = 0;
	low = ULONG_MAX >> shift;
	high = ULONG_MAX << shift;

	while(size1 > 0)
	{
		tmp = (*num1 & low) + (*num2 & low) + carry;
		carry = (tmp & high) >> shift;

		tmp ^= tmp & high;
		*num1 ^= *num1 & low;
		*num1 |= tmp;		

		tmp = ((*num1 & high) >> shift) + ((*num2 & high) >> shift) + carry;			

		carry = (tmp & high) >> shift;

		tmp <<= shift;
		*num1 ^= *num1 & high;
		*num1 |= tmp;	

		num1 += inc;
		num2 += inc;
		--size1;
		
		if(size1 == 0 && carry != 0)
		{
			bn_ext(number1, sizeof(ulong));
			size1 += sizeof(ulong);
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




