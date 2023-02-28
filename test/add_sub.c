#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
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
	char *str = malloc(SIZE);
	char *str2 = malloc(SIZE);
	double add_bench = 0.0, sub_bench = 0.0;
	FILE *file;
	bn_t num1, num2;

	while((file = fopen("/tmp/bitstr.txt", "r")) == NULL)
	{
		char str[256];
		snprintf(str, 256, "python3 ./data/gen_bitstr.py %d %d", SIZE, SIZE);
		
		printf("Missing /tmp/bitstr.txt, running data/gen_bitstr.py...");
		fflush(stdout);
		
		system(str);
	}
	fread(str, 1, SIZE, file);
	fread(str2, 1, SIZE, file);
	fclose(file);

	bn_init_s(&num1, str);
	bn_init_s(&num2, str2);
	free(str);
	free(str2);

#ifdef DEBUG
	printf("Allocate num1(%lu B):\n", num1.size);
	bn_print(&num1);
	printf("Allocate num2(%lu B):\n", num2.size);
	bn_print(&num2);
#endif

	for(int i=0; i<100; ++i)
	{
		/* num1 = num1 + num2 */
		ulong add_st = nanos();
		bn_add(&num1, &num1, &num2);
		ulong add_end = nanos();

		ulong sub_st = nanos();
		bn_sub(&num1, &num1, &num2);
		ulong sub_end = nanos();

		printf("Add time: %f ms\n", (add_end-add_st)*1e-6);
		printf("Sub time: %f ms\n", (sub_end-sub_st)*1e-6);
		add_bench += (add_end-add_st)*1e-6;
		sub_bench += (sub_end-sub_st)*1e-6;
	}

	printf("\nAdd avg: %f ms\n", add_bench/100);
	printf("Sub avg: %f ms\n", sub_bench/100);

	bn_free(&num1);
	bn_free(&num2);

	return EXIT_SUCCESS;
}

