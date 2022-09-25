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

	bn_init_s(&num1, "B100000110000111100111100000001011100111111101010110101101001011011100100110101101011110101110110110110011000000100011011000110110010100011110011001111111001011100000100110100011010110111000010000001111010110110101110001011111010111000101111010100001100011");
	bn_init_s(&num2, "B000000001000001011000111111101100011000000001111100010001110101110000011010010011101000010110101011011011110100000100011010111011001000000111011001011110000101000100011000011000111010100101111010010000001001000100101011001111000100101001101101011000101111");
	bn_init_s(&sum_check, "B100000101000110001110100000011111001111111011011010011011010101101100001100011001110110011000001011010111001100011110111101111011001100010111000000100001000110011100001110001010011100010010010101111111001101110001000110010000010010011100001101001000110100");
	
	printf("Num1:\nSize: %lu\n", num1.size);
	bn_print(&num1);

	ulong sumstart = nanos();
	//bn_add(&num1, &num1, &num2);
	bn_sub(&num1, &num1, &num2);
	ulong sumend = nanos();

	printf("Num1:\nSize: %lu\n", num1.size);
	bn_print(&num1);

	ubyte check;
	bn_comp(&num1, &sum_check, &check);

	printf("Check: %u\n", check);

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
	printf("Sl duration: %lu\n", sumend-sumstart);

	printf("\n");
	return EXIT_SUCCESS;
}
