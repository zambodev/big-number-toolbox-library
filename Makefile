# ompiler settings
CC = gcc
CFLAGS = -Wall -g 

# Folders
SRC = src
BIN = bin
BUILD = build
LIB = lib
TEST = test
CFL := $(BIN) $(BUILD) $(LIB)	# Create Folders List

# Files
LIBA = libbntl.a
INCLIB = bntl
TESTFILE = $(basename $(word 1, $(MAKECMDGOALS))).c
SRCS := $(wildcard $(SRC)/*.c)
OBJS := $(addprefix $(BUILD)/, $(notdir $(SRCS:.c=.o)))

# Executables
EXE = $(basename $(TESTFILE))

# Architecture
ARCH = $(shell uname -m)

# Aesthetics
GREEN = \033[0;32m
RESET = \033[0m

all: archinfo $(CFL) $(LIBA)

# Build c files into object files
$(BUILD)/%.o: $(SRC)/%.c
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

# Compile the test file
$(TESTFILE): all
	@ $(CC) $(CFLAGS) -o $(BIN)/$(EXE) $(TEST)/$(TESTFILE) -l$(INCLIB) -I$(SRC)/ -L$(LIB)/
	./$(BIN)/$(EXE)

# Check if all needed directory exists, if not, creates it
$(CFL):
ifeq ("$(wildcard $@)", "")
	@echo -n 'Creating $@ folder: '
	@ mkdir $@
	@echo -e '			$(GREEN)Done$(RESET)'
endif

# Print architecture info
archinfo:
	@echo 'Test file: $(TESTFILE)'
	@echo 'Building on $(ARCH) architecture'

# Clear folders
clean:
	rm $(BIN)/* $(BUILD)/* $(LIB)/*

# Clear folders and delete the directories
cleanall:
	rm -r $(BIN)/ $(BUILD)/ $(LIB)/

