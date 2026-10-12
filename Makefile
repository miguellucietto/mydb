CC = gcc
CLANG_FORMAT ?= clang-format

CFLAGS = -Wall -Wextra -Wpedantic -std=c17 -ggdb -Iinclude -MMD -MP
TARGET ?= mydb

CFILES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c,build/%.o,$(CFILES))
DEPS = $(OBJECTS:.o=.d)

all: $(TARGET)

run:
	./$(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf build $(TARGET)

format:
	$(CLANG_FORMAT) -i $(CFILES) $(wildcard include/*.h)

format-check:
	$(CLANG_FORMAT) --dry-run --Werror $(CFILES) $(wildcard include/*.h)

.PHONY: all clean format format-check
