# CHIP-8 Emulator Makefile
# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -I3rdParty/include/SDL2

# Linker flags
LDFLAGS = -L3rdParty/lib -lmingw32 -lSDL2main -lSDL2

# Source files
SOURCES = Source/Main.cpp Source/Chip8.cpp Source/Platform.cpp

# Output executable
TARGET = chip8.exe

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)
	@echo Build complete! Run with: .\chip8.exe 10 1 test_opcode.ch8

# Clean build artifacts
clean:
	del /Q $(TARGET)

# Phony targets
.PHONY: all clean