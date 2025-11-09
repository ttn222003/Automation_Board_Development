.PHONY: build run clean

CC := gcc
CXX := g++
CFLAGS := -Icpputest/include		\
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
OBJS := $(SRC:$(SRC_TEST_DIR)/%.cpp=$(OBJS_DIR)/%.o)
OBJS := $(OBJS:main.cpp=$(OBJS_DIR)/main.o)
OBJS := $(OBJS:$(SRC_CORES_DIR)/%.c=$(OBJS_DIR)/%.o)
OBJS := $(OBJS:tests/%.cpp=$(OBJS_DIR)/tests/%.o)

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
	./test.exe -c -v

clean:
	@echo "------- Clean all file -------"
	@echo "====================================================="
	@echo "======= Do you want to clean all files? (y/n) ======="
	@echo "====================================================="
	
	rm -rf test.exe
	rm -rf $(OBJS_DIR) test.exe