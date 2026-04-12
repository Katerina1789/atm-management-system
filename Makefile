# Compiler and flags
CC      = gcc
CFLAGS  = -Wall -Wextra -Iinclude -I/home/linuxbrew/.linuxbrew/include
LDFLAGS = -L/home/linuxbrew/.linuxbrew/lib -lsqlite3

# Colors
GREEN   = \033[0;32m
YELLOW  = \033[0;33m
RED     = \033[0;31m
RESET   = \033[0m

# Paths
SRC     = $(wildcard src/*.c)
OBJ     = $(patsubst src/%.c, build/%.o, $(SRC))
TARGET  = build/atm

# Default build
all: $(TARGET)
	@echo -e "$(GREEN)Build complete.$(RESET)"

# Build object files
build/%.o: src/%.c
	@mkdir -p build
	@echo -e "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Link final executable
$(TARGET): $(OBJ)
	@echo -e "$(YELLOW)Linking...$(RESET)"
	@$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Debug build
debug: CFLAGS += -g
debug: clean all
	@echo -e "$(GREEN)Debug build ready.$(RESET)"

# Run program
run: all
	@echo -e "$(GREEN)Running ATM...$(RESET)"
	@./$(TARGET)

# Remove build artifacts
clean:
	@echo -e "$(RED)Cleaning build directory...$(RESET)"
	@rm -rf build

# Full rebuild
rebuild: clean all

.PHONY: all clean debug run rebuild
