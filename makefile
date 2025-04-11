# Compiler
CXX = g++

# Compiler flags: 
# -Wall: enables all common warnings.
# -g: includes debugging symbols.
# -std=c++17: compiles using the C++17 standard.
# -pthread: enables POSIX threads.
CXXFLAGS = -Wall -g -std=c++17 -pthread

# List all source files in the project.
SRCS = main.cpp BankSimulation.cpp Teller.cpp Customer.cpp semaphore.cpp

# Generate object files automatically from the source file list.
OBJS = $(SRCS:.cpp=.o)

# Name of the target executable.
TARGET = bank_simulation

# Default target.
all: $(TARGET)

# Link the object files to produce the final executable.
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Generic rule: compile .cpp files to .o object files.
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Remove all generated object files and the executable.
clean:
	rm -f $(OBJS) $(TARGET)

# Prevent make from treating these as files.
.PHONY: all clean
