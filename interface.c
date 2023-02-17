#include "interface.h"




/**
 * @brief 
 * 
 * @param i ROW 
 * @param j COLLUM
 * @param jewels_bitmap 
 * @param jp 
 */
void draw_jewel(int i,int j,ALLEGRO_BITMAP** jewels_bitmap,JEWEL_TYPE jp){
    al_draw_bitmap(jewels_bitmap[jp], BOARD_W+JEWEL_PIX*i,BOARD_H+JEWEL_PIX*j, 0);
}

/**
 * @brief 
 * 
 * @param i ROW 
 * @param j COLLUM
 * @param jewels_bitmap 
 * @param jp 
 */
void draw_jewel_move(coord ini, int i_valid, int j_valid ,ALLEGRO_BITMAP** jewels_bitmap,JEWEL_TYPE jp_ini, int *i){
    al_draw_bitmap(jewels_bitmap[jp_ini], BOARD_W+JEWEL_PIX*(ini.x) - i_valid*(*i+1) ,BOARD_H+JEWEL_PIX*(ini.y) - j_valid*(*i+1), 0);
}

void draw_jewel_transparency(int i,int j, ALLEGRO_BITMAP** jewels_bitmap, JEWEL_TYPE jp_ini, int k){
    al_draw_tinted_bitmap(jewels_bitmap[jp_ini], al_map_rgb_f(((float)100-(k))/100,((float)100-(k))/100,((float)100-(k))/100),BOARD_W+JEWEL_PIX*(i), BOARD_H+JEWEL_PIX*(j), 0);
}

void draw_score(int points, ALLEGRO_FONT* font, ALLEGRO_FONT* font_small, int *score_board){

    al_draw_filled_rectangle(JEWEL_PIX/2 -10, BOARD_H -10, BOARD_W-JEWEL_PIX/2 +10,BOARD_H+JEWEL_PIX*8 +10 ,al_map_rgb(80,80,80));        
    al_draw_filled_rectangle(JEWEL_PIX/2,BOARD_H , BOARD_W-JEWEL_PIX/2,BOARD_H+JEWEL_PIX*8,al_map_rgb(100,100,100));        

    al_draw_textf(font, al_map_rgb(255, 255, 255), 110, 65, 0, "%09i", points);
    al_draw_filled_rectangle(JEWEL_PIX/2 -10, 110, BOARD_W-JEWEL_PIX/2 +10,120 +10 ,al_map_rgb(80,80,80));        

    al_draw_textf(font, al_map_rgb(255, 255, 255), 120, 140, 0, "Ranking");
    for (int i = 0; i < SCORE_BOARD_SIZE; i++){
        if (score_board[i] == 0 )
            break;
        al_draw_textf(font, al_map_rgb(255, 255, 255), 50,200+i*50, 0, "%i - %i", i+1, score_board[i]);
    }   
    
    al_draw_textf(font_small, al_map_rgb(255, 255, 255), JEWEL_PIX/2 +10,BOARD_H+JEWEL_PIX*8 -30, 0, "[H] ou [F1] para Ajuda");
    
}

void draw_endgame_screen(int points, ALLEGRO_FONT* font, int *score_board){

    al_draw_filled_rectangle(140, 140, 910, 660,al_map_rgb(80,80,80));        
    al_draw_filled_rectangle(150, 150, 900, 650,al_map_rgb(100,100,100));        
    al_draw_textf(font, al_map_rgb(255, 255, 255), 160, 160, 0, "Parabêns sua pontuação foi: %09i", points);
    al_draw_filled_rectangle(150, 200, 900, 210,al_map_rgb(80,80,80));        

    al_draw_textf(font, al_map_rgb(255, 255, 255), 450, 250, 0, "Ranking");
    for (int i = 0; i < SCORE_BOARD_SIZE; i++){
        if (score_board[i] == 0 )
            break;
        al_draw_textf(font, al_map_rgb(255, 255, 255), 450  ,300+i*50, 0, "%i - %i", i+1, score_board[i]);
    }  

    al_draw_textf(font, al_map_rgb(255, 255, 255), 160, 600, 0, "Aperte [R] retornar ao Menu    ");
}

void draw_help_screen(ALLEGRO_FONT* font, ALLEGRO_FONT  *font_small){

    al_draw_filled_rectangle(140, 140, 910, 660,al_map_rgb(80,80,80));        
    al_draw_filled_rectangle(150, 150, 900, 650,al_map_rgb(100,100,100));        
    al_draw_filled_rectangle(140, 140, 900, 210,al_map_rgb(80,80,80));        
    al_draw_textf(font, al_map_rgb(255, 255, 255), 160, 160, 0, "Ajuda");

    al_draw_textf(font_small, al_map_rgb(255, 255, 255), 160, 260, 0, "Use o Mouse para selecionar uma joia e selecione uma joia adjacente para tentar move-las ");
    al_draw_textf(font_small, al_map_rgb(255, 255, 255), 160, 280, 0, "Um click em qualquer local que não seja uma joia não adjacente desceleciona a joia anterior ");
    al_draw_textf(font_small, al_map_rgb(255, 255, 255), 160, 300, 0, "Novas Joias são adicionadas quanto maior a sua pontuação ");

    al_draw_textf(font, al_map_rgb(255, 255, 255), 160, 550, 0, "[Esc] Retorna para o menu");
    al_draw_textf(font, al_map_rgb(255, 255, 255), 160, 600, 0, "Aperte [h] para retornar");
}

void draw_game_screen(ALLEGRO_FONT* font,ALLEGRO_FONT *font_small,int pontos,int*score_board,int click,coord pos0, JEWEL_TYPE **board, ALLEGRO_BITMAP **jewel_bitmap, short int help,STATE state){
            al_clear_to_color(al_map_rgb(50, 50, 50));
            draw_score(pontos,font,font_small, score_board);
            al_draw_filled_rectangle(BOARD_W -10,BOARD_H -10,BOARD_W+JEWEL_PIX*8 + 10,BOARD_H+JEWEL_PIX*8+10,al_map_rgb(80,80,80));        
            al_draw_filled_rectangle(BOARD_W,BOARD_H,BOARD_W+JEWEL_PIX*8,BOARD_H+JEWEL_PIX*8,al_map_rgb(0,0,0));        
            /* Desenha Marcador de Joia */
            if(click)
                al_draw_filled_rectangle(BOARD_W+JEWEL_PIX*(pos0.x),BOARD_H+JEWEL_PIX*(pos0.y),BOARD_W+JEWEL_PIX*(pos0.x+1)-10,BOARD_H+JEWEL_PIX*(pos0.y+1)-10,al_map_rgba_f(0.2,0.2,0.2,0.1));        
            
            /* Desenha todas as joias */
            for(int i= 0 ; i<8;i++){
                for(int j= 0 ; j<8;j++){
                    draw_jewel(i,j,jewel_bitmap,board[j][i]);
                }
            }

            if (state == FIM)
                draw_endgame_screen(pontos,font, score_board);

            if (help)
                draw_help_screen(font,font_small);
}

void draw_menu_screen(ALLEGRO_FONT* font,ALLEGRO_FONT* font_small,short int rank,short int info,int *score_board,short int snap){
    al_clear_to_color(al_map_rgb(50, 50, 50));
    al_draw_textf(font, al_map_rgb(255, 255, 255), 100, 100, 0, "JEWELS");
    al_draw_textf(font, al_map_rgb(255, 255, 255), 100, 400, 0, "[J]   Jogar");
    al_draw_textf(font, al_map_rgb(255, 255, 255), 100, 450, 0, "[R]   Ranking");
    al_draw_textf(font, al_map_rgb(255, 255, 255), 100, 500, 0, "[H]   Informações");
    al_draw_textf(font, al_map_rgb(255, 255, 255), 100, 550, 0, "[Esc] Sair");
    if (info ){
            al_draw_filled_rectangle(490, 140, 910, 660,al_map_rgb(80,80,80));        
            al_draw_filled_rectangle(500, 150, 900, 650,al_map_rgb(100,100,100));        
            al_draw_filled_rectangle(500, 150, 900, 210,al_map_rgb(80,80,80));        
            al_draw_textf(font, al_map_rgb(255, 255, 255), 510, 160, 0, "Ajuda");
            al_draw_textf(font_small, al_map_rgb(255, 255, 255), 510, 240, 0, "Autor: Thiago da C. P. Senff");
            al_draw_textf(font_small, al_map_rgb(255, 255, 255), 540, 270, 0, "Creditos a Terceiros: ");
            al_draw_textf(font_small, al_map_rgb(255, 255, 255), 510, 300, 0, "Musica: Kitten Caboodle (Cat Quest II) ");
            al_draw_textf(font_small, al_map_rgb(255, 255, 255), 510, 330, 0, "Fonte:  Germania One (Cat Quest II) ");
            al_draw_textf(font_small, al_map_rgb(255, 255, 255), 540, 360, 0, "Cheat Code : ");
            al_draw_textf(font_small, al_map_rgb(255, 255, 255), 510, 390, 0, "[S] Durante o jogo : %s",(snap)?"Ligado":"Desligado");
    }
    if (rank){
        al_draw_filled_rectangle(490, 140, 910, 660,al_map_rgb(80,80,80));        
        al_draw_filled_rectangle(500, 150, 900, 650,al_map_rgb(100,100,100));        
        al_draw_filled_rectangle(500, 150, 900, 210,al_map_rgb(80,80,80));        
        al_draw_textf(font, al_map_rgb(255, 255, 255), 510, 160, 0, "Rank");
        for (int i = 0; i < SCORE_BOARD_SIZE; i++){
        if (score_board[i] == 0 ){
            al_draw_textf(font, al_map_rgb(255, 255, 255), 510  ,240+i*50, 0, "%i - ", i+1);
        }
        else{
            al_draw_textf(font, al_map_rgb(255, 255, 255), 510  ,240+i*50, 0, "%i - %i", i+1, score_board[i]);
        }
    }  

    }
    

}
void resolve_keybord(int keycode, STATE *state,short int click, short int *rank, short int *help,short int *snap, int *animation_frame_counter, ALLEGRO_SAMPLE* secret_sound,JEWEL_TYPE **board, JEWEL_TYPE **board_aux, JEWEL_TYPE *buffer_jewels, int** queda_pri, int **queda_aux,_Bool *done,_Bool *redraw,int level){
    /* Menu de Ajuda, Sempre Disponivel */
    if((keycode == ALLEGRO_KEY_F1 || keycode == ALLEGRO_KEY_H) ){
        *help = !(*help); 
        *rank = 0;
        return;
    }
    
    /* Teclado dentro do Menu */
    if(*state == MENU){
        switch (keycode){
            case ALLEGRO_KEY_J:
                *help = 0;
                *state = NEUTRO;
                board = create_board();
                
                if(board == NULL)
                    return;
                
                fill_board(board);
                validate_start(board);
                
                board_aux = copy_board(board);
                if(board_aux == NULL)
                    return;

                for (int i = 0; i < 7; i++)
                    buffer_jewels[i] = rand_jewel(level);

                break;
            
            case ALLEGRO_KEY_S:
                if (*help == 1)
                    *snap = !(*snap);
                break;

            case ALLEGRO_KEY_R:
                *rank = !(*rank);
                *help = 0;
                break;

            case ALLEGRO_KEY_ESCAPE:
                *done = true;
                *redraw = false;
                break;
            default:
                break;
        }
        return;
    }
    
    
    /* Durante o Jogo */
    switch (keycode){
        case ALLEGRO_KEY_S:
            if (*state == NEUTRO && *snap == 1 ){
                
                *queda_pri = calloc(8,sizeof(int));
                if (queda_pri == NULL)
                    return;
                
                *queda_aux = calloc(8,sizeof(int));
                if (queda_aux == NULL)
                    return;

                for (int i = 0; i < 4; i++){
                    for (int j = 0; j < 8; j++){
                        board_aux[j][i] = -1;
                    }
                    *queda_pri[i] = 1;
                    *queda_aux[i] = 8;
                }
                /*
                    queda_pri[0] = 1;
                    queda_pri[1] = 1;
                    queda_pri[2] = 1;
                    queda_pri[3] = 1;
                    queda_pri[4] = 0;
                    queda_pri[5] = 0;
                    queda_pri[6] = 0;
                    queda_pri[7] = 0;
                    queda_aux[0] = 8;
                    queda_aux[3] = 8;
                    queda_aux[1] = 8;
                    queda_aux[2] = 8;
                    queda_aux[4] = 0;
                    queda_aux[5] = 0;
                    queda_aux[6] = 0;
                    queda_aux[7] = 0;
                */
                *state = DELAY;
                al_play_sample(secret_sound, 2, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                *animation_frame_counter = 60 * 5;
            }
            break;
            
        case ALLEGRO_KEY_R:
            if (*state == FIM)
                *state = MENU;
            break;

        case ALLEGRO_KEY_ESCAPE:
            *state = MENU;
            *help = 0;
            *rank = 0;
            break;
        default:
            break;
        }
}