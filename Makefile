# Compiler
CXX = g++

# Directories
SRCDIR = src
INCDIR = include
BINDIR = bin
LIBDIR = lib

# Target binary
TARGET = $(BINDIR)/core

# Compiler & linker flags
CXXFLAGS = -I$(INCDIR) \
           -I$(SRCDIR) \
           -std=c++17 -Wall -Wextra -MMD -MP
LDFLAGS = -L$(LIBDIR) -lbox2d -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Find all .cpp files under src/
SRCFILES := $(shell find $(SRCDIR) -name '*.cpp') main.cpp

# Convert src/.../file.cpp -> bin/.../file.o
OBJECTS := $(patsubst %.cpp,$(BINDIR)/%.o,$(SRCFILES))

# Default
all: $(TARGET)

# Link
$(TARGET): $(OBJECTS)
	@echo "Linking: $@"
	$(CXX) $^ -o $@ $(LDFLAGS)

# Compile rule — keep directory structure inside bin/
$(BINDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Include dependency files
-include $(OBJECTS:.o=.d)

# Clean
clean:
	@echo "Cleaning build..."
	rm -rf $(BINDIR)/* $(TARGET)

# Run
run: all
	./$(TARGET)

.PHONY: all clean run
