CC=gcc
FLAGS=-Wall -g -lm -std=c99
EXEC=test

all: $(EXEC)

test: Lceb_test.o Lceb_chained_list.o Lceb_fonctions.o
		$(CC) -o $@ $^ $(FLAGS)

Lceb_test.o: Lceb_test.c Lceb.h
		$(CC) -c $^ $(FLAGS)

Lceb_fonctions.o: Lceb_fonctions.c Lceb.h
		$(CC) -c $^ $(FLAGS)

Lceb_chained_list.o: Lceb_chained_list.c Lceb.h
		$(CC) -c $^ $(FLAGS)

purge:
		rm -rf $(EXEC)
		rm -rf *.o *.gch

exec:
		rm -rf *.o *.gch
