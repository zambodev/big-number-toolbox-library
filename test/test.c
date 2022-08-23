#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <bntl.h>


// Theorical result: 01010110

int main()
{	
	/* Declare variables */
	bn_t *result, *result2;

	/* Allocate struct memeory (close to be changed) */
	result = malloc(sizeof(bn_t));
	result2 = malloc(sizeof(bn_t));

	bn_init(result, 10);
	bn_init_n(result2, 0b00000000000000000000000000000010);

	puts("result");
	bn_print(result);
	puts("result2");
	bn_print(result2);

	if(bn_cpy(result, result2) == -1) return EXIT_FAILURE;
	puts("result copied");
	bn_print(result);

	/* Free memory */
	bn_free(result);
	bn_free(result2);

	printf("\n");
	return EXIT_SUCCESS;
}
