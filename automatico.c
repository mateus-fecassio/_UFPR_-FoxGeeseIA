#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tabuleiro.h"


#define MAXSTR 512
#define MAXINT 16
#define MAXTAB 10

//---------------------------------------VARIÁVEIS GLOBAIS
int GANSOS = 13; //armazena a quantidade de gansos atuais
//------------------------------------------

//---------------------------------------ESTRUTURA DE DADOS
typedef struct tGanso {
    int x;
    int y;
} tGanso;
//---------------------------------------


//---------------------------------------ESCOPO DE FUNÇÕES
void tabuleiroToMatriz(char tabuleiro[MAXSTR], char matriz[MAXTAB][MAXTAB]);

void getPosRaposa(char matriz[MAXTAB][MAXTAB], int *rapX, int *rapY);
void jogadaRaposa(char matriz[MAXTAB][MAXTAB], char *buf);


void getPosGansos(char matriz[MAXTAB][MAXTAB], tGanso *gansos);
void printPosGansos(tGanso *gansos);
int possPerderGanso(char matriz[MAXTAB][MAXTAB], int x, int y);
int verificarPerigo(char matriz[MAXTAB][MAXTAB], tGanso *gansos);

//---------------------------------------


//---------------------------------------FUNÇÕES IMPLEMENTADAS
//AUXILIAR:transforma um tabuleiro numa matriz
void tabuleiroToMatriz(char tabuleiro[MAXSTR], char matriz[MAXTAB][MAXTAB]) { 
    int i, j, k = 0;

    for (i = 0; i < MAXTAB; ++i) {
        for (j = 0; j < MAXTAB; ++j) {
            matriz[i][j] = tabuleiro[k];
            k++;
        }
    }
} //FINALIZADO


//RAPOSA:pega a posição de uma raposa
void getPosRaposa(char matriz[MAXTAB][MAXTAB], int *rapX, int *rapY) {
    int i, j;

    for (i = 0; i < MAXTAB; ++i) {
        for (j = 0; j < MAXTAB; ++j) {
            if (matriz[i][j] == 'r') {
                *rapX = i;
                *rapY = j;
            }
        }
    }
} //FINALIZADO

void jogadaRaposa(char matriz[MAXTAB][MAXTAB], char *buf) {
    int rapX, rapY, r;

    getPosRaposa(matriz, &rapX, &rapY);

    if ((matriz[rapX-1][rapY] == 'g') && (matriz[rapX-2][rapY]) == '-') { //come ganso acima
        sprintf(buf, "r m %d %d %d %d\n", rapX, rapY, rapX-2, rapY);
    }
    else if ((matriz[rapX+1][rapY] == 'g') && (matriz[rapX+2][rapY]) == '-') { //come ganso abaixo
        sprintf(buf, "r m %d %d %d %d\n", rapX, rapY, rapX+2, rapY);
    }
    else if ((matriz[rapX][rapY-1] == 'g') && (matriz[rapX][rapY-2]) == '-') { //come ganso à esquerda
        sprintf(buf, "r m %d %d %d %d\n", rapX, rapY, rapX, rapY-2);
    }
    else if ((matriz[rapX][rapY+1] == 'g') && (matriz[rapX][rapY+2]) == '-') { //come ganso à direita
        sprintf(buf, "r m %d %d %d %d\n", rapX, rapY, rapX, rapY+2);
    }

    //se não tem nenhuma raposa para comer ao seu redor
    else {
        r = rand() % 4;
        switch(r) {
            case 0:
            if ((matriz[rapX-1][rapY] == '-')) // move para cima
                sprintf(buf, "r m %d %d %d %d\n", rapX, rapY, rapX-1, rapY);
            break;
            
            case 1:
            if ((matriz[rapX+1][rapY] == '-')) // move para baixo
                sprintf(buf, "r m %d %d %d %d\n", rapX, rapY, rapX+1, rapY);
            break;
            
            case 2:
            if ((matriz[rapX][rapY-1] == '-')) // move para a esquerda
                sprintf(buf, "r m %d %d %d %d\n", rapX, rapY, rapX, rapY-1);
            break;
            
            case 3:
            if ((matriz[rapX][rapY+1] == '-')) // move para a direita
                sprintf(buf, "r m %d %d %d %d\n", rapX, rapY, rapX, rapY+1);
            break;
        }
    }
} //FINALIZADO




//GANSO: pega as posições de todos os gansos
void getPosGansos(char matriz[MAXTAB][MAXTAB], tGanso *gansos) {
    int i, j, k = 0;

    for (i = 0; i < MAXTAB; ++i) {
        for (j = 0; j < MAXTAB; ++j) {
            if (matriz[i][j] == 'g') {
                gansos[k].x = i;
                gansos[k].y = j;
                k++;
            }
        }
    }
    GANSOS = k; //atualiza o total de gansos
} //FINALIZADO

//GANSO: printa a posição de todos os gansos
void printPosGansos(tGanso *gansos) {
    int i;

    // printf("ID: X Y\n");
    for (i = 0; i < GANSOS; ++i) {
        printf("GANSO %d: %d %d\n", i, gansos[i].x, gansos[i].y);
    }
} //FINALIZADO

//GANSO: determina para uma posição x,y se há a possibilidade de um ganso ser comido
int possPerderGanso(char matriz[MAXTAB][MAXTAB], int x, int y) {
    if ((matriz[x][y+1] == 'r') && (matriz[x][y-1] == '-')) //come para a esquerda
        return 1;
    else if ((matriz[x][y-1] == 'r') && (matriz[x][y+1] == '-')) //come para a direita
        return 1;
    else if ((matriz[x-1][y] == 'r') && (matriz[x+1][y] == '-')) //come para cima
        return 1;
    else if ((matriz [x+1][y] == 'r') && (matriz[x-1][y] == '-')) //come para baixo
        return 1;
    else
        return 0;
} //FINALIZADO

//GANSO: verifica se existe algum ganso da lista em perigo, se sim, devolve qual
int verificarPerigo(char matriz[MAXTAB][MAXTAB], tGanso *gansos) {
    int i;

    for (i = 0; i < GANSOS; ++i) {
        if (possPerderGanso(matriz, gansos[i].x, gansos[i].y))
            return i;
    }
    return -1;
} //FINALIZADO

//GANSO: jogada aleatória
void jogadaGanso(char matriz[MAXTAB][MAXTAB], char *buf) {
    int i,j,g,m = 0;
    int jogada = 0;
    int tentativasGansos = 0;

    tGanso *gansos = (tGanso *) malloc(GANSOS * sizeof(tGanso));
    getPosGansos(matriz, gansos);

    //se existir um ganso em perigo
    if ((g = verificarPerigo(matriz, gansos)) != -1) {
        i = gansos[g].x;
        j = gansos[g].y;

        if (matriz[i-1][j] == 'r') { // se a raposa está acima
            //achar um ganso que possa ajudar a bloquear a raposa
            if (matriz[i+1][j-1] == 'g')
                sprintf(buf, "g m %d %d %d %d", i+1, j-1, i+1, j);
            if (matriz[i+2][j] == 'g')
                sprintf(buf, "g m %d %d %d %d", i+2, j, i+1, j);
            if (matriz[i+1][j+1] == 'g')
                sprintf(buf, "g m %d %d %d %d", i+1, j+1, i+1, j);
        }
        else if (matriz[i+1][j] == 'r') { // se a raposa está abaixo
            //achar um ganso que possa ajudar a bloquear a raposa
            if (matriz[i-1][j-1] == 'g')
                sprintf(buf, "g m %d %d %d %d", i-1, j-1, i-1, j);
            if (matriz[i-2][j] == 'g')
                sprintf(buf, "g m %d %d %d %d", i-2, j, i-1, j);
            if (matriz[i-1][j+1] == 'g')
                sprintf(buf, "g m %d %d %d %d", i-1, j+1, i-1, j);
        }
        else if (matriz[i][j-1] == 'r') { // se a raposa está à esquerda
            //achar um ganso que possa ajudar a bloquear a raposa
            if (matriz[i-1][j+1] == 'g')
                sprintf(buf, "g m %d %d %d %d", i-1, j+1, i, j+1);
            if (matriz[i][j+2] == 'g')
                sprintf(buf, "g m %d %d %d %d", i, j+2, i, j+1);
            if (matriz[i+1][j+1] == 'g')
                sprintf(buf, "g m %d %d %d %d", i+1, j+1, i, j+1);
        }
        else if (matriz[i][j+1] == 'r') { // se a raposa está à direita
            //achar um ganso que possa ajudar a bloquear a raposa
            if (matriz[i-1][j-1] == 'g')
                sprintf(buf, "g m %d %d %d %d", i-1, j-1, i, j-1);
            if (matriz[i][j-2] == 'g')
                sprintf(buf, "g m %d %d %d %d", i, j-2, i, j-1);
            if (matriz[i+1][j-1] == 'g')
                sprintf(buf, "g m %d %d %d %d", i+1, j-1, i, j-1);
        }
        else { //não encontrou nenhuma outra raposa para ajudar a bloquear, então, o ganso em perigo tenta se salvar
            if (matriz[i][j-1] == '-') // vai para a esquerda
                sprintf(buf, "g m %d %d %d %d", i, j, i, j-1);
            if (matriz[i][j+1] == '-') // vai para a direita
                sprintf(buf, "g m %d %d %d %d", i, j, i, j+1);
            if (matriz[i-1][j] == '-') // vai para cima
                sprintf(buf, "g m %d %d %d %d", i, j, i-1, j);
            if (matriz[i+1][j] == '-') // vai para baixo
                sprintf(buf, "g m %d %d %d %d", i, j, i+1, j);
        }

    } else { //se não existir um ganso em perigo, escolhe um ganso a ser movido
        m = rand() % GANSOS; // escolhe um ganso aleatório a ser movido

        while ((tentativasGansos < GANSOS) && (!jogada)) {
            //move para baixo
            if ((matriz[gansos[m].x+1][gansos[m].y] == '-') && !possPerderGanso(matriz, gansos[m].x+1, gansos[m].y)) {
                sprintf(buf, "g m %d %d %d %d", gansos[m].x, gansos[m].y, gansos[m].x+1, gansos[m].y);
                jogada = 1;
            }
            //move para a cima
            else if ((matriz[gansos[m].x-1][gansos[m].y] == '-') && !possPerderGanso(matriz, gansos[m].x-1, gansos[m].y)) {
                sprintf(buf, "g m %d %d %d %d", gansos[m].x, gansos[m].y, gansos[m].x-1, gansos[m].y);
                jogada = 1;
            }
            //move para a direita
            else if ((matriz[gansos[m].x][gansos[m].y+1] == '-') && !possPerderGanso(matriz, gansos[m].x, gansos[m].y+1)) {
                sprintf(buf, "g m %d %d %d %d", gansos[m].x, gansos[m].y, gansos[m].x, gansos[m].y+1);
                jogada = 1;
            }
            //move para a esquerda
            else if ((matriz[gansos[m].x][gansos[m].y-1] == '-') && !possPerderGanso(matriz, gansos[m].x, gansos[m].y-1)) {
                sprintf(buf, "g m %d %d %d %d", gansos[m].x, gansos[m].y, gansos[m].x, gansos[m].y-1);
                jogada = 1;
            }
            else { //testa as possibilidades com outro ganso
                m = (m+1) % GANSOS; 
            }
            tentativasGansos++;
        }
    }
    // printaPosGansos(gansos);
    free(gansos);
} //FINALIZADO

//---------------------------------------


//------------------------------------------MAIN
int main(int argc, char **argv) {
    char buf[MAXSTR];
    char tabuleiro[MAXSTR]; 
    char lado_meu;
    char lado_adv;
    char tipo_mov_adv;
    int num_mov_adv;
    int mov_adv_l[MAXINT];
    int mov_adv_c[MAXINT];
    char matriz[MAXTAB][MAXTAB];
    int i,j;

    // conecta com o controlador do campo
    tabuleiro_conecta(argc, argv);
    srand(time(NULL));


    while(1) {
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

        tabuleiroToMatriz(tabuleiro, matriz);
        printf("\n");
        printf("%s", tabuleiro);

        printf("mat:");
        for (i = 0 ; i < MAXTAB ; i++){
            for (j = 0 ; j < MAXTAB ; j++){
                printf ("%c", matriz[i][j]);
            }
        }

        // prepara um string com o movimento
        if (lado_meu == 'r')
            jogadaRaposa(matriz,buf);
        else
            jogadaGanso(matriz,buf);

        // envia o movimento para o controlador do campo
        printf("JOGADA: %s", buf);
        tabuleiro_envia(buf);
    }
}