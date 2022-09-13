# ompiler settings
CC = gcc
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
OBJS := $(addprefix $(BUILD)/, $(notdir $(SRCS:.c=.o)))

# Executables
TEST = $(BIN)/test

# Architecture
ARCH = $(shell uname -m)

# Aesthetics
GREEN = \033[0;32m
RESET = \033[0m

all: archinfo dircheck $(LIBA)

$(TEST): dircheck $(LIBA)
	@ $(CC) $(CFLAGS) -o $@ $(MAIN) -l$(INCLIB) -I $(INC)/ -L $(LIB)/ -lpthread

# Architecture info
archinfo:
	@echo 'Building on $(ARCH) architecture'

# Create object files
$(BUILD)/%.o: $(FUNC)/%.c
	@echo -n 'Building object $^: '
	@ $(CC) -c $(CFLAGS) $^
	@ mv *.o $(BUILD)
	@echo -e '	$(GREEN)Done$(RESET)'

# Create static library archive
$(LIBA): $(OBJS)
	@echo -n 'Creating static library archive: '
	@ ar -rcs $@ $(BUILD)/*
	@ cp -p *.a $(LIB)/
	@ rm *.a
	@echo -e '	$(GREEN)Done$(RESET)'

# Check if all needed directory exists, if not, creates it
dircheck:
ifeq ("$(wildcard $(BIN))", "")
	@echo -n 'Creating bin/ folder: '
	@ mkdir $(BIN)
	@echo -e '			$(GREEN)Done$(RESET)'
endif
ifeq ("$(wildcard $(BUILD))", "")
	@echo -n 'Creating build/ folder: '
	@ mkdir $(BUILD)
	@echo -e '		$(GREEN)Done$(RESET)'
endif
ifeq ("$(wildcard $(LIB))", "")
	@echo -n 'Creating lib/ folder: '
	@ mkdir $(LIB)
	@echo -e '			$(GREEN)Done$(RESET)'
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
