CC = gcc
CFLAGS = -Wall -Wextra -std=c99

agenda: agenda.o libagenda.o
	$(CC) $(CFLAGS) -o agenda agenda.o libagenda.o

agenda.o: agenda.c libagenda.h
	$(CC) $(CFLAGS) -c agenda.c

libagenda.o: libagenda.c libagenda.h
	$(CC) $(CFLAGS) -c libagenda.c

clean:
	rm -f agenda agenda.o libagenda.o