# Makefile for the smash program
CC = g++
CXXFLAGS = -g -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG
CCLINK = $(CC) 
OBJS = smash.o commands.o signals.o
TARGET = smash
RM = rm -f
# Creating the  executable
smash: $(OBJS)
	$(CCLINK) -o $(TARGET) $(OBJS)

# Creating the object files
commands.o: commands.cpp commands.h Job.h 

smash.o: smash.cpp commands.h signals.h Job.h

signals.o: signals.cpp signals.h Job.h

# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*

