#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <bntl.h>

#define SIZE 8192


ulong nanos()
{
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC_RAW, &t);
	return (ulong)t.tv_sec*1000000000+(ulong)t.tv_nsec;
}

int main()
{	
	bn_t num1, num2, sum_check;

	ulong total_st = nanos();
	ulong alloc_st = nanos();

	char *str = malloc(SIZE);
	char *str2 = malloc(SIZE);

	FILE *file = fopen("./data/nums.txt", "r");
	fread(str, 1, SIZE, file);
	fread(str2, 1, SIZE, file);
	fclose(file);


	bn_init_s(&num1, str);
	bn_init_s(&num2, str2);
	bn_init(&sum_check, num1.size);
	bn_cpy(&sum_check, &num1);

	free(str);
	free(str2);

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

	ulong total_end = nanos();

	printf("Total time: %f ms\n", (total_end-total_st)*1e-6);
	printf("Allocation time: %f ms\n", (alloc_end-alloc_st)*1e-6);
	printf("Addition time: %f ms\n", (add_end-add_st)*1e-6);
	printf("Subtraction time: %f ms\n", (sub_end-sub_st)*1e-6);
	
	return EXIT_SUCCESS;
}

