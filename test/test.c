#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <bntl.h>

// Theorical result: 01010110

ulong nanos()
{
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC_RAW, &t);
	return (ulong)t.tv_sec*1000000000+(ulong)t.tv_nsec;
}

int main()
{	
	bn_t result, result2;
	ulong start = nanos();

	ulong slstart = nanos();
	printf("Allocated %d bytes\n", bn_init_s(&result, "L10000001000000000000000010000000"));
	printf("Allocated %d bytes\n", bn_init_s(&result2, "L10001100001000000000000010000000"));
	ulong slend = nanos();
	
	puts("result:");
	bn_print(&result);
	puts("result2:");
	bn_print(&result2);

	bn_ext(&result2, 4);
	
	ulong sumstart = nanos();
	bn_sum(&result, &result2);
	ulong sumend = nanos();
	puts("result:");
	bn_print(&result);

	/*

	bn_ncpy(&result, &result2, 16);
	puts("result ncpy");
	bn_print(&result);
	
	bn_sl(&result, 32*8+2);	
	puts("result sl");
	bn_print(&result);
	
	bn_sr(&result, 32*8+2);	
	puts("result sr");
	bn_print(&result);

	ubyte cmp;
	ulong cstart = nanos();
	bn_comp(&result, &result2, &cmp);
	ulong cend = nanos();
	printf("Cmp: %u\n", cmp);
	*/



	/* Free memory */
	bn_free(&result);
	bn_free(&result2);

	ulong end = nanos();

	printf("Test duration: %lu\n", end-start);
	printf("Allocation duration: %lu\n", slend-slstart);
	//printf("Comparison duration: %lu\n", cend-cstart);
	printf("Sum duration: %lu\n", sumend-sumstart);


	printf("\n");
	return EXIT_SUCCESS;
}
