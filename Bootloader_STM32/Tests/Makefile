.PHONY: build run clean

CC := gcc
CXX := g++
CFLAGS := -Icpputest/include				\
			-I./../Cores/include			\
			-I./../Dependencies/include

# Get all source folder
SRC_TEST_DIR := cpputest/src
SRC_CORES_DIR := ./../Cores

SRC_DEPE_DIR ?= ./../Dependencies
SRC_DEPE_MOCK_DIR ?= ./cpputest/src/UartBootloaderProtocolDepenedenciesMock
DEPE_DIR := $(SRC_DEPE_DIR)

TEST_PROD_DIR ?= ./tests
TEST_MOCK_DIR ?= ./mock_tests
TEST_DIR := $(TEST_PROD_DIR)

ifeq ($(DEPE), mock)
	DEPE_DIR := $(SRC_DEPE_MOCK_DIR)
	TEST_DIR := $(TEST_MOCK_DIR)
else ifeq ($(DEPE), production)
	DEPE_DIR := $(SRC_DEPE_DIR)
	TEST_DIR := $(TEST_PROD_DIR)
endif

SRC := $(wildcard $(SRC_TEST_DIR)/CppUTest/*.cpp)		\
		$(wildcard $(SRC_TEST_DIR)/CppUTestExt/*.cpp)	\
		$(wildcard $(SRC_TEST_DIR)/Platforms/Gcc/*.cpp)	\
		main.cpp										\
		$(wildcard $(SRC_CORES_DIR)/src/*.c)			\
		$(wildcard $(DEPE_DIR)/src/*.c)						\
		$(wildcard $(DEPE_DIR)/src/*.cpp)					\
		$(wildcard $(TEST_DIR)/*.cpp)

OBJS_DIR := obj
OBJS := $(SRC:$(SRC_TEST_DIR)/%.cpp=$(OBJS_DIR)/%.o)
OBJS := $(OBJS:main.cpp=$(OBJS_DIR)/main.o)
OBJS := $(OBJS:$(SRC_CORES_DIR)/%.c=$(OBJS_DIR)/%.o)
OBJS := $(OBJS:$(DEPE_DIR)/%.c=$(OBJS_DIR)/%.o)
OBJS := $(OBJS:$(DEPE_DIR)/%.cpp=$(OBJS_DIR)/%.o)
OBJS := $(OBJS:$(TEST_DIR)/%.cpp=$(OBJS_DIR)/%.o)

$(OBJS_DIR)/%.o: $(SRC_TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -c $^ -o $@

$(OBJS_DIR)/main.o: main.cpp
	$(CXX) $(CFLAGS) -c $^ -o $@

$(OBJS_DIR)/%.o: $(SRC_CORES_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJS_DIR)/%.o: $(DEPE_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJS_DIR)/%.o: $(DEPE_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/%.o: $(TEST_DIR)/%.cpp
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

print-sources:
	@echo "===== SRC LIST ====="
	@for f in $(SRC); do echo $$f; done