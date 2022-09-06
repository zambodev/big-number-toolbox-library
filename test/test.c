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
	bn_t num1, num2;
	ulong start = nanos();

	ulong slstart = nanos();
	bn_init_s(&num1, "L10011110010");
	bn_init_s(&num2, "B0000000001010101010100000000010111110101");
	ulong slend = nanos();
	
	puts("result:");
	bn_print(&num1);

	puts("result2:");
	bn_print(&num2);


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
	bn_free(&num1);
	bn_free(&num2);

	ulong end = nanos();

	printf("Test duration: %lu\n", end-start);
	printf("Allocation duration: %lu\n", slend-slstart);
	//printf("Comparison duration: %lu\n", cend-cstart);
	//printf("Sum duration: %lu\n", sumend-sumstart);


	printf("\n");
	return EXIT_SUCCESS;
}
