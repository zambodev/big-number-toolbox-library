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

	printf("Allocated %d bytes\n", bn_init_s(&result, "L10001100000000000000000000000000"));
	printf("Allocated %d bytes\n", bn_init_n(&result2, 0b11111111111111110000000011111111));

	puts("result:");
	bn_print(&result);
	puts("result2:");
	bn_print(&result2);

	bn_ext(&result, 4);
	puts("result ext");
	bn_print(&result);

	bn_ncpy(&result, &result2, 16);
	puts("result ncpy");
	bn_print(&result);
	

	bn_sl(&result, 2);
	puts("result sl");
	bn_print(&result);

	/* Free memory */
	bn_free(&result);
	bn_free(&result2);

	ulong end = nanos();

	printf("Test duration: %u\n", end-start);

	printf("\n");
	return EXIT_SUCCESS;
}
