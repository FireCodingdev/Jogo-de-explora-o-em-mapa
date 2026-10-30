#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tda.h"

#define LINHAS 22
#define COLUNAS 20
#define NUM_BLOCOS 5

char mapa[LINHAS][COLUNAS + 1];

typedef struct {
    int n;
    const char *linhas[6];
} Bloco;
/* mapas pré carregados para a geração continua */

static const Bloco blocos[NUM_BLOCOS] = {

    { 5, {
        "#.##.###....######.#",
        "#.#.#..##...#.#$...#",
        "#.#$...#.#..#..#.#.#",
        "#.##.###....#.#.##.#",
        "#....#....#........#",
        "#..................#"
    } },

    { 5, {
        "#.###.####..###.##.#",
        "#.##...#....#$...#.#",
        "#..$##..........$#.#",
        "#.###$###...######.#",
        "#.#....###..###$##.#",
        "#.#.##.$##..###.##.$",
        "#.#.#$#$##....#.$#.#",
        "$.#.#$..##..#$#.##.#",
        "#.#.#$.#....###.##.#",
        "#.#.#$.#.#..##$.##.#",
        "#.#..##..#..###.##.#",
        "#.##....##..#...##.#",
        "#.###$####....#....#",
        "#.###$####..#$####.#",
        "##$...##...#.#.....#",
        "#..................#"
    } },

    { 6, {
        "#.#######..###.###.#",
        "#.#.....#.....#..###",
        "#.#..#..#..#.....#.#",
        "###.$#.....#.$#..#.#",
        "#.################.#",
        "#..................#"
    } },

    { 4, {
        "#..#$#..#$$#..#$#..#",
        "#..#$#..$##$..#$#..#",
        "#..#$#..#$$#..#$#..#",
        "#..................#"
    } },

    { 6, {
        "#.##################",
        "#.#.........#$.....#",
        "#...##..#...####...#",
        "####$$#.#.#..#...#.#",
        "#.#$$$#...#........#",
        "#.#.#.###.#.#.#.#..#",
        "#......#....#......#",
        "#.#.#....#.....#...#",
        "#.############$#####",
        "#..................#"
    } }
};

/* Inicializa o mapa original */
void iniciarMapa(void) {
    char inicial[LINHAS][COLUNAS + 1] = {
        "####################",
        "#..................#",
        "#.###.##..#####..#.#",
        "##$$#..#....##$#.#.#",
        "#.#..#.#..###..#.#.#",
        "#.$.#.........##.#.#",
        "#....###.........#.#",
        "####.#..##.#..##.#.#",
        "#$$#....##.#.##.##.#",
        "#..$##..$$.##..#$$.#",
        "#...$#..##.#..####.#",
        "#...$#.............#",
        "#$#.#..##$#####$.#.#",
        "#$#..##.$#$.....#$##",
        "####.#..#...#..#$$$#",
        "#....#.##..#..##.#.#",
        "#.#..#..#..#.#.#.#.#",
        "#.#.#.#......#.#.#.#",
        "#.#.....#....#.#....#",
        "#.###...#........#.#",
        "#.....#....#..#....#",
        "#.#.#####.######.#.#",
        "#..................#"
    };

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j <= COLUNAS; j++) {
            mapa[i][j] = inicial[i][j];
        }
    }
}



/* Copia uma linha pronta para a ultima linha do mapa */
static void escreverLinha(const char *origem) {
    for (int j = 0; j < COLUNAS; j++) {
        mapa[LINHAS - 1][j] = origem[j];
    }
    mapa[LINHAS - 1][COLUNAS] = '\0';
}

/* Gerador original: linha totalmente aleatoria */
static void gerarLinhaAleatoria(void) {

    mapa[LINHAS - 1][0] = '#';
    mapa[LINHAS - 1][COLUNAS - 1] = '#';

    for (int j = 1; j < COLUNAS - 1; j++) {

        int sorteio = rand() % 100;

        if (sorteio < 20) {
            mapa[LINHAS - 1][j] = '#';
        }
        else if (sorteio < 30) {
            mapa[LINHAS - 1][j] = '$';
        }
        else {
            mapa[LINHAS - 1][j] = '.';
        }
    }

    /* Garante pelo menos uma passagem */
    mapa[LINHAS - 1][COLUNAS / 2] = '.';

    mapa[LINHAS - 1][COLUNAS] = '\0';
}

/* Gerador por blocos: entrega a proxima linha do bloco atual */
static int blocoAtual = 0;
static int linhaAtual = 0;

static void gerarLinhaBloco(void) {
    escreverLinha(blocos[blocoAtual].linhas[linhaAtual]);
    linhaAtual++;
}

/* Controle: alterna entre aleatorio e blocos */
#define MIN_ALEATORIAS 5
#define MAX_ALEATORIAS 10

typedef enum {
    FASE_CORREDOR_INICIAL,
    FASE_ALEATORIA,
    FASE_CORREDOR_ANTES_BLOCO,
    FASE_BLOCO
} Fase;

static Fase fase = FASE_CORREDOR_INICIAL;
static int restantes = 0;

void gerarLinha(void) {

    const char *corredor = "#..................#";

    switch (fase) {

        case FASE_CORREDOR_INICIAL:
            escreverLinha(corredor);
            restantes = MIN_ALEATORIAS +
                        rand() % (MAX_ALEATORIAS - MIN_ALEATORIAS + 1);
            fase = FASE_ALEATORIA;
            break;

        case FASE_ALEATORIA:
            gerarLinhaAleatoria();
            restantes--;
            if (restantes <= 0) {
                fase = FASE_CORREDOR_ANTES_BLOCO;
            }
            break;

        case FASE_CORREDOR_ANTES_BLOCO:
            escreverLinha(corredor);
            blocoAtual = rand() % NUM_BLOCOS;
            linhaAtual = 0;
            fase = FASE_BLOCO;
            break;

        case FASE_BLOCO:
            gerarLinhaBloco();
            /* O bloco ja termina com um corredor livre */
            if (linhaAtual >= blocos[blocoAtual].n) {
                restantes = MIN_ALEATORIAS +
                            rand() % (MAX_ALEATORIAS - MIN_ALEATORIAS + 1);
                fase = FASE_ALEATORIA;
            }
            break;
    }
}

/* Desloca o mapa para cima */
void avancarMapa(void) {

    for (int i = 0; i < LINHAS - 1; i++) {

        for (int j = 0; j < COLUNAS; j++) {
            mapa[i][j] = mapa[i + 1][j];
        }
    }

    gerarLinha();
}

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

        /* Impede sair dos limites do mapa */
        if (proxX < 0 || proxX >= COLUNAS ||
            proxY < 0 || proxY >= LINHAS) {
            continue;
        }

        /* Movimento para baixo que avança o mapa */
        if (tecla == 's' || tecla == 'S') {

            if (proxY == LINHAS - 2) {

                /* Verifica se existe parede */
                if (mapa[proxY][proxX] == '#') {
                    continue;
                }

                /* Coleta item antes de avançar */
                coletarItem(&player1, mapa, proxY, proxX);

                /* Avança o mapa */
                avancarMapa();

                continue;
            }
        }

        /* Movimentos normais */
        if (mapa[proxY][proxX] != '#') {

            coletarItem(&player1, mapa, proxY, proxX);

            move(&player1, proxY, proxX);
        }
    }

    return 0;
}
