#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabuleiro.h"

#define MAXSTR 512
#define MAXINT 16

int main(int argc, char **argv) {
  char buf[MAXSTR];
  char tabuleiro[MAXSTR]; 
  char lado_meu;
  char lado_adv;
  char tipo_mov_adv;
  int num_mov_adv;
  int mov_adv_l[MAXINT];
  int mov_adv_c[MAXINT];
  int i;

  // conecta com o controlador do campo
  tabuleiro_conecta(argc, argv);

  // recebe o campo inicial e o movimento do adversario
  tabuleiro_recebe(buf);

  // separa os elementos do string recebido
  sscanf(strtok(buf, " \n"), "%c", &lado_meu);
  sscanf(strtok(NULL, " \n"), "%c", &lado_adv);
  sscanf(strtok(NULL, " \n"), "%c", &tipo_mov_adv);
  if(tipo_mov_adv == 'm') {
    num_mov_adv = 2;
    for(i = 0; i < num_mov_adv; i++) {
      sscanf(strtok(NULL, " \n"), "%d", &(mov_adv_l[i]));
      sscanf(strtok(NULL, " \n"), "%d", &(mov_adv_c[i]));
    }
  }
  else if(tipo_mov_adv == 's') {
    sscanf(strtok(NULL, " \n"), "%d", &num_mov_adv);
    for(i = 0; i < num_mov_adv; i++) {
      sscanf(strtok(NULL, " \n"), "%d", &(mov_adv_l[i]));
      sscanf(strtok(NULL, " \n"), "%d", &(mov_adv_c[i]));
    }
  }
  strncpy(tabuleiro, strtok(NULL, "."), MAXSTR);

  // mostra o que recebeu
  printf("%c\n", lado_meu);
  printf("%c %c", lado_adv, tipo_mov_adv);
  if(tipo_mov_adv != 'n') {
    if(tipo_mov_adv == 's') 
      printf(" %d", num_mov_adv);
    for(i = 0; i < num_mov_adv; i++) {
      printf(" %d", mov_adv_l[i]);
      printf(" %d", mov_adv_c[i]);
    }
  }
  printf("\n");
  printf("%s", tabuleiro);
  
  // prepara um string com o movimento
  if(lado_meu == 'r')
    sprintf(buf, "r m 5 4 5 3\n");
  else
    sprintf(buf, "g m 3 1 4 1\n");

  // envia o movimento para o controlador do campo
  tabuleiro_envia(buf);  
}
