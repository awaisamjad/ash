# Compilers
CC = gcc
MINGW_CC = x86_64-w64-mingw32-gcc

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin

# Output executables
TARGET = $(BIN_DIR)/ash
TARGET_WINDOWS = $(BIN_DIR)/ash_windows.exe

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)
OBJS_WINDOWS = $(SRCS:$(SRC_DIR)/%.c=$(BIN_DIR)/%.windows.o)

# Compiler flags
CFLAGS = -I$(INCLUDE_DIR) -Wall -Wextra
MINGW_CFLAGS = -I$(INCLUDE_DIR) -Wall -Wextra

# Linker flags
LDFLAGS =

# Default target
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Rule to compile source files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to link the Windows executable
$(TARGET_WINDOWS): $(OBJS_WINDOWS)
	@mkdir -p $(BIN_DIR)
	$(MINGW_CC) $(OBJS_WINDOWS) -o $(TARGET_WINDOWS) $(LDFLAGS)

# Rule to compile source files for Windows
$(BIN_DIR)/%.windows.o: $(SRC_DIR)/%.c
	@mkdir -p $(BIN_DIR)
	$(MINGW_CC) $(MINGW_CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(BIN_DIR)/*.o $(BIN_DIR)/*.windows.o $(TARGET) $(TARGET_WINDOWS)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Build and run the Windows program
windows: $(TARGET_WINDOWS)

windows_run: $(TARGET_WINDOWS)
	./$(TARGET_WINDOWS)

# Phony targets
.PHONY: all clean run windows windows_run
