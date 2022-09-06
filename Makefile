# ompiler settings
CC = gcc -m32
CFLAGS = -Wall -g

# Folders
SRC = src
BIN = bin
BUILD = build
FUNC = $(SRC)/func
LIB = lib
INC = $(SRC)/include

# Files
LIBA = libbntl.a
INCLIB = bntl
MAIN = test/test.c
HEAD = $(INC)/include.h
SRCS := $(wildcard $(FUNC)/*.c)
OBJS := $(SRCS:.c=.o)

# Executables
TEST = $(BIN)/test

all: dircheck $(LIBA)

$(TEST): dircheck $(LIBA)
	@ $(CC) $(CFLAGS) -o $@ $(MAIN) -l$(INCLIB) -I $(INC)/ -L $(LIB)/

# Create object files
%.o: %.c
	@echo -n 'Building objects -> '
	@ $(CC) -c $(CFLAGS) $^
	@ mv *.o $(BUILD)
	@echo done

# Create static library archive
$(LIBA): $(OBJS)
	@echo -n 'Creating static library archive -> '
	@ ar -rcs $@ $(BUILD)/*
	@ cp -p *.a $(LIB)/
	@ rm *.a
	@echo Done

# Check if all needed directory exists, if not, creates it
dircheck:
ifeq ("$(wildcard $(BIN))", "")
	@echo -n 'Creating bin/ folder -> '
	@ mkdir $(BIN)
	@echo done
endif
ifeq ("$(wildcard $(BUILD))", "")
	@echo -n 'Creating build/ folder -> '
	@ mkdir $(BUILD)
	@echo done
endif
ifeq ("$(wildcard $(LIB))", "")
	@echo -n 'Creating lib/ folder -> '
	@ mkdir $(LIB)
	@echo done
endif

test: $(TEST)
	@echo ' '
	./$^


# Clear folders
clean:
	rm $(BIN)/* $(BUILD)/* $(LIB)/*

# Delete also the directory
cleanall:
	rm -r $(BIN)/ $(BUILD)/ $(LIB)/
