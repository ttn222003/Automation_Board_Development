.PHONY: all run clean rebuild

CC = gcc
CFLAGS = -Wall \
		 -I./../../ThirdParty/Unity/src \
		 -I./../../ThirdParty/CMock/src \
		 -I./../../Systems/PLC_Main/include \
		 -I./../../Systems/CommonDefine \

SRCS = ./../../ThirdParty/Unity/src/unity.c \
	   ./../../ThirdParty/CMock/src/cmock.c \
       ./TestRunner.c \
       ./TestPlcGpio.c \
       ./../../Systems/PLC_Main/src/PlcGpio.c \

TARGET = TestRunner
BUILD_DIR = build

OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SRCS)))

VPATH = ./../../ThirdParty/Unity/src \
        ./../../ThirdParty/CMock/src \
        ./../../Systems/PLC_Main/src \
        .

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
	@echo ""
	@echo "Build OK → ./$(TARGET)"

run: all
	@echo ""
	@./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

rebuild: clean all