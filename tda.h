
#ifndef TDA_H
#define TDA_H

#include <stdio.h>
#include <time.h>

#define LINHAS 23
#define COLUNAS 20

#ifdef _WIN32

    #include <conio.h>
    #define LIMPAR_TELA "cls"

#else

    #include <termios.h>
    #include <unistd.h>
    #define LIMPAR_TELA "clear"

#endif

typedef struct {

    char skin;
    int pos[2];
    int points;

} Player;

typedef struct {

    char nome[TAM_NOME + 1];
    int pontos;
    int segundos;

} Registro;


/* Mapa do jogo */
extern char mapa[LINHAS][COLUNAS + 1];

/* Funcoes do jogador */
void start(Player *p);
void move(Player *p, int y, int x);
void mostrarMapa(Player *p, char mapa[][COLUNAS + 1],
                 int linhas, int colunas);
void coletarItem(Player *p, char mapa[][COLUNAS + 1],
                 int y, int x);
void exibirResultados(Player p, time_t inicio);
char lerTecla(void);

/* Funcoes do ranking */
int carregarRanking(Registro ranking[]);
int salvarRegistro(const char *nome, int pontos, int segundos);
void lerNome(char *destino, int tamanho);
void exibirRanking(void);


/* Funcoes do mapa */
void iniciarMapa(void);
void gerarLinha(void);
void avancarMapa(void);

#endif
