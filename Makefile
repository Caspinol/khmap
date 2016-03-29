#chashtable makefile

# System stuff
UNAME		= $(shell uname)
SYSTEM		= $(UNAME)

RM		= /bin/rm -f

CC		= gcc
AR		= ar
SIZE		= size

# Libary names
LIB_NAME	= libkhmap.a
TEST_LIB_NAME	= khmap_test

# Buld directories
BUILD		= obj
LIB_DIR		= lib
TEST_DIR	= test
SRC_DIR 	= src

# The easiest is just to specify each file 
SRC		= khmap.c

TSRC		= setTest.c runTest.c CuTest.c 

INC		=

# Global defines
DEFINE		= -D_GNU_SOURCE

# Linker flags
LFLAGS		= -Llib/ -lkhmap
ifeq ($(SYSTEM), Linux)
LFLAGS 		+=
endif

# Compilation flags
CFLAGS 		= -Wall -Wextra -Wshadow -pedantic
CFLAGS		+= -g -ggdb -std=gnu99 -O3
CFLAGS		+= $(INC) $(DEFINE)

TCFLAGS		= -Wall -Wextra -Wshadow -pedantic
TCFLAGS		+= -g -ggdb -std=gnu99
TCFLAGS		+= $(INC) $(DEFINE)

VPATH		= src

# Test and core objects
OBJS		= $(addprefix $(BUILD)/, $(SRC:.c=.o))
# Build objects out of test files
TOBJS		= $(addprefix $(TEST_DIR)/, $(TSRC:.c=.o))

.PHONY: all dir clean maketest test

all: $(LIB_DIR)/$(LIB_NAME) $(TEST_DIR)/$(TEST_LIB_NAME)

dir: $(BUILD) $(LIB_DIR)

$(BUILD):
	@echo "MK DIR	-	 $@"
	@mkdir -p $@

$(LIB_DIR):
	@echo "MK DIR	-	 $@"
	@mkdir -p $@

$(BUILD)/%.o: %.c | dir
	@echo "CC 	-	$<"
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB_DIR)/$(LIB_NAME): $(OBJS)
	@echo "AR	-	$(LIB_NAME)"
	@$(AR) -cvq $@ $^
	@echo "SIZE	OF	$(LIB_NAME)"
	@$(SIZE) $(LIB_DIR)/$(LIB_NAME)

$(TEST_DIR)/%.o: %.c
	@echo "CC	-	$<"
	$(CC) $(TCFLAGS) -c -o $@ $<

$(TEST_DIR)/$(TEST_LIB_NAME): $(TOBJS) $(OBJS)
	@echo "LD	-	$(TEST_LIB_NAME)"
	@$(CC) $(LFLAGS) $^ -o $@
	@echo "Tests build - run: \"make test\""

test:
	$(TEST_DIR)/$(TEST_LIB_NAME)

clean:
	@echo "Deleting *.o files"
	@$(RM) $(OBJS) $(TOBJS) | true
	@$(RM) -r $(BUILD) | true
	@echo "Deleting $(LIB_NAME) libary"
	@$(RM) $(LIB_DIR)/$(LIB_NAME) | true
	@$(RM) -r $(LIB_DIR)/ | true
	@echo "Deleting test libary"
	@$(RM) $(TEST_DIR)/$(TEST_LIB_NAME) | true
	@echo "Cleanup complete!"
