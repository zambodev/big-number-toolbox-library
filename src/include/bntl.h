/**
 * @file bntl.h
 * @brief Library header
 * @author Zambo-dev
 * @date 23/12/2021
 * @copyright Zambo-dev - 2022 | licensed under MIT
*/


#ifndef BNTL_H
#define BNTL_H


/*
################################
	Include libraies
################################
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASCII_ZERO 48


/*
################################
	Define datatypes
################################
*/

typedef unsigned long ulong;
typedef char byte;
typedef unsigned char ubyte;		/* Use byte ad an unsigned char (8 bit) */
typedef struct BN_T					/* Big number struct */
{
	size_t size;
	ulong* num;
} bn_t;


/*
################################
	Initialization
################################
*/

size_t bn_init(bn_t *number, size_t size);					/* Initialize n bytes and set them to 0 */
size_t bn_init_n(bn_t *number, ulong value);
size_t bn_init_s(bn_t *number, char *string);

/*
################################
	Operations
################################
*/

void bn_sum(bn_t *number1, bn_t *number2);			/* Add number2 into number1 */
void bn_sub(bn_t *number1, bn_t *number2);			/* Subtract number2 from number1 */
void bn_mul(bn_t *number1, bn_t *number2);			/* Multiply number1 by number2 */
void bn_div(bn_t *number1, bn_t *number2, bn_t *rest);		/* Divide number1 by number2 and store the rest */


/*
################################
	Utility
################################
*/
				
void bn_inc(bn_t *number);					/* Increment number by one */
void bn_dec(bn_t *number);					/* Decrement number by one */
void bn_ext(bn_t *number, size_t bytes);				/* Extend number size by n bytes */
void bn_free(bn_t *number);					/* Free number */
void bn_sl(bn_t *number);					/* Shift number left by one bit */
void bn_ssl(bn_t *output, bn_t *number);				/* Shift number left by one bit and store the overflow in output */
void bn_sr(bn_t *number);					/* Shift number right by one bit */
void bn_ssr(bn_t *output, bn_t *number);				/* Shift number right by one bit and store the overflow in output */
void bn_srk(bn_t *number);					/* Shrink empty bytes from the front of the number */
void bn_cpy(bn_t *result, bn_t *number);				/* Copy number into result reallocating the size */
void bn_hcpy(bn_t *result, bn_t *number);				/* Copy number into result without reallocating (set to 0 the surplus) */
void bn_ncpy(bn_t *result, bn_t *number, size_t size);		/* Copy n bytes from number to result starting from the less valuable byte */
void bn_rev(bn_t *number);					/* Revers byte order */
void bn_comp(bn_t *number1, bn_t *number2, ubyte *result);		/* Compare numbers and set check to 0: equal, 1: first bigger, 2: second bigger */

/*
################################
	Conversion
################################
*/

char * bntobs(bn_t *number);					/* Convert number to binary string */


/*
################################
	Print
################################
*/

void bn_print(bn_t *number);					/* Print number into stdout in binary string format */


#endif
