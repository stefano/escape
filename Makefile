INC=
LIB=-lGL -lglut
CC=gcc -Wall

all: escape

escape: main.o sun.o
	$(CC) -o escape main.o sun.o $(LIB)

main.o: main.c sun.h geometry.h field.h
	$(CC) -c main.c $(INC)

sun.o: sun.h sun.c geometry.h
	$(CC) -c sun.c $(INC)

clean:
	rm *.o escape
