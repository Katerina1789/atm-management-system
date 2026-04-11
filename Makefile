CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, build/%.o, $(SRC))
TARGET = build/atm

all: $(TARGET)

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

debug: CFLAGS += -g
debug: clean all

clean:
	@rm -rf build/*

.PHONY: all clean debug
