[![License](https://img.shields.io/github/license/Zambo-dev/big-number-toolbox-library?style=flat-square)](./LICENSE)
[![Build](https://github.com/Zambo-dev/big-number-toolbox-library/actions/workflows/compile.yml/badge.svg)](https://github.com/Zambo-dev/big-number-toolbox-library/actions/workflows/compile.yml)
<!--[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/Zambo-dev/big-number-toolbox-library.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/Zambo-dev/big-number-toolbox-library/context:cpp)
-->
# Big Number Toolbox Library

This library implements mathematical operations for numbers over the usual 64bit limit as efficently as possible and tries to keep a reasonable readability

<br>

## Get started
```bash
# Build the library and create the archive
make

# Link into a project
gcc -o output_name your_file_name -l bntl -I header_folder -L lib_folder
    
# Run test from test/ folder
# Use "debug" flag to run in debug mode (gcc -Wall -DDEBUG)
make debug test_name.c
```

<br>

## Info
- **Numbers:** Chunks of unsigned long, which size depends on the architecture to match full CPU capabilities, dynamically allocated
- **Byte order:** The number's byte order is the same as the CPU's

<br>

## Contribute
To contribute follow the instructions in [CONTRIBUTING.md](./CONTRIBUTING.md)<br>
Devlopment version of the repo is in the "dev" branch
