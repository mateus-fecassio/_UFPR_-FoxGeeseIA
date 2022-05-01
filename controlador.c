#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis/hiredis.h>

#define MAXSTR 512
#define MAXINT 16
#define OUTRO(l) ((l) == 'r' ? 'g' : 'r')
#define POS(l,c) ((l) * 10 + (c))
#define ABS(x) ((x < 0)?(-(x)):(x))

redisContext* inicia(int argc, char **argv, char *lado, int *jogadas, char *tempo) {
  redisContext *rediscontext;
  char *ip;
  int porta;
  
  if(argc < 4) {
    printf("formato:\n");
    printf("         %s lado jogadas tempo [ip porta]\n\n", argv[0]);
    printf("   lado: indica que lado inicia o jogo, os valores possívies são r ou g\n");
    printf("   jogadas: número máximo do jogadas na partida\n");
    printf("   tempo: limite em segundos para cada jogada, 0 indica sem limite\n");
    printf("   ip: parâmetro opcional que indica o ip ou o hostname do servidor redis\n");
    printf("       o valor default é 127.0.0.1\n");
    printf("   porta: parâmetro opcional que indica a porta do servidor redis\n");
    printf("          o valor default é 10001\n");
    exit(1);
  }
  
  *lado = argv[1][0];
  *jogadas = atoi(argv[2]);
  strcpy(tempo, argv[3]);
  ip = (argc > 4) ? argv[4] : "127.0.0.1";
  porta = (argc > 5) ? atoi(argv[5]) : 10001;
  
  rediscontext = redisConnect(ip, porta);
  if (rediscontext == NULL || rediscontext->err) {
    if(rediscontext) {
      printf("Erro ao conectar com o servidor redis: %s\n", rediscontext->errstr);
      exit(1);
    } else {
      printf("Não foi possível conectar com o servidor redis\n");
    }
  }
  return rediscontext;
}  

int pos_valida(int l, int c) {
  if(l < 1 || l > 7 || c < 1 || c > 7)
    return 0;
  if((l < 3 || l > 5) && (c < 3 || c > 5))
    return 0;
  return 1;
}

int parse(char *jogada, char *lado, char *tipo,
	  int *num_mov, int *mov_l, int *mov_c) {
  int i, p;
  char *s;

  if(!(s = strtok(jogada, " \n")) || sscanf(s , "%c", lado) != 1)
    return 0;
  if(*lado != 'g' && *lado != 'r')
    return 0;
  if(!(s = strtok(NULL, " \n")) || sscanf(s, "%c", tipo) != 1)
    return 0;
  if(*tipo != 'n' && *tipo != 'm' && *tipo != 's')
    return 0;
  if(*tipo == 'm') {
    *num_mov = 2;
    for(i = 0; i < *num_mov; i++) {
      if(!(s = strtok(NULL, " \n")) || sscanf(s, "%d", &(mov_l[i])) != 1)
	return 0;
      if(!(s = strtok(NULL, " \n")) || sscanf(s, "%d", &(mov_c[i])) != 1)
	return 0;
    }
  }
  else {
    if(*lado == 'g')
      return 0;
    if(!(s = strtok(NULL, " \n")) || sscanf(s, "%d", num_mov) != 1)
      return 0;
    if(*num_mov < 2)
      return 0;
    for(i = 0; i < *num_mov; i++) {
      if(!(s = strtok(NULL, " \n")) || sscanf(s, "%d", &(mov_l[i])) != 1)
	return 0;
      if(!(s = strtok(NULL, " \n")) || sscanf(s, "%d", &(mov_c[i])) != 1)
	return 0;
    }
  }    
  p = 0;
  p += sprintf(&(jogada[p]), "%c %c", *lado, *tipo);
  if(*tipo == 's')
    p += sprintf(&(jogada[p]), " %d", *num_mov);
  for(i = 0; i < *num_mov; i++)
    p += sprintf(&(jogada[p]), " %d %d", mov_l[i], mov_c[i]);
  return 1;
}

int aplica(char *buf, char *tabuleiro, char lado, char tipo,
	   int num_mov, int *mov_l, int *mov_c) {
  int i, l, c, p, ln, cn, pn, dist;

  strcpy(buf, tabuleiro);
  if(tipo == 'n')
    return 1;
  l = mov_l[0];
  c = mov_c[0];
  if(!pos_valida(l,c))
    return 0;
  p = POS(l,c);
  if(tipo == 'm') {
    if(buf[p] != lado)
      return 0;
    buf[p] = '-';
    ln = mov_l[1];
    cn = mov_c[1];
    if(!pos_valida(ln,cn))
      return 0;
    pn = POS(ln,cn);
    dist = ln + cn - l - c;
    dist = ABS(dist);
    if((buf[pn] != '-') || // destino livre
       ((ln != l) && (cn != c)) || // movimento na linha ou coluna
       (dist != 1)) // movimento simples de 1 passo
      return 0;
    buf[pn] = lado;
  }
  else { // tipo s
    if((lado != 'r') || (buf[p] != 'r'))
      return 0;
    for(i = 1; i < num_mov; i++) {
      buf[p] = '-';
      ln = mov_l[i];
      cn = mov_c[i];
      if(!pos_valida(ln,cn))
	return 0;
      pn = POS(ln,cn);
      dist = ln + cn - l - c;
      dist = ABS(dist);
      if((buf[pn] != '-') || // destino livre
	 ((ln != l) && (cn != c)) || // movimento na linha ou coluna
	 (dist != 2)) // movimento de salto, 2 passos
	return 0;
      l = (l + ln) / 2; // posicao do ganso que sera saltado
      c = (c + cn) / 2;
      p = POS(l,c);
      if(buf[p] != 'g')
	return 0;
      buf[p] = '-';
      buf[pn] = 'r';
      l = ln;
      c = cn;
      p = pn;
    }
  }
  return 1;
}

int vitoria(char lado, char *tabuleiro) {
  int l, c, ng;

  if(lado == 'r') {
    ng = 0;
    for(l = 1; l < 8; l++)
      for(c = 1; c < 8; c++)
	if(tabuleiro[POS(l,c)] == 'g')
	  ng++;
    if(ng > 3)
      return 0;
    return 1;
  }
  for(l = 1; l < 8; l++)
    for(c = 1; c < 8; c++)
      if(tabuleiro[POS(l,c)] == 'r') {
	if(pos_valida(l-1,c) && tabuleiro[POS(l-1,c)] == '-')
	  return 0;
	if(pos_valida(l+1,c) && tabuleiro[POS(l+1,c)] == '-')
	  return 0;
	if(pos_valida(l,c-1) && tabuleiro[POS(l,c-1)] == '-')
	  return 0;
	if(pos_valida(l,c+1) && tabuleiro[POS(l,c+1)] == '-')
	  return 0;
	return 1;
      }
  return 0;
}

int main(int argc, char **argv) {
  redisContext *c;
  redisReply *r;
  char buffer[MAXSTR];
  char jogada[MAXSTR];
  char key[16];
  char timeout[16];
  char quem_joga;
  char vencedor;
  char lado;
  char tipo_mov;
  int num_mov;
  int mov_l[MAXINT];
  int mov_c[MAXINT];
  int num_jogadas;
  int ok;
  char tabuleiro[MAXSTR] =
    "#########\n"
    "#  ggg  #\n"
    "#  ggg  #\n"
    "#ggggggg#\n"
    "#-------#\n"
    "#---r---#\n"
    "#  ---  #\n"
    "#  ---  #\n"
    "#########\n";
    
  c = inicia(argc, argv, &quem_joga, &num_jogadas, timeout);

  vencedor = ' ';
  
  printf("%d:\n%s", num_jogadas, tabuleiro);

  sprintf(buffer, "%c\n%c n\n%s", quem_joga, OUTRO(quem_joga), tabuleiro);

  while(num_jogadas) {
    sprintf(key, "tabuleiro_%c", quem_joga);
    r = redisCommand(c, "LTRIM %s 1 0", key);
    freeReplyObject(r);
    r = redisCommand(c, "RPUSH %s %s", key, buffer);
    freeReplyObject(r);
    
    ok = 0;
    sprintf(key, "jogada_%c", quem_joga);
    r = redisCommand(c, "BLPOP %s %s", key, timeout);
    if(r->type != REDIS_REPLY_NIL) {
      strcpy(jogada, r->element[1]->str);
      if(parse(jogada, &lado, &tipo_mov, &num_mov, mov_l, mov_c) &&
	 quem_joga == lado &&
	 aplica(buffer, tabuleiro, lado, tipo_mov, num_mov, mov_l, mov_c)) {
	strcpy(tabuleiro, buffer);
	ok = 1;
      }
    }
    freeReplyObject(r);
    if(!ok)
      sprintf(jogada, "%c n", quem_joga);

    printf("%d: %s\n", num_jogadas, jogada);
    printf("%s", tabuleiro);

    if(vitoria(quem_joga, tabuleiro)) {
      printf("%d: vitória de %c\n", num_jogadas, quem_joga);
      vencedor = quem_joga;
      break;
    }
    
    quem_joga = OUTRO(quem_joga);
    sprintf(buffer, "%c\n%s\n%s", quem_joga, jogada, tabuleiro);
    num_jogadas--;
  }

  sprintf(buffer, "r\ng n\n%s", tabuleiro);
  r = redisCommand(c, "RPUSH tabuleiro_r %s", buffer);
  freeReplyObject(r);

  sprintf(buffer, "g\nr n\n%s", tabuleiro);
  r = redisCommand(c, "RPUSH tabuleiro_g %s", buffer);
  freeReplyObject(r);

  if(num_jogadas == 0)
    printf("empate\n");
  else    
    printf("vencedor: %c\n", vencedor);
}
