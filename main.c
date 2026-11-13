
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tda.h"

int main(void) {

    Player player1;
    char tecla;

    srand((unsigned int)time(NULL));

    iniciarMapa();
    start(&player1);

    time_t inicio = time(NULL);

    while (1) {

        system(LIMPAR_TELA);

        mostrarMapa(&player1, mapa, LINHAS, COLUNAS);

        printf("\nMover (WASD) | Sair (Q): ");
        tecla = lerTecla();

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
                exibirResultados(player1, inicio);
                return 0;

            default:
                continue;
        }

        if (proxX < 0 || proxX >= COLUNAS ||
            proxY < 0 || proxY >= LINHAS) {
            continue;
        }

        if (tecla == 's' || tecla == 'S') {

            if (proxY == LINHAS - 2) {

                if (mapa[proxY][proxX] == '#') {
                    continue;
                }

                coletarItem(&player1, mapa, proxY, proxX);

                avancarMapa();

                continue;
            }
        }

        if (mapa[proxY][proxX] != '#') {

            coletarItem(&player1, mapa, proxY, proxX);

            move(&player1, proxY, proxX);
        }
    }

    return 0;
}
