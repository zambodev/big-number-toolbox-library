#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <bntl.h>

ulong nanos()
{
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC_RAW, &t);
	return (ulong)t.tv_sec*1000000000+(ulong)t.tv_nsec;
}

int main()
{	
	bn_t num1, num2, sum_check;
	int *t = (int *)malloc(sizeof(int));

	ulong start = nanos();

	bn_init_s(&num1, "B110001001000001011000111111101100011000000001111100010001110101110000011010010011101000010110101011011011110100000100011010111011001000000111011001011110000101000100011000011000111010100101111010010000001001000100101011001111000100101001101101011000101111");
	bn_init(&num2, 56);
	bn_init(&sum_check, num2.size);
	
	puts("num1:");
	printf("Size: %lu\n", num1.size);
	bn_print(&num1);

	bn_ext(&sum_check, &num1, 8);
	bn_srk(&sum_check, &sum_check);
	/*
	puts("sum_check:");
	printf("Size: %lu\n", sum_check.size);
	bn_print(&sum_check);
	*/
	ulong slstart = nanos();
	bn_sl(&num2, &num1, 68);
	ulong slend = nanos();

	puts("num2:");
	printf("Size: %lu\n", num2.size);
	bn_print(&num2);

	/* Free memory */
	bn_free(&num1);
	puts("free num1");
	bn_free(&num2);
	puts("free num2");
	bn_free(&sum_check);
	puts("free sum");
	free(t);

	ulong end = nanos();

	printf("Test duration: %lu\n", end-start);
	printf("Sl duration: %lu\n", slend-slstart);

	printf("\n");
	return EXIT_SUCCESS;
}
