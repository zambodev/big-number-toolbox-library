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
byte bn_free(bn_t *number)
{
	if(number == NULL) return -1;

	free(number->num);
	number->num = NULL;
	number->size = 0;
	free(number);

	return 0;
}


/**
 *	@brief Revers number's bytes
 *	@param [in,out] number Initialized number
*/
byte bn_rev(bn_t *number)
{
	if(number == NULL || number->num == NULL)
		return -1;

	byte *start = (byte *)number->num;
	byte *end = (byte *)number->num + number->size - 1;
	byte tmp;
	size_t size = number->size / 2;

	while(size > 0)
	{	
		tmp = *start;
		*start = *end;
		*end = tmp;

		++start;
		--end;
		--size;
	}

	return 0;
}


/**
 * 	@brief Copy value from number to result reallocating result memory
 *	@param [in,out] result Initialized number
 *	@param [in] number Initialized number
*/
byte bn_cpy(bn_t *result, bn_t *number)
{	
	if(result == NULL || number == NULL || number->num == NULL)
		return -1;

	ulong *tmp;
	size_t size = number->size;
	
	if((tmp = realloc(result->num, size)) == NULL)
		return -1;

	number->num = tmp;
	result->size = size;

	ulong *res = result->num;
	ulong *num = number->num;
	size /= sizeof(ulong);

	while(size > 0)
	{	
		*res = *num;
		++res;
		++num;
		--size;
	}

	return 0;
}


/**
 * 	@brief Copy value from number to result not reallocating result memory
 *	@param [in,out] result Initialized number
 *	@param [in] number Initialized number
 * 	@note Result can either be bigger or smaller than number, if is bigger the spare is set to 0, if is smaller only the available bytes are set
*/
byte bn_hcpy(bn_t *result, bn_t *number)
{	
	if(result == NULL || number == NULL || number->num == NULL)
		return -1;

	byte* res = result->num + result->size - 1;
	byte* num = number->num + number->size - 1;
	size_t num_size = number->size;
	size_t res_size = result->size;

	while(num_size > 0 && res_size > 0)
	{	
		*res = *num;
		res--;
		num--;
		num_size--;
		res_size--;
	}
	
	while(res_size > 0)
	{
		*res = 0;
		res--;
		res_size--;
	}

	return 0;
}


/**
 * 	@brief Copy n bytes of value from number to result
 *	@param [in,out] result Initialized number
 *	@param [in] number Initialized number
 *	@param [in] size Byte size
*/
byte bn_ncpy(bn_t *result, bn_t *number, size_t size)
{	
	if(result == NULL || number == NULL || number->num == NULL)
		return -1;

	byte* res = result->num + result->size - 1;
	byte* num = number->num + number->size - 1;
	size_t res_size = result->size;

	while(size > 0)
	{	
		*res = *num;
		res--;
		num--;
		size--;
		res_size--;
	}
	
	while(res_size > 0)
	{
		*res = 0;
		res--;
		res_size--;
	}

	return 0;
}


/**
 * 	@brief Extend a number by n bytes
 *	@param [in,out] result Initialized number
 *	@param [in] number Initialized number
 *	@param [in] size Byte size
*/
byte bn_ext(bn_t *number, size_t bytes)
{
	bn_t *tmp = malloc(sizeof(bn_t));
	bn_init(tmp, number->size + bytes);
	if(bn_hcpy(tmp, number) == -1) return -1;
	if(bn_cpy(number, tmp) == -1) return -1;
	if(bn_free(tmp) == -1) return -1;

	return 0;
}


/**
 * 	@brief Increment number by one
 *	@param [in,out] number Initialized number
*/
void bn_inc(bn_t *number)
{
	bn_t *one = malloc(sizeof(bn_t));
	bn_init_n(one, 1);

	bn_sum(number, one);

	bn_free(one);
}


/**
 * 	@brief Decrement number by one
 *	@param [in,out] number Initialized number
*/
void bn_dec(bn_t *number)
{
	bn_t *one = malloc(sizeof(bn_t));
	bn_init_n(one, 1);

	bn_sub(number, one);

	bn_free(one);
}


/**
 * 	@brief Shrink unused bytes in front of the value
 *	@param [in,out] number Initialized number
*/
void bn_srk(bn_t *number)
{
	byte* num = number->num;
	size_t size, old_size;
	old_size = size = number->size;

        while(size-1 > 0)
        {
                if((*num & 0b11111111) != 0)
                        break;
                
                num++; 
                size--;
        }

	if(size != old_size)
	{
		bn_t *tmp = malloc(sizeof(bn_t));
		bn_init(tmp, size);
		bn_ncpy(tmp, number, size);

		number->num = realloc(number->num, size);
		number->size = size;
		
		bn_hcpy(number, tmp);

		bn_free(tmp);
	}
}


/**
 * 	@brief Shift value left by one bit
 *	@param [in,out] number Initialized number
*/
void bn_sl(bn_t *number)
{
	byte* num = number->num + number->size - 1;
	size_t size = number->size;

	byte carry = 0;
	byte tmp = 0;
	byte check = 0b10000000;

	while(size > 0)
	{                
	if((*num & check) != 0)
					tmp++;

			*num <<= 1;

	if(carry != 0)
		*num |= carry;

	carry = tmp;
	tmp = 0;

	num--;
	size--;
	}
}


/**
 * 	@brief Shift number left by one bit and store the excess in output
 *	@param [in,out] output Initialized number
 *	@param [in] number Initialized number
 * 	@note Parameters need to have the same size
*/
void bn_ssl(bn_t *output, bn_t *number)
{
	byte* num = number->num + number->size - 1;
	size_t size = number->size;

        byte carry = 0;
	byte tmp = 0;
        byte check = 0b10000000;

        while(size > 0)
        {                
		if((*num & check) != 0)
                        tmp++;

		if(size == 1)
		{
			bn_sl(output);
			*(output->num+output->size-1) |= (*num & check) >> 7;
		}


                *num <<= 1;

		if(carry != 0)
			*num |= carry;

		carry = tmp;
		tmp = 0;
		
		num--;
		size--;
        }

}


/**
 * 	@brief Shift value right by one bit
 *	@param [in,out] number Initialized number
 * 	@note Require initialized number
*/
void bn_sr(bn_t *number)
{
        byte* num = number->num;
	size_t size = number->size;

        byte carry = 0;
	byte tmp = 0;
        byte check = 0b00000001;

        while(size > 0)
        {                
		if((*num & check) != 0)
                        tmp = 128;

                *num >>= 1;

		if(carry != 0)
			*num |= carry;

		carry = tmp;
		tmp = 0;

		num++;
		size--;
        }
}


/**
 * 	@brief Shift number right by one bit and store the excess in output
 *	@param [in,out] output Initialized number
 *	@param [in] number Initialized number
 * 	@note Parameters need to have the same size
*/
void bn_ssr(bn_t *output, bn_t *number)
{
	byte* num = number->num + number->size - 1;
	size_t size = number->size;

        byte carry = 0;
	byte tmp = 0;
        byte check = 0b00000001;

        while(size > 0)
        {                
		if((*num & check) != 0)
                        tmp++;

		if(size == 1)
		{
			bn_sl(output);
			*output->num |= (*num & check) << 7;
		}

                *num >>= 1;

		if(carry != 0)
			*num |= carry;

		carry = tmp;
		tmp = 0;
		
		num--;
		size--;
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
	size_t size1 = number1->size;
	size_t size2 = number2->size;

	byte* num1 = number1->num;
	byte* num2 = number2->num;

	while((size1--) > size2 && *(num1++) == 0) {}
	size1++;
	while(size1 < (size2--) && *(num2++) == 0) {}
	size2++;

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

	*result = 0;

	while(size1 > 0)
	{
		if(*num1 > *num2)
		{
			*result = 1;
			break;
		}
		else if(*num1 < *num2)
		{
			*result = 2;
			break;
		}


		num1++;
		num2++;
		size1--;
	}
}

