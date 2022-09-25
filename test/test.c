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

	bn_init_s(&num1, "B0000000000000000000000000000000000000001000000000000000000000000");
	bn_init_s(&num2, "B1000000000000000000000000000000000000000000000000000000000000000");
	bn_init(&sum_check, num2.size);
	
	printf("Num1:\nSize: %lu\n", num1.size);
	bn_print(&num1);

	ulong sumstart = nanos();
	//bn_add(&sum_check, &num1, &num2);
	bn_sub(&num1, &num1, &num2);
	ulong sumend = nanos();

	printf("Num1:\nSize: %lu\n", num1.size);
	bn_print(&num1);

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
