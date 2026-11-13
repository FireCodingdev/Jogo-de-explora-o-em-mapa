
#include "tda.h"
#include <string.h>
#include <stdlib.h>

// Mapa global 
char mapa[LINHAS][COLUNAS + 1];

// TDA Player 

void start(Player *p) {

    p->skin = '@';
    p->pos[0] = 1;
    p->pos[1] = 1;
    p->points = 0;
}

void move(Player *p, int y, int x) {

    p->pos[0] = y;
    p->pos[1] = x;
}

void mostrarMapa(Player *p, char mapa[][COLUNAS + 1],
                 int linhas, int colunas) {

    for (int i = 0; i < linhas; i++) {

        for (int j = 0; j < colunas; j++) {

            if (i == p->pos[0] && j == p->pos[1]) {
                printf("%c", p->skin);
            }
            else {
                printf("%c", mapa[i][j]);
            }
        }

        printf("\n");
    }

    printf("\nItens coletados: %d\n", p->points);
}

void coletarItem(Player *p, char mapa[][COLUNAS + 1],
                 int y, int x) {

    if (mapa[y][x] == '$') {

        p->points++;
        mapa[y][x] = '.';
    }
}

void exibirResultados(Player p, time_t inicio) {

    time_t fim = time(NULL);

    int totalSegundos = (int)difftime(fim, inicio);

    int minutos = totalSegundos / 60;
    int segundos = totalSegundos % 60;

    printf("\n===== RESULTADOS DA PARTIDA =====\n");

    printf("Itens coletados: %d\n", p.points);

    printf("Tempo de partida: %02d:%02d\n",
           minutos, segundos);

    printf("=================================\n");
}

char lerTecla(void) {

#ifdef _WIN32

    return _getch();

#else

    struct termios antigo;
    struct termios novo;

    tcgetattr(STDIN_FILENO, &antigo);

    novo = antigo;

    novo.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &novo);

    char c = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &antigo);

    return c;

#endif
}

// ordena por mais pontos; em empate, vence o menor tempo e tal
static int compararRegistros(const void *a, const void *b) {

    const Registro *ra = (const Registro *)a;
    const Registro *rb = (const Registro *)b;

    if (rb->pontos != ra->pontos) {
        return rb->pontos - ra->pontos;
    }

    return ra->segundos - rb->segundos;
}

// lee o arquivo e devolve quantos registros foram carregados
int carregarRanking(Registro ranking[]) {

    FILE *arquivo = fopen(ARQUIVO_RANKING, "r");

    if (arquivo == NULL) {
        return 0; // ainda nao existe ranking
    }

    int n = 0;
    char linha[128];

    while (n < MAX_RANKING && fgets(linha, sizeof(linha), arquivo)) {

        Registro r;

        if (sscanf(linha, "%d;%d;%20[^\r\n]",
                   &r.pontos, &r.segundos, r.nome) == 3) {
            ranking[n++] = r;
        }
    }

    fclose(arquivo);

    return n;
}

// Insere a partida, reordena, mantem so o top MAX_RANKING e grava
// Retorna 1 em caso de sucesso e 0 em caso de falha
int salvarRegistro(const char *nome, int pontos, int segundos) {

    Registro lista[MAX_RANKING + 1];

    int n = carregarRanking(lista);

    strncpy(lista[n].nome, nome, TAM_NOME);
    lista[n].nome[TAM_NOME] = '\0';
    lista[n].pontos = pontos;
    lista[n].segundos = segundos;
    n++;

    qsort(lista, n, sizeof(Registro), compararRegistros);

    if (n > MAX_RANKING) {
        n = MAX_RANKING;
    }

    FILE *arquivo = fopen(ARQUIVO_RANKING, "w");

    if (arquivo == NULL) {
        return 0;
    }

    for (int i = 0; i < n; i++) {
        fprintf(arquivo, "%d;%d;%s\n",
                lista[i].pontos, lista[i].segundos, lista[i].nome);
    }

    fclose(arquivo);

    return 1;
}

// ler o nome do jogador (usa "Anonimo" se ficar vazio)
void lerNome(char *destino, int tamanho) {

    // Descarta teclas pendentes do buffer de entrada
#ifdef _WIN32
    while (_kbhit()) {
        _getch();
    }
#else
    tcflush(STDIN_FILENO, TCIFLUSH);
#endif

    if (fgets(destino, tamanho, stdin) == NULL) {
        destino[0] = '\0';
    }

    destino[strcspn(destino, "\r\n")] = '\0';

    if (destino[0] == '\0') {
        strcpy(destino, "Anonimo");
    }
}

void exibirRanking(void) {

    Registro lista[MAX_RANKING];

    int n = carregarRanking(lista);

    printf("\n========== RANKING (TOP %d) ==========\n", MAX_RANKING);

    if (n == 0) {
        printf("Nenhuma partida registrada ainda.\n");
    }
    else {
        printf("%-3s %-*s %6s %7s\n", "#", TAM_NOME, "Nome", "Itens", "Tempo");

        for (int i = 0; i < n; i++) {
            printf("%-3d %-*s %6d %02d:%02d\n",
                   i + 1, TAM_NOME, lista[i].nome, lista[i].pontos,
                   lista[i].segundos / 60, lista[i].segundos % 60);
        }
    }

    printf("======================================\n");
}

// GERACAO DO MAPA

#define NUM_BLOCOS 5
#define MIN_ALEATORIAS 5
#define MAX_ALEATORIAS 10

typedef struct {
    const char *linhas[25];
} Bloco;

// Mapas pre-carregados 

static const Bloco blocos[NUM_BLOCOS] = {

    { {
        "#.##.###....######.#",
        "#.#.#..##...#.#$...#",
        "#.#$...#.#..#..#.#.#",
        "#.##.###....#.#.##.#",
        "#....#....#........#",
        "#..................#"
    } },

    { {
        "#.###.####..###.##.#",
        "#.##...#....#$...#.#",
        "#..$##..........$#.#",
        "#.###$###...######.#",
        "#.#....###..###$##.#",
        "#.#.##.$##..###.##.#",
        "#.#.#$#$##....#.$#.#",
        "#.#.#$..##..#$#.##.#",
        "#.#.#$.#....###.##.#",
        "#.#.#$.#.#..##$.##.#",
        "#.#..##..#..###.##.#",
        "#.##....##..#...##.#",
        "#.###$####....#....#",
        "#.###$####..#$####.#",
        "##$...##...#.#.....#",
        "#..................#"
    } },

    { {
        "#.#######..###.###.#",
        "#.#.....#.....#..###",
        "#.#..#..#..#.....#.#",
        "###.$#.....#.$#..#.#",
        "#.################.#",
        "#..................#"
    } },

    { {
        "#..#$#..#$$#..#$#..#",
        "#..#$#..$##$..#$#..#",
        "#..#$#..#$$#..#$#..#",
        "#..................#"
    } },

    { {
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

// Conta as linhas de um bloco

static int tamanhoBloco(const Bloco *b) {

    int n = 0;

    while (b->linhas[n] != NULL) {
        n++;
    }

    return n;
}

// Inicializa o mapa original 

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
        "#.#.....#...#.#....#",
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

// Copia uma linha para a ultima linha do mapa 

static void escreverLinha(const char *origem) {

    for (int j = 0; j < COLUNAS; j++) {

        mapa[LINHAS - 1][j] = origem[j];
    }

    mapa[LINHAS - 1][COLUNAS] = '\0';
}

// Geracao de linhas aleatorias 

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

    mapa[LINHAS - 1][COLUNAS / 2] = '.';

    mapa[LINHAS - 1][COLUNAS] = '\0';
}

// Geracao por blocos

static int blocoAtual = 0;
static int linhaAtual = 0;

static void gerarLinhaBloco(void) {

    escreverLinha(blocos[blocoAtual].linhas[linhaAtual]);

    linhaAtual++;
}

// Controle das fases 

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

            if (linhaAtual >= tamanhoBloco(&blocos[blocoAtual])) {

                restantes = MIN_ALEATORIAS +
                    rand() % (MAX_ALEATORIAS - MIN_ALEATORIAS + 1);

                fase = FASE_ALEATORIA;
            }

            break;
    }
}

// Desloca o mapa para cima 

void avancarMapa(void) {

    for (int i = 0; i < LINHAS - 1; i++) {

        for (int j = 0; j < COLUNAS; j++) {

            mapa[i][j] = mapa[i + 1][j];
        }
    }

    gerarLinha();
}
