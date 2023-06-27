CFLAGS = -Wall -std=c90 -g  # flags de compilacao
LDFLAGS =
CC = gcc

# arquivos-objeto
        objects = agenda.o libagenda.o

all: agenda teste


teste: libagenda.o
        $(CC) -o teste agenda_teste.o libagenda.o $(LDFLAGS)

agenda: agenda.o libagenda.o
        $(CC) -o agenda $(objects) $(LDFLAGS)

libagenda.o: libagenda.c libagenda.h
        $(CC) -c $(CFLAGS) libagenda.c

agenda.o: agenda.c
        $(CC) -c $(CFLAGS) agenda.c

clean:
        rm -f $(objects) agenda 
