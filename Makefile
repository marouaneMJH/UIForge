CC = gcc
CFLAGS = $(shell pkg-config --cflags gtk+-3.0) -I$(INCLUDE_DIR) -g -O0
LDFLAGS = $(shell pkg-config --libs gtk+-3.0)

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

EXECUTABLE = $(BUILD_DIR)/main

# Find all .c files inside SRC_DIR, but manually add main.c
SOURCES = main.c $(shell find $(SRC_DIR) -name "*.c")
OBJECTS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SOURCES))

all: run


# Ensure object files are placed correctly in the build directory
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure the build directory exists
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Build the executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(LDFLAGS)


# Run the program
run: $(EXECUTABLE)
	./$(EXECUTABLE)

# Clean up
clean:
	rm -rf $(BUILD_DIR)
	