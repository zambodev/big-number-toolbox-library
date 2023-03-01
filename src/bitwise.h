#ifndef BITWISE_H
#define BITWISE_H

#include "bntl.h"

void bn_not(bn_t *destn, bn_t *source);
void bn_and(bn_t *destn, bn_t *source);
void bn_or(bn_t *destn, bn_t *source);	
void bn_xor(bn_t *destn, bn_t *source);	
void bn_sl(bn_t *destn, bn_t *source, ulong val);					/* Shift number left by n-val bit */
void bn_sr(bn_t *destn, bn_t *source, ulong val);					/* Shift number right by n-val bit */

#endif
