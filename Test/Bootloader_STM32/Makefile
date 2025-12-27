.PHONY: build run clean

CC := gcc

CFLAGS ?= -DTEST

INCLUDES := -I./../../Unity									\
			-I./../../Bootloader_STM32/Cores/include		\
			-I./../../Bootloader_STM32/Dependencies/include	\

SRC_UNITY_DIR := ./../../Unity
SRC_CORES_DIR := ./../../Bootloader_STM32/Cores/src
SRC_DEPES_DIR := ./../../Bootloader_STM32/Dependencies/src
SRC_TESTS_DIR := ./tests

SRC := $(wildcard $(SRC_UNITY_DIR)/*.c)		\
		$(wildcard $(SRC_CORES_DIR)/*.c)	\
		$(wildcard $(SRC_DEPES_DIR)/*.c)	\
		$(wildcard $(SRC_TESTS_DIR)/*.c)	\

OBJS_DIR := objs
OBJS := $(patsubst %.c,$(OBJS_DIR)/%.o,$(SRC))

$(OBJS_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build: $(OBJS)
	@echo "------- Building Test Output -------"
	$(CC) $(OBJS) -o test.exe

run:
	./test.exe

clean:
	@echo "------- Clean all file -------"
	@echo "====================================================="
	@echo "======= Do you want to clean all files? (y/n) ======="
	@echo "====================================================="
	
	rm -rf test.exe
	rm -rf $(OBJS_DIR) test.exe