.PHONY: build run clean

CC := gcc

CFLAGS ?= 

INCLUDES = -I./unity	\
			-I./../Cores/include

# Get all source files
SRC_CORES_DIR := ./../Cores

SRC :=	$(wildcard $(SRC_CORES_DIR)/src/*.c)			\
		$(wildcard tests/*.c)

OBJS_DIR := obj
OBJS := $(OBJS:$(SRC_CORES_DIR)/%.c=$(OBJS_DIR)/%.o)
OBJS := $(OBJS:tests/%.c=$(OBJS_DIR)/tests/%.o)

$(OBJS_DIR)/%.o: $(SRC_CORES_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJS_DIR)/tests/%.o: tests/%.c
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -c $^ -o $@

# Link all .o files and compile output file
build: $(OBJS)
	@echo "------- Building Test Output -------"
	$(CXX) $(OBJS) -o test.exe

# -c: color
run:
	@echo "------- Running Test -------"
	./test.exe -c

clean:
	@echo "------- Clean all file -------"
	@echo "====================================================="
	@echo "======= Do you want to clean all files? (y/n) ======="
	@echo "====================================================="
	
	rm -rf test.exe
	rm -rf $(OBJS_DIR) test.exe