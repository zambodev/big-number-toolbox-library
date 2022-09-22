#include "../include/bntl.h"


/* Free number */
void bn_free(bn_t *number)
{
	if(number->size != 0)
	{
		free(number->num);
		number->num = NULL;
		number->size = 0;
	}
}

/* Copy source into destination */
void bn_cpy(bn_t *destn, bn_t *source)
{
	if(source->size == 0) return;
	if(destn->size == 0)
		bn_init(destn, source->size);
	else if(destn->size != source->size)
	{
		/* Reallocate number to the right size */
		ulong *tmp;
		if((tmp = realloc(destn->num, source->size)) == NULL)
			return;

		destn->num = tmp;
		destn->size = source->size;
	}

	size_t size = source->size;

	ulong *dst = destn->num;
	ulong *src = source->num;
	size /= sizeof(ulong);

	while(size > 0)
	{
		*dst = *src;
		++dst;
		++src;
		--size;
	}
}

/* Copy value from source to destination not reallocating destinatino memory */
void bn_hcpy(bn_t *destn, bn_t *source)
{
	if(source->size == 0) return;
	if(destn->size == 0)
		bn_init(destn, source->size);

	ubyte *dst, *src;
	byte inc;

	#if(defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN)
		dst = (ubyte *)destn->num;
		src = (ubyte *)source->num;
		inc = 1;
	#elif(defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN)
		dst = (ubyte *)destn->num + destn->size - 1;
		src = (ubyte *)source->num + source->size - 1;
		inc = -1;
	#else
		#error "Unsupported architecture!"
	#endif


	size_t src_size = source->size;
	size_t dst_size = destn->size;

	while(src_size > 0 && dst_size > 0)
	{
		*dst = *src;
		dst += inc;
		src += inc;
		--src_size;
		--dst_size;
	}

	while(dst_size > 0)
	{
		*dst = 0;
		dst += inc;
		--dst_size;
	}
}

/* Copy n bytes of value from source to destination */
void bn_ncpy(bn_t *destn, bn_t *source, size_t size)
{
	if(source->size == 0) return;
	if(destn->size ==0)
		bn_init(destn, size);

	ubyte *dst, *src;
	byte inc;

	#if(defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN)
		dst = (ubyte *)destn->num;
		src = (ubyte *)source->num;
		inc = 1;
	#elif(defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN)
		dst = (ubyte *)destn->num + destn->size - 1;
		src = (ubyte *)source->num + source->size - 1;
		inc = -1;
	#else
		#error "Unsupported architecture!"
	#endif

	size_t dst_size = destn->size;
	if(size > source->size)
		size = (source->size > dst_size) ? dst_size : source->size;

	while(size > 0)
	{
		*dst = *src;
		dst += inc;
		src += inc;
		--size;
		--dst_size;
	}

	while(dst_size > 0)
	{
		*dst = 0;
		dst += inc;
		--dst_size;
	}
}

/* Reverse bytes order */
void bn_rev(bn_t *destn, bn_t *source)
{
	if(source->size == 0 || destn->size == 0) return;
	if(destn->size != source->size) return;

	ubyte *src_st = (ubyte *)source->num;
	ubyte *src_end = (ubyte *)source->num + source->size - 1;
	ubyte *dst_st = (ubyte *)destn->num;
	ubyte *dst_end = (ubyte *)destn->num + destn->size - 1;
	ubyte tmp;
	size_t size = source->size / 2;

	while(size > 0)
	{
		tmp = *src_st;
		*dst_st = *src_end;
		*dst_end = tmp;
	
		++src_st;
		--src_end;
		++dst_st;
		--dst_end;
		--size;
	}
}

/* Extend a source by n bytes */
void bn_ext(bn_t *destn, bn_t *source, size_t bytes)
{
	if(source->size == 0) return;

	ulong mod = bytes % sizeof(ulong);
	bytes += (mod == 0) ? 0 : sizeof(ulong) - mod;

	bn_t tmp;
	bn_init(&tmp, source->size + bytes);
	bn_hcpy(&tmp, source);
	bn_cpy(destn, &tmp);
	bn_free(&tmp);
}

/* Shrink unused bytes in front of the value */
void bn_srk(bn_t *destn, bn_t *source)
{
	if(source->size == 0) return;

	ulong *src;
	byte inc;
	size_t size, old_size;

	#if(defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN)
		src = source->num + source->size/sizeof(ulong) - 1;
		inc = -1;
	#elif(defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN)
		src = source->num;
		inc = 1;
	#else
		#error "Unsupported architecture!"
	#endif

	old_size = size = source->size;
	
	while(size > sizeof(ulong))
	{	
		if((*src & ULONG_MAX) != 0) break;
		src += inc;
		size -= sizeof(ulong);
	}

	/* Resize number */
	if(size != old_size)
	{ 
		bn_t tmp;
		bn_init(&tmp, size);
		bn_ncpy(&tmp, source, size);
		bn_cpy(destn, &tmp);
		bn_free(&tmp);
	}
}

/* Shift value left by one bit */
void bn_sl(bn_t *destn, bn_t *source, ulong val)
{
	if(source->size == 0 || destn->size == 0) return;
 
	ulong tmp, carry = 0, shift, *src, *dst_st, *dst_end;
	byte inc;
	size_t size, size2;
	
	#if(defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN)
		src = source->num + source->size/sizeof(ulong) - 1;
		dst_end = destn->num + destn->size/sizeof(ulong) - 1;
		dst_st = destn->num;
		inc = 1;
	#elif(defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN)
		src = source->num;
		dst_st = destn->num + destn->size/sizeof(ulong) - 1;
		dst_end = destn->num;
		inc = -1;
	#else
		#error "Unsupported architecture!"
	#endif

	/* Full byte to be shifted */
	shift = val/(sizeof(ulong)*8);
	size2 = (destn->size - source->size)/sizeof(ulong);

	if(shift > 0)
	{
		/* Rest bits to be shifted */
		val -= shift*(sizeof(ulong)*8);
		/* Bytes offset to be copyed */
		size = source->size/sizeof(ulong);
		
		if(size > shift)
		{
			size -= shift;
			tmp = shift;
		}
		else if(size <= shift)
		{
			tmp = size;
			size = 0;
		};

		while(size > 0)
		{
			
			*dst_end = *(src-(inc*(shift-size2)));
			/* Decrement iterators */
			dst_end -= inc;
			if(size2 > 0) 
				--size2;
			else 
			{
				++shift;
				--size;
			}
		}
		while(tmp > 0)
		{
			*dst_end = 0;
			dst_end -= inc;
			--tmp;
		}
	}

	size = source->size/sizeof(ulong);

	while(size > 0 && val > 0)
	{
		tmp = (*dst_st & (ULONG_MAX << (sizeof(ulong)*8-val))) >> (sizeof(ulong)*8-val);
		*dst_st <<= val;
		*dst_st |= carry;
		
		carry = tmp;
		
		tmp = 0;
		dst_st += inc;
		--size;
	}
}

/**
 * 	@brief Shift value right by one bit
 *	@param [in,out] number Initialized number
 * 	@note Require initialized number
 */
void bn_sr(bn_t *destn, bn_t *source, ulong val)
{
	
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
	size_t size1 = number1->size/sizeof(ulong);
	size_t size2 = number2->size/sizeof(ulong);
	ulong *num1, *num2;
	byte inc;

	#if(defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN)
		num1 = number1->num + size1 - 1;
		num2 = number2->num + size2 - 1;
		inc = -1;
	#elif(defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN)
		num1 = number1->num;
		num2 = number2->num;
		inc = 1;
	#else
		#error "Unsupported architecture!"
	#endif

	while(*num1 == 0)
	{
		--size1;
		num1 += inc;
	}
	while(*num2 == 0)
	{
		--size2;
		num2 += inc;
	}

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

	while(size1 > 0)
	{
		if(*num1 > *num2)
		{
			*result = 1;
			return;
		}
		else if(*num1 < *num2)
		{
			*result = 2;
			return;
		}

		num1 += inc;
		num2 += inc;
		--size1;
	}	

	*result = 0;
}
