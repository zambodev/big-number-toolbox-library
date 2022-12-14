#include "init.h"


/* Initialize a number setting the value at zero */
void bn_init(bn_t *number, size_t size)
{
	size = (size % sizeof(ulong) == 0) ? size/sizeof(ulong) : size/sizeof(ulong)+1;
	number->size = size * sizeof(ulong);
	number->num = (size != 0) ? (ulong *)calloc(size, sizeof(ulong)) : NULL;
}

/* Initialize a number setting the value */
void bn_init_n(bn_t *number, ulong value)
{
	number->size = sizeof(ulong);
	number->num = (ulong *)calloc(1, sizeof(ulong));
	*(number->num) |= value;
}

/* Initialize number from a binary number string */
void bn_init_s(bn_t *number, char *string)
{
	char *ptr;
	byte strinc, numinc;
	ubyte *num, shift, count = 0;
	size_t len = strlen(string) - 1;
	bn_init(number, len/8);

	if(*string == 'L')
	{
		ptr = string + 1;
		strinc = 1;
		shift = 7;
	}
	else
	{
		ptr = string + len;
		strinc = -1;
		shift = 0;
	}

	#if(defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN)
		num = (ubyte *)number->num;
		numinc = 1;
	#elif(defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN)
		num = (ubyte *)number->num + number->size - 1;
		numinc = -1;

	#else
		#error "Unsupported architecture!"
	#endif

	while(len > 0)
	{
		*num |= (*ptr - ASCII_ZERO) << shift;
		
		shift -= strinc;
		ptr += strinc;
		++count;
		--len;

		if(count == 8)
		{
			shift = (strinc > 0) ? 7 : 0;
			num += numinc;
			count ^= count;
		}
	}


}
