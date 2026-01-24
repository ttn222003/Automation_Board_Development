.PHONY: build run clean

CC := gcc

CFLAGS ?= -DTEST -DUNITY_COLOR
DEPE_FLAG ?= SRC

INCLUDES := -I./../../Unity/src										\
			-I./../../Bootloader_STM32/Cores/include				\
			-I./../../Bootloader_STM32/Dependencies/include			\
			-I./../../Bootloader_STM32/Platform/include				\
			-I./../../Bootloader_STM32/Platform/mocks

DEPE_DIR ?= ./../../Bootloader_STM32/Dependencies/src

SRC_UNITY_DIR		:= ./../../Unity/src
SRC_CORES_DIR		:= ./../../Bootloader_STM32/Cores/src
SRC_DEPES_DIR		:= ./../../Bootloader_STM32/Dependencies/src
MOCK_DEPES_DIR		:= ./../../Bootloader_STM32/Dependencies/mocks
SRC_PLATFORM_DIR	:= ./../../Bootloader_STM32/Platform/mocks
SRC_TESTS_DIR		:= ./tests
LOG_DIR 			:= ./logs

ifeq ($(DEPE_FLAG), SRC)
	DEPE_DIR := $(SRC_DEPES_DIR)
else ifeq ($(DEPE_FLAG), MOCK)
	DEPE_DIR := $(MOCK_DEPES_DIR)
endif

SRC := $(wildcard $(SRC_UNITY_DIR)/*.c)		\
		$(wildcard $(SRC_CORES_DIR)/*.c)	\
		$(wildcard $(DEPE_DIR)/*.c)	\
		$(wildcard $(SRC_PLATFORM_DIR)/*.c)

TEST_SRCS := $(wildcard $(SRC_TESTS_DIR)/*Test.c)
TEST_EXES := $(patsubst %.c,%.exe,$(notdir $(TEST_SRCS)))

%.exe: $(SRC_TESTS_DIR)/%.c $(SRC)
	@echo "======= Build test: $* ======="
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

$(LOG_DIR):
	@mkdir -p $(LOG_DIR)

build: $(TEST_EXES)
	@echo "======= Finish Building Test Output ======="

run: build $(LOG_DIR)
	@for t in $(TEST_EXES); do \
		name=$${t%.exe}; \
		echo "==== Run $$name ===="; \
		./$$t > $(LOG_DIR)/$$name.txt 2>&1 || exit 1; \
		echo "Log saved to $(LOG_DIR)/$$name.txt"; \
	done

clean:
	@echo "Cleaning executables and logs..."
	rm -f *.exe
	rm -rf $(LOG_DIR)
	@echo "Clean complete."