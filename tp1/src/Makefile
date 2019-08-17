CC=g++

CFLAGS=-c

all: tp1

tp1: main.o matriz.o instancia.o elimgauss.o wp.o
	$(CC) main.o matriz.o instancia.o elimgauss.o wp.o -o tp1

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

matriz.o: ./matriz/matriz.cpp 
	$(CC) $(CFLAGS) ./matriz/matriz.cpp

instancia.o: ./instancia/instancia.cpp
	$(CC) $(CFLAGS) ./instancia/instancia.cpp

elimgauss.o: ./eliminaciongauss/elimgauss.cpp
	$(CC) $(CFLAGS) ./eliminaciongauss/elimgauss.cpp

wp.o: ./wp/wp.cpp
	$(CC) $(CFLAGS) ./wp/wp.cpp

clean:
	rm -rf *o tp1
