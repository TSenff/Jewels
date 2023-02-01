#ifndef AUXILIAR_JEWELS_H_
#define AUXILIAR_JEWELS_H_

#include <stdlib.h>
#include <allegro5/allegro5.h>

struct  coord_s{
    int x;
    int y;
};
typedef struct coord_s coord;


/**
 * @brief Verifica se foi inicializada 
 * 
 * @param check         bool a ser verificado
 * @param mensagem      Nome do que esta sendo testado 
 */
void check_init(bool check, char *mensagem);

coord click_pos(int x, int y);

#endif