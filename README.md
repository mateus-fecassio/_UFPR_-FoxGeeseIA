# Raposa e Gansos (uma abordagem por Inteligência Artificial)

Esse projeto é uma implementação do jogo da [Raposa e Gansos](https://www.mastersofgames.com/rules/fox-geese-rules.htm).

## Dependências
Este projeto utiliza o **redis**, que é uma ferramenta de armazenamento de estrutura de dados de chave-valor de código aberto e na memória. O **redis** oferece um conjunto de estruturas versáteis de dados na memória que permite a fácil criação de várias aplicações personalizadas.

Assim, precisamos instalar o **redis**, bem como algumas bibliotecas necessárias para execução do controlador que será responsável por fazer a ponte entre dois jogadores deste projeto.

Em um ambiente Linux, execute:
```
   $ sudo apt install redis-server
   $ sudo apt-get install libreadline-dev
   $ sudo apt-get install libhiredis-dev
```


## Uso
Para compilar todos programas, excute:
```
   $ make all
```

### Preparação do Ambiente de Execução
Para testar o seu jogador você deve iniciar o servidor **redis** e
executar o controlador.

Para iniciar o servidor **redis** execute em um
terminal:

```
   $ redis-server redis.conf
```

Para iniciar o controlador execute em outro terminal:
```
   $ ./controlador r 50 0
```
Este comando inicia o controlador indicando que o jogador que começa é
o 'r' (raposa), a partida terá 50 movimentos e não há limite de tempo
para as jogadas. Para mais detalhes sobre os parâmetros do controlador
execute:

```
   $ ./controlador
```

### Jogadores
Para conectar duas instâncias de jogadores ao controlador, uma de cada
lado do tabuleiro, execute em outros dois terminais os comandos:

```
   $ ./interativo r
```
```
   $ ./interativo g
```
O uso dessas duas instâncias permite que dois jogadores (humanos rs) joguem um contra o outro. Para mais detalhes sobre a forma de jogabilidade, o arquivo de especificação do projeto poderá ajudar. Este aquivo está presente em arquivos/especificação.pdf. .

Para conectar duas instâncias de jogadores automáticos, uma de cada
lado do tabuleiro, execute em outros dois terminais os comandos:

```
   $ ./automatico r
```
```
   $ ./automatico g
```