#include <stdio.h>

#define LINHAS 10
#define COLUNAS 20

// TDA Player
typedef struct {
    char skin;
    int pos[2]; 
    int points;
} player;

void start(player *p) {
    p->skin = '@';
    p->pos[0] = 1;
    p->pos[1] = 1;
    p->points = 0;
}

void move(player *p, int y, int x) {
    p->pos[0] = y; 
    p->pos[1] = x; 
}

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
    player player1;
    char tecla;
    
    start(&player1);
    
    while(1) {
        mostrarMapa(player1);
        printf("\nMover (WASD) | Sair (Q): ");
        scanf(" %c", &tecla);

        // posições para os calculos
        int proxY = player1.pos[0];
        int proxX = player1.pos[1];

        switch (tecla) {
            case 'w': case 'W': proxY--; break;
            case 's': case 'S': proxY++; break; 
            case 'a': case 'A': proxX--; break; 
            case 'd': case 'D': proxX++; break;
            case 'q': case 'Q': 
                printf("Saindo do jogo...\n");
                return 0;
            default:
                continue; // Ignora teclas inválidas
        }
        // verificar se chegou na parede
        if (mapa[proxY][proxX] != '#') {
            // verificar se é um item e suas ações
            if (mapa[proxY][proxX] == '$') {
                player1.points++;
                mapa[proxY][proxX] = '.'; 
            }
            
            // atualiza a posição na matriz do mapa
            move(&player1, proxY, proxX);

        }
    }
    return 0;
}
