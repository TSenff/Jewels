#ifndef AL_INTERFACE_JEWELS
#define AL_INTERFACE_JEWELS

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "auxiliar.h"
#include "board.h"

void draw_jewel(int i,int j,ALLEGRO_BITMAP** jewels_bitmap,JEWEL_TYPE jp);

void draw_jewel_move(coord ini, int i_valid, int j_valid ,ALLEGRO_BITMAP** jewels_bitmap,JEWEL_TYPE jp_ini, int *i);

void draw_jewel_transparency(int i,int j, ALLEGRO_BITMAP** jewels_bitmap, JEWEL_TYPE jp_ini, int k);

void draw_score(int points, ALLEGRO_FONT* font, ALLEGRO_FONT* font_small, int *score_board);

void draw_endgame_screen(int points, ALLEGRO_FONT* font, int *score_board);

void draw_menu_screen(ALLEGRO_FONT* font,ALLEGRO_FONT* font_small,short int rank,short int info,int *score_board,short int snap);

void draw_help_screen(ALLEGRO_FONT* font, ALLEGRO_FONT  *font_small);

void draw_game_screen(ALLEGRO_FONT* font,ALLEGRO_FONT *font_small,int pontos,int*score_board,int click,coord pos0, JEWEL_TYPE **board, ALLEGRO_BITMAP **jewel_bitmap, short int help,enum STATE_E state);

void resolve_queda();
void resolve_destroi();
void resolve_queda();
void resolve_queda();
void resolve_queda();

// snap, help, queda_pri, queda auxiliar 
void resolve_keybord(int keycode, STATE *state,short int click, short int *rank, short int *help,short int *snap, int *animation_frame_counter, ALLEGRO_SAMPLE* secret_sound,JEWEL_TYPE **board, JEWEL_TYPE **board_aux, JEWEL_TYPE *buffer_jewels, int** queda_pri, int **queda_aux,_Bool *done,_Bool *redraw,int level);


#endif