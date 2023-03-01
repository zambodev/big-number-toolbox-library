#include "bitwise.h"


/* NOT */
void bn_not(bn_t *destn, bn_t *source)
{
	if(source->size == 0 || destn->size == 0) return;
	if(destn->size != source->size) return;

	ulong *dst = destn->num; 
	ulong *src = source->num;
	size_t size = source->size/sizeof(ulong);
	
	while(size > 0)
	{
		*dst = ~(*src);
		++dst;
		++src;
		--size;
	}
}

/* Shift value left by n-val bits */
void bn_sl(bn_t *destn, bn_t *source, ulong val)
{
	if(source->size == 0 || destn->size == 0) return;
 
	ulong tmp, carry = 0, shift, *src, *dst_st, *dst_end;
	byte inc;
	size_t size1, size2;
	
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

	/* Set numbers size */
	size1 = source->size/sizeof(ulong);
	size2 = destn->size/sizeof(ulong);
	/* Set full byte chunks shift */
	shift = val/(sizeof(ulong)*8);
	if(shift > size2) shift = size2;
	/* Set val to remaining bits to shift */
	val -= shift*(sizeof(ulong)*8);

	/* Check for size cases 
	* If the two sizes are equal shift the source end to ignore cut off chunks
	* If the destination is bigger shift the destn end to ignore useless chunks 
	* and if the shift will overflow anyway shift the source end to ignore cut off chunks
	*/
	if(size2 == size1)
	{
		size1 -= shift;
		src -= inc*shift;
	}
	else
	{
		tmp = size1+shift;
		if(size2 < tmp)
		{
			src -= (tmp-size2);
			size1 -= (tmp-size2);
			tmp = size2;
		}

		dst_end -= inc*(size2-tmp);
		size2 -= size2-tmp;
	}

	/* Shift chunk into new position */
	while(size1 > 0)
	{
		*dst_end = *src;
	
		src -= inc;
		dst_end -= inc;
		--size1;
		--size2;
	}
	/* Set remaining bytes to 0 */
	while(size2 > 0)
	{
		*dst_end = 0;

		dst_end -= inc;
		--size2;
	}
	
	/* Restore size for a new loop */
	size2 = destn->size/sizeof(ulong);	

	/* Shift remaining bits */
	while(size2 > 0 && val > 0)
	{
		/* Store in tmp the carryf or the next chunk of bytes */
		tmp = (*dst_st & (ULONG_MAX << (sizeof(ulong)*8-val))) >> (sizeof(ulong)*8-val);
		/* Shift the value */
		*dst_st <<= val;
		/* Insert the carry */
		*dst_st |= carry;
		/* Set carry for the next chunk */
		carry = tmp;	
		tmp = 0;
		
		dst_st += inc;
		--size2;
	}
}

/* Shift value right by n-val bits */
void bn_sr(bn_t *destn, bn_t *source, ulong val)
{
	if(source->size == 0 || destn->size == 0) return;
 
	ulong tmp, carry = 0, shift, *src, *dst_st, *dst_end;
	byte inc;
	size_t size1, size2;
	
	#if(defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN)
		src = source->num;
		dst_end = destn->num;
		dst_st = destn->num + destn->size/sizeof(ulong) - 1;
		inc = -1;
	#elif(defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN)
		src = source->num + source->size/sizeof(ulong) - 1;
		dst_st = destn->num;
		dst_end = destn->num + destn->size/sizeof(ulong) - 1;
		inc = 1;
	#else
		#error "Unsupported architecture!"
	#endif

	/* Set numbers size */
	size1 = source->size/sizeof(ulong);
	size2 = destn->size/sizeof(ulong);
	/* Set full byte chunks shift */
	shift = val/(sizeof(ulong)*8);
	if(shift > size2) shift = size2;
	/* Set val to remaining bits to shift */
	val -= shift*(sizeof(ulong)*8);

	/* Check for size cases 
	* If the two sizes are equal shift the source end to ignore cut off chunks
	* If the destination is bigger shift the destn end to ignore useless chunks 
	* and if the shift will overflow anyway shift the source end to ignore cut off chunks
	*/
	if(size2 == size1)
	{
		size1 -= shift;
		src -= inc*shift;
	}
	else
	{
		tmp = size1+shift;
		if(size2 < tmp)
		{
			src -= (tmp-size2);
			size1 -= (tmp-size2);
			tmp = size2;
		}

		dst_end -= inc*(size2-tmp);
		size2 -= size2-tmp;
	}

	/* Shift chunk into new position */
	while(size1 > 0)
	{
		*dst_end = *src;
	
		src -= inc;
		dst_end -= inc;
		--size1;
		--size2;
	}
	/* Set remaining bytes to 0 */
	while(size2 > 0)
	{
		*dst_end = 0;

		dst_end -= inc;
		--size2;
	}
	
	/* Restore size for a new loop */
	size2 = destn->size/sizeof(ulong);	

	/* Shift remaining bits */
	while(size2 > 0 && val > 0)
	{
		/* Store in tmp the carryf or the next chunk of bytes */
		tmp = (*dst_st & (ULONG_MAX >> (sizeof(ulong)*8-val))) << (sizeof(ulong)*8-val);
		/* Shift the value */
		*dst_st >>= val;
		/* Insert the carry */
		*dst_st |= carry;
		/* Set carry for the next chunk */
		carry = tmp;	
		tmp = 0;
		
		dst_st += inc;
		--size2;
	}
}
