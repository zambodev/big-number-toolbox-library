[![License](https://img.shields.io/github/license/Zambo-dev/big-number-toolbox-library?style=flat-square)](./LICENSE)
[![Build](https://github.com/Zambo-dev/big-number-toolbox-library/actions/workflows/compile.yml/badge.svg)](https://github.com/Zambo-dev/big-number-toolbox-library/actions/workflows/compile.yml)
<!--[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/Zambo-dev/big-number-toolbox-library.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/Zambo-dev/big-number-toolbox-library/context:cpp)
-->
# Big Number Toolbox Library

This library implements mathematical operations for numbers over the usual 64bit limit as efficently as possible and tries to keep a reasonable readability

---

## Get started
```bash
# Build the library and create the archive
make

# Run test from test/
make test_name.c

# Link into a project
gcc -o output_name your_file_name -l bntl -I header_folder -L lib_folder
```
---

## Code sample
```C
	/* Coming soon... */
```

## Info
- **Numbers:** Chunks of unsigned long, which size depends on the architecture to match full CPU capabilities, allocated in the heap
- **Byte order:** The number's byte order follow the CPU architercture
