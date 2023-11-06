
# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17

# Source files
SRCS = $(wildcard *.cpp)

# Object files
OBJS = $(addprefix obj/,$(notdir $(SRCS:.cpp=.o)))

# Target executable
TARGET = raytracer

# Rule to compile object files
obj/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to link object files into executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Rule to create obj directory if it doesn't exist
obj:
	mkdir obj

# Default rule
all: obj $(TARGET)

# Clean rule
clean:
	rm -rf obj $(TARGET)
