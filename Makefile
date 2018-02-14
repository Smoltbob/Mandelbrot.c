CC=clang
CFLAGS=-std=c99 -Wall -Wextra -g
LDFLAGS=-lm
EXEC=tests_couleur
SRC=$(wildcard *.c)
OBJ=$(SRC: .c=.o)

all : tests_couleur test_ppm test_fractale test_parser

tests_couleur : couleur.o tests_couleurs.o
	$(CC) $(LDFLAGS) -o $@ $^ 

test_ppm : ppm.o test_ppm.o couleur.o
	$(CC) $(LDFLAGS) -o $@ $^ 
	
test_fractale: fractale.o ppm.o test_fractale.o couleur.o parser.o
	$(CC) $(LDFLAGS) -o $@ $^ 

test_parser: fractale.o parser.o test_parser.o couleur.o
	$(CC) $(LDFLAGS) -o $@ $^ 

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f *.o core

mrproper: clean
	rm -f tests_couleur test_ppm test_fractale test_ppm
