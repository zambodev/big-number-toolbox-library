#ifndef UTIL_H
#define UTIL_H

#include "bntl.h"


void bn_ext(bn_t *destn, bn_t *source, size_t bytes);				/* Extend number size by n bytes */
void bn_free(bn_t *number);											/* Free number */
void bn_srk(bn_t *destn, bn_t *source);								/* Shrink empty bytes from the front of the number */
void bn_cpy(bn_t *destn, bn_t *source);								/* Copy source into destn reallocating the size */
void bn_hcpy(bn_t *destn, bn_t *source);							/* Copy source into destn without reallocating (set to 0 the surplus) */
void bn_ncpy(bn_t *destn, bn_t *source, size_t size);				/* Copy n bytes from number to result starting from the less valuable byte */
void bn_rev(bn_t *destn, bn_t *source);								/* Revers byte order */
void bn_comp(bn_t *number1, bn_t *number2, ubyte *result);			/* Compare numbers and set check to 0: equal, 1: first bigger, 2: second bigger */

#endif
