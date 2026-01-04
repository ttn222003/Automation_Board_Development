.PHONY: build run clean

CC := gcc

CFLAGS ?= -DTEST -DUNITY_COLOR

INCLUDES := -I./../../Unity/src										\
			-I./../../ESP32C3/BootloaderHost/Cores/include			\
			-I./../../ESP32C3/BootloaderHost/Dependencies/include	\
			-I./../../ESP32C3/BootloaderHost/Platform/include		\
			-I./../../ESP32C3/BootloaderHost/Platform/mocks

SRC_UNITY_DIR	 := ./../../Unity/src
SRC_CORES_DIR	 := ./../../ESP32C3/BootloaderHost/Cores/src
SRC_DEPES_DIR	 := ./../../ESP32C3/BootloaderHost/Dependencies/src
SRC_PLATFORM_DIR := ./../../ESP32C3/BootloaderHost/Platform/mocks
SRC_TESTS_DIR	 := ./tests

SRC := $(wildcard $(SRC_UNITY_DIR)/*.c)		\
		$(wildcard $(SRC_CORES_DIR)/*.c)	\
		$(wildcard $(SRC_DEPES_DIR)/*.c)	\
		$(wildcard $(SRC_PLATFORM_DIR)/*.c)

TEST_SRCS := $(wildcard $(SRC_TESTS_DIR)/*Test.c)
TEST_EXES := $(patsubst %.c,%.exe,$(notdir $(TEST_SRCS)))

%.exe: $(SRC_TESTS_DIR)/%.c $(SRC)
	@echo "======= Build test: $* ======="
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

build: $(TEST_EXES)
	@echo "======= Finish Building Test Output ======="

run: build
	@for t in $(TEST_EXES); do \
		echo "==== Run $$t ===="; \
		./$$t || exit 1; \
	done

clean:
	@echo "Cleaning executables..."
	rm -f *.exe