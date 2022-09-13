# ompiler settings
CC = gcc
CFLAGS = -Wall -g 

# Folders
SRC = src
FUNC = $(SRC)/func
INC = $(SRC)/include
BIN = bin
BUILD = build
LIB = lib
CFL := $(BIN) $(BUILD) $(LIB)	# Create Folders List

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


all: archinfo $(CFL) $(LIBA)

# Build c files into object files
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
$(CFL):
ifeq ("$(wildcard $@)", "")
	@echo -n 'Creating $@ folder: '
	@ mkdir $@
	@echo -e '			$(GREEN)Done$(RESET)'
endif

# Compile the test file
$(TEST): $(CFL) $(LIBA)
	@ $(CC) $(CFLAGS) -o $@ $(MAIN) -l$(INCLIB) -I $(INC)/ -L $(LIB)/ -lpthread

# Print architecture info
archinfo:
	@echo 'Building on $(ARCH) architecture'

# Run the test file
test: $(TEST)
	@echo ' '
	./$^

# Clear folders
clean:
	rm $(BIN)/* $(BUILD)/* $(LIB)/*

# Clear folders and delete the directories
cleanall:
	rm -r $(BIN)/ $(BUILD)/ $(LIB)/
