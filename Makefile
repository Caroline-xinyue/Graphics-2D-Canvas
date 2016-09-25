# basic Makefile 
# compile with "make <filename without .c>
# Note that if you installed your OpenGL libraries in a different place
# the paths that follow -L in LDFLAGS and -I in CFLAGS will have to change
# accordingly

ifeq ($(shell uname -s), Darwin)
LDFLAGS=-L/opt/ports/lib -framework OpenGL -lglfw -lGLEW -lm
else
LDFLAGS=-lX11 -lGL -lGLU -lglfw -lGLEW -lm
endif
CC = gcc
CFLAGS=-g -Wall -I/opt/ports/include
DEPS = shapes.h
OBJ = shapes.o hw1.o

shapes.o: shapes.c $(DEPS)
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $<
hw1.o: hw1.c $(DEPS)
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $<
hw1: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm *.o hw1
