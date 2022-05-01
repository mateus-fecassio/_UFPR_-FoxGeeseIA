#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "tabuleiro.h"

#define MAXSTR 512

int main(int argc, char **argv) {
  char buf[MAXSTR];
  char *linha;
  
  tabuleiro_conecta(argc, argv);

  while(1) {
    tabuleiro_recebe(buf);
    printf("%s", buf);
    linha = readline(NULL);
    if(linha[0] == '0')
      break;
    sprintf(buf, "%s\n", linha);
    free(linha);
    tabuleiro_envia(buf);
  }
}