[![License](https://img.shields.io/github/license/Zambo-dev/big-number-toolbox-library?style=flat-square)](./LICENSE)
[![Build](https://github.com/Zambo-dev/big-number-toolbox-library/actions/workflows/compile.yml/badge.svg)](https://github.com/Zambo-dev/big-number-toolbox-library/actions/workflows/compile.yml)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/Zambo-dev/big-number-toolbox-library.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/Zambo-dev/big-number-toolbox-library/context:cpp)

# Big Number Toolbox Library

This library implements mathematical operations for numbers over the usual 64bit limit as efficently as possible and tries to keep a reasonable readability

---

## Get started
```bash
# Build the library
make

# Link into a project
gcc -o output_name your_file_name -l bntl -I header_folder -L lib_folder
```
---

## Code sample
```C
/*
Factorial of 100 (100!)
*/

#include <bntl.h>


int main()
{	
	/* Declare variables */
	bn_t result, tmp, zero, one;
	byte check = 1;

	/* Allocate struct memeory (close to be changed) */
	result = malloc(sizeof(struct _bn_struct));
	zero = malloc(sizeof(struct _bn_struct));
	one = malloc(sizeof(struct _bn_struct));
	tmp = malloc(sizeof(struct _bn_struct));
                	
	bn_init_s(result, 1);	/* Multiplication neutral number */
	bn_init_s(one, 1);	/* Number one for subtraction */
	bn_init_s(tmp, 100);	/* Number for the factorial */
	bn_init(zero, 1);	/* Zero for comparison */

	/* Calculation loop */
	while(check)
	{
		bn_mul(result, tmp);
		bn_sub(tmp, one);
		bn_comp(tmp, zero, &check);
	}

	/* Print result size and result */
	printf("Result size %ld\n", result->size);
	bn_print(result);

	/* Free memory */
	bn_free(result);
	bn_free(zero);
	bn_free(one);
	bn_free(tmp);

	printf("\n");
	return EXIT_SUCCESS;
}
```
Note: Other used library such as stdio are not included in this example

## Info
- **Numbers:** It uses unsigned integers for now
- **Size limit:** Theoretically the max number of bytes of a number is the highest value of *size_t*, but it's not tested yet


## Notes

- All functions use big-endian numbers and all operations are made manipulating single bytes and bits 
- Return value of the functions is stored in the first parameter