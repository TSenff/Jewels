#ifndef AUXILIAR_JEWELS_H_
#define AUXILIAR_JEWELS_H_

#include <stdlib.h>
#include <allegro5/allegro5.h>
#include "board.h"


#define SCORE_FILE "resources/pontos.txt"

enum STATE_E {MENU,DELAY, NEUTRO, TROCA, DESTROI, RETROCA, QUEDA, REFILL, FIM} ;
typedef enum STATE_E STATE;
/* Tela */
#define DISPLAY_W  1024
#define DISPLAY_H  768

/* Tabuleiro */
#define JEWEL_PIX 80
#define BOARD_W 350
#define BOARD_H 50


#define SCORE_BOARD_SIZE 5

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

coord click_pos(float x, float y);

ALLEGRO_COLOR jewel_color(JEWEL_TYPE jp);

int write_scores(char *path, int *score);

int* read_scores(char *path);

int insere(int* score_board, int score);

#endif