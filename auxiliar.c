#include "auxiliar.h"
#include <math.h>
#include <stdio.h>



void check_init(bool check, char *mensagem){
    if (check)
        return;
    printf("Erro ao inicializar: %s",mensagem);
}

/* 350+JP*I <= x < 400+JP+JP*I */
/* JP*I <= x-400 < JP+JP*I */
/* I <= (x-400)/JP < I+1 */
coord click_pos(float x, float y){
    coord pos;
    pos.x = (int)floor( ((x-350)/80) );
    pos.y = (int)floor( ((y-50)/80) );
    return pos;
}

ALLEGRO_COLOR jewel_color(JEWEL_TYPE jp){
    switch(jp){
        case RED : 
            return al_map_rgb(240,0,0);
        case BLUE :
            return al_map_rgb(0,0,240);
        case YELLOW : 
            return al_map_rgb(240,240,0);
        case GREEN :
            return al_map_rgb(0,240,0);
        case MAGENTA :
            return al_map_rgb(150,50,250);
        default :
            return al_map_rgb(255,255,255);
    }
}