CFLAGS = -Wall -std=c90 -g  # flags de compilacao
LDFLAGS = -lm

CC = gcc

# arquivos-objeto
objects = testa_agenda.o libagenda.o 

all: testa_agenda

testa_agenda: $(objects)
	$(CC) -o testa_agenda $(objects) $(LDFLAGS)

libagenda.o: libagenda.c
	$(CC) -c $(CFLAGS) liblista_ordenada.c

testa_agenda.o: testa_agenda.c
	$(CC) -c $(CFLAGS) testa_agenda.c

clean:
	rm -f $(objects) testa_agenda