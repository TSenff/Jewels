#include "auxiliar.h"
#include <math.h>
#include <stdio.h>



void check_init(bool check, char *mensagem){
    if (check)
        return;
    printf("Erro ao inicializar: %s",mensagem);
}

coord click_pos(int x, int y){
    coord pos;
    //if(x < 400 || x => 960 || y < 50 || y => 610){
    //   pos.x = -1;
    //   pos.x = -1;
    //}

    /* 400+JP*I <= x < 400+JP+JP*I */
    /* JP*I <= x-400 < JP+JP*I */
    /* I <= (x-400)/JP < 1+I */
    pos.x = (int)floor( (x-400)/70 );
    /* 50+JP*I <= x < 50+JP+JP*I */
    /* JP*I <= x-50 < JP+JP*I */
    /* I <= (x-50)/JP < 1+I */
    pos.y = (int)floor( (y-50)/70 );


    return pos;
}