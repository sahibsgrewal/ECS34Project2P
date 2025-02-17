\CC = gcc
CXX = g++
INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
TEST_SRC_DIR = ./testsrc
CXXFLAGS = -std=c++17 -I$(INC_DIR) -Wall

LDFLAGS = -lexpat -lgtest_main -lgtest -lpthread

all: directories runtests

runtests: $(BIN_DIR)/teststrutils $(BIN_DIR)/teststrdatasource $(BIN_DIR)/teststrdatasink $(BIN_DIR)/testdsv $(BIN_DIR)/testxml
	@for test in $^; do $$test; done

# Object files
OBJECTS = $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/DSVWriter.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/XMLWriter.o

# Test executables - added proper indentation for commands
$(BIN_DIR)/teststrutils: $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringUtilsTest.o
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BIN_DIR)/teststrdatasource: $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSourceTest.o
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BIN_DIR)/teststrdatasink: $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSinkTest.o
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BIN_DIR)/testdsv: $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/DSVWriter.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/DSVTest.o
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BIN_DIR)/testxml: $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/XMLTest.o
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile source and test object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/%.h
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

directories:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)