#OBJS specifies which files to compile as part of the project
OBJS = main.c

#CC specifies which compiler we're using
CC = gcc

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -ggdb3 -Og -Wall -Wextra -Wpedantic

CFLAGS = -std=c99 $(COMPILER_FLAGS)
LIBS += -lX11

LINKER_FLAGS = -lm

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = xkslags
#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LIBS) $(LINKER_FLAGS) -o $(OBJ_NAME)
