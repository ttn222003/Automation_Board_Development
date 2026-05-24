.PHONY: all run coverage clean rebuild

CC = gcc
CFLAGS = -Wall \
		-O0 \
		-g \
		--coverage \
		-I./../../../ThirdParty/Unity/src \
		-I./../../../ThirdParty/CMock/src \
		-I./../../../Systems/Protocol/include \
		-I./../../../Systems/Dependencies/STM32F103C8T6/include \
		-I./../../../Systems/CommonDefine \
		-I./../../Dependencies/STM32F103C8T6/stub \
		-I./../../Dependencies/STM32F103C8T6/UART/mocks

LDFLAGS = --coverage

SRCS = ./../../../ThirdParty/Unity/src/unity.c \
	   ./../../../ThirdParty/CMock/src/cmock.c \
       ./TestRunner.c \
       ./Test_MainComn2Wifi_and_UartCommon.c \
	   ./../../Dependencies/STM32F103C8T6/UART/mocks/Mockstm32f1xx_hal_uart.c \
       ./../../../Systems/Protocol/src/MainComn2WifiCard.c \
       ./../../../Systems/Dependencies/STM32F103C8T6/src/UartCommon.c

TARGET = TestRunner
BUILD_DIR = build
COVERAGE_REPORT = coverage_report

OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SRCS)))

VPATH = ./../../../ThirdParty/Unity/src \
        ./../../../ThirdParty/CMock/src \
        ./../../../Systems/Protocol/src \
        ./../../../Systems/Dependencies/STM32F103C8T6/src \
		./../../Dependencies/STM32F103C8T6/UART/mocks \
        .

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo ""
	@echo "Build OK -> ./$(TARGET)"

run: all
	@echo ""
	@./$(TARGET)

coverage: run
	@echo ""
	@echo "Generating coverage report..."
	@mkdir -p $(COVERAGE_REPORT)
	@python -m gcovr \
		--gcov-executable gcov \
		--object-directory $(BUILD_DIR) \
		--root "$(abspath ../../..)" \
		--filter ".*/Systems/Protocol/src/MainComn2WifiCard\\.c" \
		--filter ".*/Systems/Dependencies/STM32F103C8T6/src/UartCommon\\.c" \
		--txt-metric branch \
		--txt $(COVERAGE_REPORT)/summary.txt \
		--json $(COVERAGE_REPORT)/.raw.json \
		--print-summary
	@python ./render_uncovered_report.py \
		--json $(COVERAGE_REPORT)/.raw.json \
		--root "$(abspath ../../..)" \
		--output $(COVERAGE_REPORT)/uncovered.txt
	@echo ""
	@echo "summary.txt  : $(COVERAGE_REPORT)/summary.txt"
	@echo "uncovered.txt: $(COVERAGE_REPORT)/uncovered.txt"

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	find . -name "*.gcda" -delete
	find . -name "*.gcno" -delete
	find . -name "*.info" -delete

rebuild: clean all
