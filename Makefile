INC=
LIB=-lGL -lglut
CC=gcc -Wall

all: escape

escape: main.o sun.o field.o objects.o
	$(CC) -o escape main.o sun.o field.o objects.o $(LIB)

main.o: main.c sun.h geometry.h field.h objects.h
	$(CC) -c main.c $(INC)

sun.o: sun.h sun.c geometry.h
	$(CC) -c sun.c $(INC)

field.o: field.c field.h geometry.h
	$(CC) -c field.c $(INC)

objects.o: objects.c objects.h geometry.h field.h
	$(CC) -c objects.c $(INC)

clean:
	rm *.o escape
