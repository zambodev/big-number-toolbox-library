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
	/* Set size */
	size_t size1 = number1->size;
	size_t size2 = number2->size;

	/* Create pointers on the least byte*/
	byte* res = number1->num + number1->size - 1;
	byte* num = number2->num + number2->size - 1;

	/* Use a short to sum the bytes and catch the overflow */
	short overflow = 1;	
	/* Use the initialized shot to get the endianness of the system */
	byte big_endian_flag = (*(byte *) &overflow == 1) ? 0 : 1;
	overflow = 0;
	
	/* carry stoes the carry for the used byte */
	byte carry = 0;
	/* carry_tmp stores the carry generated in the uses byte  */
	byte carry_tmp = 0;

	while(size1 > 0 || size2 > 0)
	{
		/* If the second number is not at the end sum it */
		if(size2 > 0) 
		{	
			if(size1 == 0)
			{
				bn_ext(number1, 1);
				size1 = number1->size;
				res = number1->num;
			}

			overflow = *res;
			overflow += *num;
						
			carry_tmp += (big_endian_flag) ? overflow << 8 : overflow >> 8;
			*res = *(byte *) &overflow;
			
			/* Point to the next byte */
			num--;
			size2--;
		}

		/* If the carry is not zero sum it */
		if(carry != 0)
		{
			overflow = *res;	
			overflow += carry;

			*res = *(byte *) &overflow; 
			carry_tmp += (big_endian_flag) ? overflow << 8 : overflow >> 8;
		}

		/* Store carry_tmp in carry for the next byte */
		carry = carry_tmp;
		carry_tmp = 0;

		/* Point to he next byte */
		res--;
		size1--;

		if(carry != 0 && size1 == 0)
		{
			bn_ext(number1, 1);
			size1 = number1->size;
			res = number1->num;
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
	size_t size = number2->size;

	/* Create pointers on the least byte*/
	byte* res = number1->num + number1->size - 1;
	byte* num = number2->num + number2->size - 1;
	
	/* Set to 1 if the number 1 is empty and cannot borrow from him */	
	byte done = 0;	

	while((size--) > 0)
	{
		/* Byte index */
		size_t idx = 0;
		
		/* If the byte is 0 search for a byte to borrow from */
		if(*res < *num)
		{	
			/* Borrow a byte from the first non-zero byte */
			while(*(res - (++idx)) == 0)
			{
				/* If the serch raech the end size the number1 is embpy */
				if(idx == number1->size)
				{
					done++;
					break;
				}
			}
			
			/* If number 1 is empty end the subtraction */
			if(done) break;

			/* Sub one from the byte to borrow from */
			*(res - (idx)) -= 1;
			/* Set the other empty to 255 */
			while((--idx) > 0) *(res - idx) = 255;
		}

		/* Sub number2 from number1 */
		*res -= *num;
		/* Point to the next bytes */
		res--;	
		num--;
	}
}


/**
 *	@brief Multiply two bn_t numbers storing the result into number1
 *	@param [in,out] number1 Initialized number
 *	@param [in] number2 Initialized number
*/
void bn_mul(bn_t *number1, bn_t *number2)
{
	bn_t *tmp = malloc(sizeof(bn_t));
	bn_init(tmp, number1->size + number2->size);
	bn_hcpy(tmp, number1);

	memset(number1->num, 0, number1->size);

	size_t size = number2->size;

	/* Pointer at the end of the second number */
	byte* num2 = number2->num + number2->size - 1;
	/* Used to check when the used bit is 1 */
	byte check = 1;					

	while(size > 0)
	{	
		/* If number2's bit is 1 add the number1 into result */
                if((*num2 & check) != 0) bn_sum(number1, tmp);

		/* Shift number1 by 1 */
		bn_sl(tmp);
                check <<= 1;

		/* When the current byte is ended, go to the next one */
		if(check == 0)
		{
			size--;
			num2--;
			check = 1;
		}
	}

	bn_free(tmp);
	bn_srk(number1);
}


/**
 *	@brief Divide two bn_t numbers storing the result into number1 and the rest into rest
 *	@param [in,out] number1 Initialized number
 *	@param [in] number2 Initialized number
 *	@param [in,out] rest Initialized number
*/
void bn_div(bn_t *number1, bn_t *number2, bn_t *rest)
{
	byte check = 128;
	size_t size = number1->size*8;
	
	bn_t *tmp = malloc(sizeof(bn_t));
	bn_t *result = malloc(sizeof(bn_t));
	
	bn_init(tmp, number1->size);
	bn_init(result, number1->size);

	/* Skip front zero bits */
	while((check & *number1->num) == 0)
	{
		bn_sl(number1);
		size--;
	}
	
	/* Set check to 0 */
	check ^= check;

	/* Sub number2 when possible */
	while((size--) > 0)
	{
		bn_ssl(tmp, number1);
		bn_comp(tmp, number2, &check);

		if(check <= 1)
		{
			bn_sub(tmp, number2);
			bn_inc(result);
		}

		bn_sl(result);	
	}
	/* Correct the over-shifting */
	bn_sr(result);

	/* Set result and rest */
	bn_cpy(number1, result);
	bn_cpy(rest, tmp);

	bn_srk(number1);
	bn_srk(rest);
}




