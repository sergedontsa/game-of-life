UNAME = $(shell uname)
CC = g++
SRC = utilities/ConfigFile.cpp main.cpp utilities/GameOfLife.cpp
EXEC = game
# Linux library flags
LDFLAGS	= -lGL -lglut -lGLU
# MacOS library flags
ifeq "$(UNAME)" "Darwin"
	LDFLAGS	= -framework OpenGL -framework GLUT
endif

all: compile run clean

compile: 
	$(CC) $(LDFLAGS) $(SRC) -o $(EXEC)

clean:
	rm -rf $(EXEC) *~

run:
	./$(EXEC) game.config

