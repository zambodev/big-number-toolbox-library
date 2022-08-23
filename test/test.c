#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <bntl.h>


// Theorical result: 01010110

int main()
{	
	bn_t result, result2;

	printf("Allocated %ld bytes\n", bn_init(&result, 10));
	printf("Allocated %ld bytes\n", bn_init_n(&result2, 0b00000000000000000000000000000010));

	puts("result:");
	bn_print(&result);
	puts("result2:");
	bn_print(&result2);

	bn_ext(&result2, 2);
	puts("result ext");
	bn_print(&result2);


	bn_t num;
	ulong val = 1235;
	bn_init_n(&num, val);
	bn_print(&num);

	/* Free memory */
	bn_free(&result);
	bn_free(&result2);

	printf("\n");
	return EXIT_SUCCESS;
}
