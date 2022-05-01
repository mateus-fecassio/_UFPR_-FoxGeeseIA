CC = gcc
CFLAGS = -g -Wall -Wno-unused-result -O3
LDLIBS = -l hiredis -l readline
TARGETS = controlador automatico interativo


.PHONY: default
default: all

.PHONY: all
all: $(TARGETS)

controlador: controlador.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

automatico: tabuleiro.h tabuleiro.c automatico.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

interativo: tabuleiro.h tabuleiro.c interativo.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)


#LIMPEZA
clean:
	@echo "Limpando ...."
	@rm -f *~ *.bak *.tmp

purge:   clean
	@echo "Faxina ...."
	@rm -f *.o core a.out
	@rm -f $(TARGETS)