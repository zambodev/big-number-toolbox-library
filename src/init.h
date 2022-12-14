#ifndef INIT_H
#define INIT_H

#include "bntl.h"


void bn_init(bn_t *number, size_t size);							/* Initialize n bytes and set them to 0 */
void bn_init_n(bn_t *number, ulong value);
void bn_init_s(bn_t *number, char *string);

#endif
