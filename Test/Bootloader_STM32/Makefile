.PHONY: build run clean

CC := gcc

CFLAGS ?= -DTEST -DUNITY_COLOR

INCLUDES := -I./../../Unity/src										\
			-I./../../Bootloader_STM32/Cores/include				\
			-I./../../Bootloader_STM32/Dependencies/include			\
			-I./../../Bootloader_STM32/Platform/include	

SRC_UNITY_DIR	 := ./../../Unity/src
SRC_CORES_DIR	 := ./../../Bootloader_STM32/Cores/src
SRC_DEPES_DIR	 := ./../../Bootloader_STM32/Dependencies/src
SRC_PLATFORM_DIR := ./../../Bootloader_STM32/Platform/mocks
SRC_TESTS_DIR	 := ./tests

SRC := $(wildcard $(SRC_UNITY_DIR)/*.c)		\
		$(wildcard $(SRC_CORES_DIR)/*.c)	\
		$(wildcard $(SRC_DEPES_DIR)/*.c)	\
		$(wildcard $(SRC_PLATFORM_DIR)/*.c)

TEST_SRCS := $(wildcard $(SRC_TESTS_DIR)/*Test.c)
TEST_NAMES := $(notdir $(basename $(TEST_SRCS)))

$(TEST_NAMES):
	@echo "======= Build test: $@ ======="
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC) $(SRC_TESTS_DIR)/$@.c -o $@.exe

build: $(TEST_NAMES)
	@echo "======= Finish Building Test Output ======="

run:
	@for t in $(TEST_NAMES); do \
		echo "==== Run $$t ===="; \
		if [ -f $$t.exe ]; then \
			./$$t.exe || exit 1; \
		else \
			echo "Executable $$t.exe not found. Run 'make build' first."; exit 1; \
		fi \
	done

clean:
	@echo "Cleaning executables..."
	rm -f *.exe