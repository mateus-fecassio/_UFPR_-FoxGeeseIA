# CFLAGS = -g -Wall -Wno-unused-result -O3
CC = gcc
CFLAGS = -g -O3
LDLIBS = -l hiredis -l readline
TARGETS = controlador interativo automatico


.PHONY: default
default: all

.PHONY: all
all: $(TARGETS)

controlador: controlador.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

p_automatico: tabuleiro.h tabuleiro.c automatico.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

p_interativo: tabuleiro.h tabuleiro.c interativo.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)


#LIMPEZA
clean:
	@echo "Limpando ...."
	@rm -f *~ *.bak *.tmp

purge:   clean
	@echo "Faxina ...."
	@rm -f *.o core a.out
	@rm -f $(TARGETS)