# Makefile for Lecture 11 C++ Tomato App

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
TARGET = tomato_app
SOURCE = main.cpp

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -I. -o $(TARGET) $(SOURCE)

# Run the application
run: $(TARGET)
	./$(TARGET)

# Clean up compiled files
clean:
	rm -f $(TARGET) $(TARGET).exe

# Help target
help:
	@echo "Available targets:"
	@echo "  all    - Build the application (default)"
	@echo "  run    - Build and run the application"
	@echo "  clean  - Remove compiled files"
	@echo "  help   - Show this help message"

.PHONY: all run clean help
