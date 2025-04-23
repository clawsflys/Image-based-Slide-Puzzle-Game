# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra

# Sources and headers
SOURCES = PGMimage.cpp puzzleGame.cpp
HEADERS = PGMimage.h

# Output binary
TARGET = puzzleGame

# Default target
all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

# Clean build files
clean:
	rm -f $(TARGET)
