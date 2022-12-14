#ifndef BNTL_H
#define BNTL_H


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define ASCII_ZERO 48


typedef unsigned long ulong;
typedef char byte;
typedef unsigned char ubyte;
typedef struct BN_T
{
	size_t size;
	ulong* num;
} bn_t;

#include "init.h"
#include "calc.h"
#include "util.h"
#include "conv.h"
#include "print.h"

#endif
