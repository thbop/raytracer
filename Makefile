# Compiler and flags
CXX = g++
CXXFLAGS = -fdiagnostics-color=always -std=c++17 -Wall -I"C:/raylib/w64devkit/include"
LDFLAGS = -L"C:/raylib/w64devkit/lib" -lmingw32 -lSDL2main -lSDL2

# Output executable
TARGET = editor.exe

# Source files
SRC = editor.cpp

# Compilation rules

all: clean $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(LDFLAGS) -o $(TARGET)

# Clean rule
clean:
	rm -f $(TARGET)