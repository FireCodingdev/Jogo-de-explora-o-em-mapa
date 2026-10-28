#ifndef TDA_H
#define TDA_H

#include <stdio.h>
#include <time.h>

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


/* Inicializa o jogador */
void start(Player *p) {

    p->skin = '@';
    p->pos[0] = 1;
    p->pos[1] = 1;
    p->points = 0;
}


/* Move o jogador */
void move(Player *p, int y, int x) {

    p->pos[0] = y;
    p->pos[1] = x;
}


/* Mostra o mapa e o jogador */
void mostrarMapa(Player *p, char mapa[][21],
                 int linhas, int colunas) {

    for (int i = 0; i < linhas; i++) {

        for (int j = 0; j < colunas; j++) {

            if (i == p->pos[0] &&
                j == p->pos[1]) {

                printf("%c", p->skin);

            } else {

                printf("%c", mapa[i][j]);
            }
        }

        printf("\n");
    }

    printf("\nItens coletados: %d\n", p->points);
}


/* Coleta item */
void coletarItem(Player *p, char mapa[][21],
                 int y, int x) {

    if (mapa[y][x] == '$') {

        p->points++;

        mapa[y][x] = '.';
    }
}


/* Exibe os resultados */
void exibirResultados(Player p, time_t inicio) {

    time_t fim = time(NULL);

    int totalSegundos =
        (int)difftime(fim, inicio);

    int minutos = totalSegundos / 60;
    int segundos = totalSegundos % 60;

    printf("\n===== RESULTADOS DA PARTIDA =====\n");

    printf("Itens coletados: %d\n", p.points);

    printf("Tempo de partida: %02d:%02d\n",
           minutos, segundos);

    printf("=================================\n");
}


/* Lê uma tecla sem precisar apertar Enter */
char lerTecla(void) {

#ifdef _WIN32

    return _getch();

#else

    struct termios antigo;
    struct termios novo;

    tcgetattr(STDIN_FILENO, &antigo);

    novo = antigo;

    novo.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO,
              TCSANOW,
              &novo);

    char c = getchar();

    tcsetattr(STDIN_FILENO,
              TCSANOW,
              &antigo);

    return c;

#endif
}

#endif
