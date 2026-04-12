# Compiler and flags
CC      = gcc
CFLAGS  = -Wall -Wextra -Iinclude
LDFLAGS = 

# Colors
GREEN   = \033[32m
YELLOW  = \033[33m
RED     = \033[31m
RESET   = \033[0m

# Paths
SRC     = $(wildcard src/*.c)
OBJ     = $(patsubst src/%.c, build/%.o, $(SRC))
TARGET  = build/atm

# Default build
all: $(TARGET)
	@echo "$(GREEN)Build complete.$(RESET)"

# Build object files
build/%.o: src/%.c
	@mkdir -p build
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Link final executable
$(TARGET): $(OBJ)
	@echo "$(YELLOW)Linking...$(RESET)"
	@$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Debug build
debug: CFLAGS += -g
debug: clean all
	@echo "$(GREEN)Debug build ready.$(RESET)"

# Run program
run: all
	@echo "$(GREEN)Running ATM...$(RESET)"
	@./$(TARGET)

# Remove build artifacts
clean:
	@echo "$(RED)Cleaning build directory...$(RESET)"
	@rm -rf build/*

# Full rebuild
rebuild: clean all

.PHONY: all clean debug run rebuild
