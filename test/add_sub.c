#include <stdio.h>
#include <stdlib.h>
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

	ulong alloc_st = nanos();

	bn_init_s(&num1, "B100000110000111100111100000001011100111111101010110101101001011011100100110101101011110101110110110110011000000100011011000110110010100011110011001111111001011100000100110100011010110111000010000001111010110110101110001011111010111000101111010100001100011");
	bn_init_s(&num2, "B0000000010000010110001111111011000110000000011111000100011101011100000110100100111010000101101010110110111101000001000110101110110010000001110110010111100001010001000110000110001110101001011110100100000010010001001010110011110001001010011011010110001011111");
	bn_init(&sum_check, num1.size);
	bn_cpy(&sum_check, &num1);

	ulong alloc_end = nanos();

	printf("Allocate num1(%lu B):\n", num1.size);
	bn_print(&num1);
	printf("Allocate num2(%lu B):\n", num2.size);
	bn_print(&num2);
	printf("Allocate sum_check(%lu B):\n", sum_check.size);
	bn_print(&sum_check);

	/* num1 = num1 + num2 */
	ulong add_st = nanos();
	bn_add(&num1, &num1, &num2);
	ulong add_end = nanos();
	/* num2 = num1 - num2 */
	ulong sub_st = nanos();
	bn_sub(&num2, &num1, &num2);
	ulong sub_end = nanos();

	/* The result should be the initial num1 */
	ubyte check;
	bn_comp(&sum_check, &num2, &check);
	printf("Check (pass if 0): %u\n", check);

	bn_free(&num1);
	bn_free(&num2);
	bn_free(&sum_check);

	printf("Allocation time: %lu nanosec\n", alloc_end-alloc_st);
	printf("Addition time: %lu nanosec\n", add_end-add_st);
	printf("Subtraction time: %lu nanosec\n", sub_end-sub_st);
	
	return EXIT_SUCCESS;
}

