# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17

# Directories
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SRC = string_utils.cpp string_utils_tests.cpp

# Object files
OBJS = $(OBJ_DIR)/string_utils.o $(OBJ_DIR)/string_utils_tests.o

# Executable
TEST_EXEC = $(BIN_DIR)/teststrutils

# Default target
default: $(TEST_EXEC)

# Create directories if they don’t exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Compile string utils and tests
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link object files to create teststrutils executable
$(TEST_EXEC): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

# Run the test executable
run: $(TEST_EXEC)
	./$(TEST_EXEC)
	
# Clean up directories
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)