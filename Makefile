CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O2
TARGET = test
SOURCES = AVL.c test.c
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Compile source files to object files
%.o: %.c AVL.h
	$(CC) $(CFLAGS) -c $< -o $@

# Run the test program
run: $(TARGET)
	./$(TARGET)

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)

# Debug build with extra flags
debug: CFLAGS += -DDEBUG -fsanitize=address
debug: $(TARGET)

# Phony targets
.PHONY: all clean run debug
