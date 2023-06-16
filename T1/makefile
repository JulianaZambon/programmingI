CFLAGS = -Wall -std=c90 -g  # flags de compilacao
LDFLAGS = -lm

CC = gcc

# arquivos-objeto
objects = agenda.o libagenda.o 

all: testa_agenda

agenda: $(objects)
	$(CC) -o agenda $(objects) $(LDFLAGS)

libagenda.o: libagenda.c
	$(CC) -c $(CFLAGS) libagenda.c

agenda.o: testa_agenda.c
	$(CC) -c $(CFLAGS) agenda.c

clean:
	rm -f $(objects) agenda