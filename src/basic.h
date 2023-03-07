#ifndef CALC_H
#define CALC_H

#include "bntl.h"


void bn_add(bn_t *destn, bn_t *number1, bn_t *number2);
void bn_sub(bn_t *destn, bn_t *number1, bn_t *number2);
void bn_mul(bn_t *destn, bn_t *number1, bn_t *number2);
void bn_div(bn_t *number1, bn_t *number2, bn_t *rest);

#endif
