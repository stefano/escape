INC=
LIB=-lGL -lglut
CC=gcc -Wall

all: escape

escape: main.o
	$(CC) -o escape main.o $(LIB)

main.o: main.c
	$(CC) -c main.c $(INC)

clean:
	rm *.o escape
