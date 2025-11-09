.PHONY: build run clean

CC := gcc
CXX := g++
CFLAGS := -Icpputest/include		\
			-I./../Cores/include	\
			-I./../Cores/include

# Get all source files
SRC_TEST_DIR := cpputest/src
SRC_CORES_DIR := ./../Cores

SRC := $(wildcard $(SRC_TEST_DIR)/CppUTest/*.cpp)		\
		$(wildcard $(SRC_TEST_DIR)/CppUTestExt/*.cpp)	\
		$(wildcard $(SRC_TEST_DIR)/Platforms/Gcc/*.cpp)	\
		main.cpp										\
		$(wildcard $(SRC_CORES_DIR)/src/*.c)			\
		$(wildcard tests/*.cpp)

OBJS_DIR := obj
OBJS := $(SRC:$(SRC_TEST_DIR)/%.cpp=$(OBJS_DIR)/%.o) # pattern substitution to replace
OBJS := $(OBJS:main.cpp=$(OBJS_DIR)/main.o)
OBJS := $(OBJS:$(SRC_CORES_DIR)/%.c=$(OBJS_DIR)/%.o)
OBJS := $(OBJS:tests/%.cpp=$(OBJS_DIR)/tests/%.o)

# Compile all .cpp files to .o files and store it to output folder
# To create .o file with structure like: obj/CppUTest/*.o, we must set to let makefile know where to find *.cpp like this
# cpputest/src/CppUTest/*.cpp. If just %.cpp makefile will find in CppUTest/*.cpp that doesn't exist.
# mkdir -p: Automatically create folder for obj folder
$(OBJS_DIR)/%.o: $(SRC_TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -c $^ -o $@

$(OBJS_DIR)/main.o: main.cpp
	$(CXX) $(CFLAGS) -c $^ -o $@

$(OBJS_DIR)/%.o: $(SRC_CORES_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJS_DIR)/tests/%.o: tests/%.cpp
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