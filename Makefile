# Define compiler and flags
CC = g++
CFLAGS = -Wall -Wextra -std=c++11

# Define source files and object files
SRCS = Blackjack.cpp Card.cpp ErrorHandler.cpp
OBJS = $(SRCS:.cpp=.o)

# Define target executable
TARGET = blackjack

# Rule to link object files into executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Rule to compile source files into object files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)
