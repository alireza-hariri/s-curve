# Define directories
SRC_DIR := src
BUILD_DIR := build

# Define the C++ compiler
CXX := g++

# Define the C++ flags
CXXFLAGS := -g -Wall

# Define the linker flags
LDFLAGS :=

# Define the libraries
LDLIBS :=

# Find all source files in the source directory
SOURCES := $(wildcard $(SRC_DIR)/*.c)

# Create object files in the build directory
OBJECTS := $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCES:.c=.o)))

# Define the target executable
TARGET := main.exe

# Create the build directory if it does not exist
$(shell mkdir -p $(BUILD_DIR) $(BIN_DIR))

# All target
.PHONY: all
all: $(TARGET)

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link object files to create the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Clean target
.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TARGET)
