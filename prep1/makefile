CFLAGS = -Wall -std=c90 -g  # flags de compilacao
LDFLAGS = -lm

CC = gcc

# arquivos-objeto
objects = testa_lista_ordenada.o liblista_ordenada.o 

all: testa_lista_ordenada

testa_lista_ordenada: $(objects)
	$(CC) -o testa_lista_ordenada $(objects) $(LDFLAGS)

liblista_ordenada.o: liblista_ordenada.c
	$(CC) -c $(CFLAGS) liblista_ordenada.c

testa_lista_ordenada.o: testa_lista_ordenada.c
	$(CC) -c $(CFLAGS) testa_lista_ordenada.c

clean:
	rm -f $(objects) testa_lista_ordenada

