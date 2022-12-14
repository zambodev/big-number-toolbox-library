#ifndef CALC_H
#define CALC_H

#include "bntl.h"


void bn_add(bn_t *destn, bn_t *number1, bn_t *number2);				/* Add number2 into number1 */
void bn_sub(bn_t *destn, bn_t *number1, bn_t *number2);				/* Subtract number2 from number1 */
void bn_mul(bn_t *number1, bn_t *number2);							/* Multiply number1 by number2 */
void bn_div(bn_t *number1, bn_t *number2, bn_t *rest);				/* Divide number1 by number2 and store the rest */

#endif
