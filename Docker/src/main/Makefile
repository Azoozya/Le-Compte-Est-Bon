CC=gcc
FLAGS=-Wall -g -lm -std=c99
EXEC=final

all: $(EXEC)

final: Lceb_main.o Lceb_chained_list.o Lceb_fonctions.o
		$(CC) -o $@ $^ $(FLAGS)

Lceb_main.o: Lceb_main.c Lceb.h
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
