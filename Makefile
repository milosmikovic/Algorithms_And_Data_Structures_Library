# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++17 -I./DataStructures

# Target executable
TARGET = test_main

# Source files
SRCS = test_main.cpp

# Header files
HEADERS = DataStructures/vector.hpp

# Object files (derived from source files)
OBJS = $(SRCS:.cpp=.o)

# Default rule to build the target
all: $(TARGET)

# Rule to link object files and create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

test_main.o : test_main.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $<

# Clean rule to remove generated files
clean:
	rm -f $(TARGET) $(OBJS)
