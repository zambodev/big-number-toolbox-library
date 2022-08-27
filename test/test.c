#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <bntl.h>


// Theorical result: 01010110

int main()
{	
	bn_t result, result2;

	printf("Allocated %d bytes\n", bn_init_s(&result, "L10001100000000000000000000000000"));
	printf("Allocated %d bytes\n", bn_init_n(&result2, 0b00000000000000000000000010001100));

	puts("result:");
	bn_print(&result);
	puts("result2:");
	bn_print(&result2);

	bn_ext(&result, 2);
	puts("result ext");
	bn_print(&result);

	bn_ncpy(&result, &result2, 8);
	puts("result ncpy");
	bn_print(&result);

	bn_sl(&result, 6);
	puts("result sl");
	bn_print(&result);

	/* Free memory */
	bn_free(&result);
	bn_free(&result2);

	printf("\n");
	return EXIT_SUCCESS;
}
