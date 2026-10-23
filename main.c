#include <stdio.h>

#define LINHAS 10
#define COLUNAS 20

//tda player
typedef struct{
    char skin;
    int pos[2];
    int points;
}player;

void start(player *p){
    p->pos[0]=1;
    p->pos[1]=1;
    p->points=0;
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


int main() {

    return 0;
}
