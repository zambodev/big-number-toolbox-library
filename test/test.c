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
	bn_t num1, num2, num3, sum_check;
	ulong start = nanos();

	ulong slstart = nanos();
	bn_init_s(&num1, "B100000110000111100111100000001011100111111101010110101101001011011100100110101101011110101110110110110011000000100011011000110110010100011110011001111111001011100000100110100011010110111000010000001111010110110101110001011111010111000101111010100001100011");
	bn_init_s(&num2, "B110001001000001011000111111101100011000000001111100010001110101110000011010010011101000010110101011011011110100000100011010111011001000000111011001011110000101000100011000011000111010100101111010010000001001000100101011001111000100101001101101011000101111");
	bn_init_s(&sum_check, "B1010001111001001000000011111110111111111111111010010111111000001001101000001000001000111000101100010001110110100100111110011110001011100100101110011011101010000100100111110111100010001011110001010011111011111111010011100101110011011101111100111111010010010");	
	bn_init_s(&num3, "B000000000000000000000000000000000000000000000000000000000000000");
	ulong slend = nanos();
	
	puts("num1:");
	bn_print(&num1);

	puts("num2:");
	bn_print(&num2);

	puts("num3:");
	bn_print(&num3);

	puts("sum_check:");
	bn_print(&sum_check);

	ulong sumstart = nanos();
	bn_sum(&num1, &num2);
	ulong sumend = nanos();
	puts("after sum num1:");
	bn_print(&num1);

	ubyte check;
	bn_comp(&num1, &sum_check, &check);
	if(check == 0)
		puts("Numbers are equal!");
	else
		puts("Numbers are not equal!");

	bn_srk(&num3);

	puts("srk num3:");
	bn_print(&num3);

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
		printf("Sum duration: %lu\n", sumend-sumstart);


	printf("\n");
	return EXIT_SUCCESS;
}
