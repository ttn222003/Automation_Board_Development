.PHONY: all run coverage clean rebuild

CC = gcc
CFLAGS = -Wall \
		-O0 \
		-g \
		--coverage \
		 -I./../../../ThirdParty/Unity/src \
		 -I./../../../ThirdParty/CMock/src \
		 -I./../../../Systems/PLC_Main/include \
		 -I./../../../Systems/CommonDefine \

LDFLAGS = --coverage

SRCS = ./../../../ThirdParty/Unity/src/unity.c \
	   ./../../../ThirdParty/CMock/src/cmock.c \
       ./TestRunner.c \
       ./TestPlcGpio.c \
       ./../../../Systems/PLC_Main/src/PlcGpio.c \

TARGET = TestRunner
BUILD_DIR = build
COVERAGE_REPORT = coverage_report

OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SRCS)))

VPATH = ./../../../ThirdParty/Unity/src \
        ./../../../ThirdParty/CMock/src \
        ./../../../Systems/PLC_Main/src \
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
		--root "$(abspath ../../../../..)" \
		--filter ".*PlcGpio.*" \
		--txt-metric branch \
		--txt $(COVERAGE_REPORT)/summary.txt \
		--json $(COVERAGE_REPORT)/.raw.json \
		--print-summary
	@python -c "\
import json, os; \
root = os.path.abspath('$(abspath ../../../../..)'); \
d = json.load(open('$(COVERAGE_REPORT)/.raw.json')); \
f = open('$(COVERAGE_REPORT)/uncovered.txt', 'w'); \
[f.write( \
    os.path.relpath(x.get('file', x.get('filename', '?')), root) + '\n' + \
    '  Uncovered lines  (C0): ' + (', '.join(str(l['line_number']) for l in x['lines'] if l['count'] == 0) or 'none') + '\n' + \
    '  Uncovered branch (C1): ' + (', '.join('line ' + str(l['line_number']) + ' branch ' + str(i) for l in x['lines'] for i,b in enumerate(l.get('branches',[])) if b.get('count',0) == 0) or 'none') + '\n\n' \
) for x in d['files']]; \
f.close()"
	@echo ""
	@echo "summary.txt  : $(COVERAGE_REPORT)/summary.txt"
	@echo "uncovered.txt: $(COVERAGE_REPORT)/uncovered.txt"

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	find . -name "*.gcda" -delete
	find . -name "*.gcno" -delete
	find . -name "*.info" -delete

rebuild: clean all