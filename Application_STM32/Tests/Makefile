.PHONY: build run clean

CXX := g++
CFLAGS := -Icpputest/include

# Get all source files
SRC_DIR := cpputest/src
SRC := $(wildcard $(SRC_DIR)/CppUTest/*.cpp)		\
		$(wildcard $(SRC_DIR)/CppUTestExt/*.cpp)	\
		$(wildcard $(SRC_DIR)/Platforms/Gcc/*.cpp)		\
		main.cpp

OBJS_DIR := obj
OBJS := $(SRC:$(SRC_DIR)/%.cpp=$(OBJS_DIR)/%.o) # pattern substitution to replace
OBJS := $(OBJS:main.cpp=$(OBJS_DIR)/main.o)

# Compile all .cpp files to .o files and store it to output folder
# To create .o file with structure like: obj/CppUTest/*.o, we must set to let makefile know where to find *.cpp like this
# cpputest/src/CppUTest/*.cpp. If just %.cpp makefile will find in CppUTest/*.cpp that doesn't exist.
$(OBJS_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) -c $^ -o $@

$(OBJS_DIR)/main.o: main.cpp
	$(CXX) $(CFLAGS) -c $^ -o $@

# Link all .o files and compile output file
build: $(OBJS)
	@echo "======= Building Test Output ======="
	$(CXX) $(OBJS) -o test.exe

# -c: color
run:
	@echo "======= Running Test ======="
	./test.exe -c

clean:
	@echo "======= Clean all file ========="
	@echo "====================================================="
	@echo "======= Do you want to clean all files? (y/n) ======="
	@echo "====================================================="
	
	rm -rf test.exe
	rm -rf $(wildcard $(OBJS_DIR)/*.o)
	rm -rf $(wildcard $(OBJS_DIR)/CppUTest/*.o)
	rm -rf $(wildcard $(OBJS_DIR)/CppUTestExt/*.o)
	rm -rf $(wildcard $(OBJS_DIR)/Platforms/Gcc/*.o)