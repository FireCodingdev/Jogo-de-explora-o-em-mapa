#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LINHAS 10
#define COLUNAS 20

// TDA Player
typedef struct {
    char skin;
    int pos[2];
    int points;
} Player;

char mapa[LINHAS][COLUNAS + 1];

// Inicializa o jogador
void start(Player *p) {
    p->skin = '@';
    p->pos[0] = 1;
    p->pos[1] = 1;
    p->points = 0;
}

// Inicializa o mapa original
void iniciarMapa(void) {
    char inicial[LINHAS][COLUNAS + 1] = {
        "####################",
        "#.............$....#",
        "#..####............#",
        "#........##........#",
        "#...$..............#",
        "#..............##..#",
        "#..####............#",
        "#..............$...#",
        "#..................#",
        "#..................#"
    };

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j <= COLUNAS; j++) {
            mapa[i][j] = inicial[i][j];
        }
    }
}

// Gera uma nova linha aleatória na parte inferior
void gerarLinha(void) {
    mapa[LINHAS - 1][0] = '#';
    mapa[LINHAS - 1][COLUNAS - 1] = '#';

    for (int j = 1; j < COLUNAS - 1; j++) {
        int sorteio = rand() % 100;

        if (sorteio < 20) {
            mapa[LINHAS - 1][j] = '#'; // 20% de chance
        } else if (sorteio < 30) {
            mapa[LINHAS - 1][j] = '$'; // 10% de chance
        } else {
            mapa[LINHAS - 1][j] = '.'; // 70% de chance
        }
    }

    // Garante pelo menos uma passagem livre
    mapa[LINHAS - 1][COLUNAS / 2] = '.';

    // Finaliza a string da linha
    mapa[LINHAS - 1][COLUNAS] = '\0';
}

// Desloca o mapa para cima e gera uma nova linha
void avancarMapa(void) {
    for (int i = 0; i < LINHAS - 1; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            mapa[i][j] = mapa[i + 1][j];
        }
    }

    gerarLinha();
}

// Atualiza a posição do jogador
void move(Player *p, int y, int x) {
    p->pos[0] = y;
    p->pos[1] = x;
}

// Exibe o mapa
void mostrarMapa(Player *p) {
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            if (i == p->pos[0] && j == p->pos[1]) {
                printf("%c", p->skin);
            } else {
                printf("%c", mapa[i][j]);
            }
        }
        printf("\n");
    }

    printf("\nItens coletados: %d\n", p->points);
}

// Verifica e coleta item na posição indicada
void coletarItem(Player *p, int y, int x) {
    if (mapa[y][x] == '$') {
        p->points++;
        mapa[y][x] = '.';
    }
}

int main(void) {
    Player player1;
    char tecla;

    srand((unsigned int)time(NULL));

    iniciarMapa();
    start(&player1);

    while (1) {
        system("cls");

        mostrarMapa(&player1);

        printf("\nMover (WASD) | Sair (Q): ");

        if (scanf(" %c", &tecla) != 1) {
            break;
        }

        int proxY = player1.pos[0];
        int proxX = player1.pos[1];

        switch (tecla) {
            case 'w':
            case 'W':
                proxY--;
                break;

            case 's':
            case 'S':
                proxY++;
                break;

            case 'a':
            case 'A':
                proxX--;
                break;

            case 'd':
            case 'D':
                proxX++;
                break;

            case 'q':
            case 'Q':
                printf("Saindo do jogo...\n");
                return 0;

            default:
                continue;
        }

        // Não permite atravessar as bordas laterais ou o topo
        if (proxX < 0 || proxX >= COLUNAS ||
            proxY < 0 || proxY >= LINHAS) {
            continue;
        }

        // Movimento para baixo que avança o mapa
        if (tecla == 's' || tecla == 'S') {
            if (proxY == LINHAS - 2) {
                // Verifica o terreno que o jogador alcançará
                if (mapa[proxY][proxX] == '#') {
                    continue;
                }

                // Coleta o item antes de deslocar o mapa
                coletarItem(&player1, proxY, proxX);

                // O jogador permanece na mesma linha da tela
                avancarMapa();
                continue;
            }
        }

        // Movimentos normais
        if (mapa[proxY][proxX] != '#') {
            coletarItem(&player1, proxY, proxX);
            move(&player1, proxY, proxX);
        }
    }

    return 0;
}
    }
    return 0;
}
