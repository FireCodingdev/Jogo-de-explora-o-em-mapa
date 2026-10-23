#include <stdio.h>

#define LINHAS 10
#define COLUNAS 20

// TDA Player
typedef struct {
    char skin;
    int pos[2]; 
    int points;
} player;
//start player
void start(player *p) {
    p->skin = '@';
    p->pos[0] = 1; 
    p->pos[1] = 1;
    p->points = 0;
}
//mapa
char mapa[LINHAS][COLUNAS + 1] = {
    "####################",
    "#.............$....#",
    "#..####............#",
    "#........##........#",
    "#...$..............#",
    "#..............##..#",
    "#..####............#",
    "#..............$...#",
    "#..................#",
    "####################"
};

// Função que mostra a posição do player
void mostrarMapa(player p) {
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            if (i == p.pos[0] && j == p.pos[1]) {
                printf("%c", p.skin); 
            } else {
                printf("%c", mapa[i][j]);
            }
        }
        printf("\n");
    }

    printf("\nItens coletados: %d\n", p.points);
}

int main() {
    player meuJogador;
    
    start(&meuJogador);
    
    mostrarMapa(meuJogador);

    return 0;
}
