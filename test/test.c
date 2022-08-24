#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <bntl.h>


// Theorical result: 01010110

int main()
{	
	bn_t result, result2;

	printf("Allocated %ld bytes\n", bn_init(&result, 10));
	printf("Allocated %ld bytes\n", bn_init_n(&result2, 0b00000000000000000000000010001100));

	puts("result:");
	bn_print(&result);
	puts("result2:");
	bn_print(&result2);

	bn_ext(&result2, 2);
	puts("result ext");
	bn_print(&result2);

	bn_sl(&result2, 4);
	puts("result sl");
	bn_print(&result2);

	bn_sr(&result2, 4);
	puts("result sr");
	bn_print(&result2);

	/* Free memory */
	bn_free(&result);
	bn_free(&result2);

	printf("\n");
	return EXIT_SUCCESS;
}
