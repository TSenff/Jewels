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

#define SCORE_FILE "resources/pontos.txt"

/* Tela */
#define DISPLAY_W  1024
#define DISPLAY_H  768

/* Tabuleiro */
#define JEWEL_PIX 80
#define BOARD_W 350
#define BOARD_H 50


enum {MENU, NEUTRO, TROCA, DESTROI, RETROCA, QUEDA, REFILL, FIM} state ;


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

void draw_game_screen(ALLEGRO_FONT* font,ALLEGRO_FONT *font_small,int pontos,int*scores,int click,coord pos0, JEWEL_TYPE **board, ALLEGRO_BITMAP **jewel_bitmap, short int help){
            al_clear_to_color(al_map_rgb(50, 50, 50));
            draw_score(pontos,font,font_small, scores);
            al_draw_filled_rectangle(BOARD_W -10,BOARD_H -10,BOARD_W+JEWEL_PIX*8 + 10,BOARD_H+JEWEL_PIX*8+10,al_map_rgb(80,80,80));        
            al_draw_filled_rectangle(BOARD_W,BOARD_H,BOARD_W+JEWEL_PIX*8,BOARD_H+JEWEL_PIX*8,al_map_rgb(0,0,0));        
            /* Desenha Marcador de Joia */
            if(click)
                al_draw_filled_rectangle(BOARD_W+JEWEL_PIX*(pos0.x)-5,BOARD_H+JEWEL_PIX*(pos0.y)-5,BOARD_W+JEWEL_PIX*(pos0.x+1)-5,BOARD_H+JEWEL_PIX*(pos0.y+1)-5,al_map_rgba_f(0.2,0.2,0.2,0.1));        
            
            /* Desenha todas as joias */
            for(int i= 0 ; i<8;i++){
                for(int j= 0 ; j<8;j++){
                    draw_jewel(i,j,jewel_bitmap,board[j][i]);
                }
            }

            if (state == FIM)
                draw_endgame_screen(pontos,font, scores);

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


int main(){
    /* Inicializando Alegro  */
    check_init(al_init()                 , "Allegro");
    check_init(al_install_keyboard()     , "Keyboard");
    check_init(al_install_mouse()        , "Mouse");
    check_init(al_init_primitives_addon(), "Primitives");
    check_init(al_init_image_addon()     , "Imagens");
    check_init(al_install_audio()        , "audio");
    check_init(al_init_acodec_addon()    , "audio codecs");
    check_init(al_reserve_samples(16)    , "reserve samples");
    check_init(al_init_font_addon()      , "Fontes");
    check_init(al_init_ttf_addon()       , "Fontes ttf");
    
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    check_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    check_init(queue, "queue");

    ALLEGRO_DISPLAY* disp = al_create_display(DISPLAY_W, DISPLAY_H);
    check_init(disp, "display");

//    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_FONT* font = al_load_ttf_font("resources/GermaniaOne-Regular.ttf",30,0);
    check_init(font, "font");

    ALLEGRO_FONT* font_small = al_load_ttf_font("resources/GermaniaOne-Regular.ttf",20,0);
    check_init(font_small, "font");


    /* Iniciando tabuleiro */

    /* Inicializando Bitmaps */
    ALLEGRO_BITMAP** jewel_bitmap = malloc(sizeof(ALLEGRO_BITMAP*)*8);
    if(jewel_bitmap == NULL)
        return 1;
    
    jewel_bitmap[RED]      = al_load_bitmap("resources/image/red_jewel.png");
    jewel_bitmap[BLUE]     = al_load_bitmap("resources/image/blue_jewel.png");   
    jewel_bitmap[GREEN]    = al_load_bitmap("resources/image/green_jewel.png"); 
    jewel_bitmap[YELLOW]   = al_load_bitmap("resources/image/orange_jewel.png");  
    jewel_bitmap[MAGENTA]  = al_load_bitmap("resources/image/magenta_jewel.png");
    jewel_bitmap[BROWN]    = al_load_bitmap("resources/image/brown_jewel.png");
    jewel_bitmap[WHITE]    = al_load_bitmap("resources/image/white_jewel.png");
    jewel_bitmap[CYAN]     = al_load_bitmap("resources/image/cyan_jewel.png");

    /* Inicializando Audios */
    ALLEGRO_SAMPLE* destroi_sound =al_load_sample("resources/Audio/plim.wav");
    check_init(destroi_sound,"Som: Destroi");
    ALLEGRO_SAMPLE* move_sound =al_load_sample("resources/Audio/swoosh.wav");
    check_init(move_sound,"Som: Movimento");
    ALLEGRO_SAMPLE* default_music =al_load_sample("resources/Audio/kitten_caboodle.wav");
    check_init(default_music,"Som: Padrao");
    ALLEGRO_SAMPLE* miss_sound =al_load_sample("resources/Audio/miss.wav");
    check_init(miss_sound,"Som: falha");
    ALLEGRO_SAMPLE* fall_sound =al_load_sample("resources/Audio/end_fall.wav");
    check_init(fall_sound,"Som: queda");

    /* Registrando Eventos */
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());
    
    /*      Variaveis e Preparaçoes para o loop      */
    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;
    ALLEGRO_MOUSE_STATE m_state;
    int pontos = 0;
    short int snap;
    /* Pontos */

    int level = 0;
    int *scores;
    scores = read_scores(SCORE_FILE);
    if (scores == NULL)
        return 1;


    /* Mouse*/
    short int click = 0;
    float x0 = -1 , y0 = -1, x1 = -1 , y1 = -1;
    coord pos0, pos1, pos_aux;
    pos0.x = 0;
    pos0.y = 0;

    pos1.x = 0;
    pos1.y = 0;

    /* Auxiliar de Movimento */
    int direction = 0;
    int *estoura_pri, *estoura_aux;
    int *queda_pri, *queda_aux;

    /* animação */
    int animation_frame_counter = 0;
    short int help = 0;
    short int rank = 0;
    /* Tabuleiro */
    int **b;
    b = malloc(sizeof(int*)*8);
    for(int i = 0; i < 8; i++)
        b[i] = calloc(8,sizeof(int));

    int **board;
    int **board_aux ;

    int *buffer_jewels = malloc(sizeof(int)*8);
    if (buffer_jewels == NULL)
        return 1;    

    

    /* Flags */
    state = MENU; 

    /* Start Song*/
    al_play_sample(default_music, 0.2, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

    /* Loop*/
    al_start_timer(timer);
    while(1){
        al_wait_for_event(queue, &event);
        switch(event.type){
            case ALLEGRO_EVENT_TIMER:
                switch (state){
                case  DESTROI:
                        #ifdef DEBUGGER
                        printf("Destroi\n");
                        #endif
                        printf("pow");

                        /* ----------- Animação --------*/
                        al_clear_to_color(al_map_rgb(50, 50, 50));
                        draw_score(pontos,font,font_small, scores);
                        al_draw_filled_rectangle(BOARD_W -10,BOARD_H -10,BOARD_W+JEWEL_PIX*8 + 10,BOARD_H+JEWEL_PIX*8+10,al_map_rgb(80,80,80));        
                        al_draw_filled_rectangle(BOARD_W,BOARD_H,BOARD_W+JEWEL_PIX*8,BOARD_H+JEWEL_PIX*8,al_map_rgb(0,0,0));   
                        
                        for(int i= 0 ; i<8;i++){
                            for(int j= 0 ; j<8;j++){
                                if(board_aux[j][i] == EMPTY){
                                    draw_jewel_transparency(i,j,jewel_bitmap,board[j][i],animation_frame_counter);
                                }
                                else{
                                    draw_jewel(i,j,jewel_bitmap,board[j][i]);
                                }
                            }
                        }
                        animation_frame_counter+=2;
                        if(animation_frame_counter > 100){
                            #ifdef DEBUGGER                
                            printf("Destroi -> Queda\n");
                            #endif
                            state = QUEDA;
                            animation_frame_counter = 0; 
                            
                        }


                        al_flip_display();

                        redraw = false;
                        break;


                        /* Reset variaveis de click */
                        pos0.x = 0;
                        pos0.y = 0;
                        pos1.x = 0;
                        pos1.y = 0;
                        click  = 0;
                        state  = NEUTRO;

                    redraw = false;
                    break;
                
                /* Faz a troca */
                case RETROCA:
                    #ifdef DEBUGGER
                    //printf("RE");
                    #endif
                case TROCA:
                    #ifdef DEBUGGER                
                    //printf("TROCA\n");
                    #endif
                    /* ----------- Animação --------*/
                    al_clear_to_color(al_map_rgb(50, 50, 50));
                    draw_score(pontos,font,font_small, scores);
                    al_draw_filled_rectangle(BOARD_W -10,BOARD_H -10,BOARD_W+JEWEL_PIX*8 + 10,BOARD_H+JEWEL_PIX*8+10,al_map_rgb(80,80,80));        
                    al_draw_filled_rectangle(BOARD_W,BOARD_H,BOARD_W+JEWEL_PIX*8,BOARD_H+JEWEL_PIX*8,al_map_rgb(0,0,0));   
                    /* Desenha todas as joias menos as que estão se movendo */
                    for(int i= 0 ; i<8;i++){
                        for(int j= 0 ; j<8;j++){
                            if( (i != pos0.x || j != pos0.y) && (i != pos1.x || j != pos1.y) )
                                draw_jewel(i,j,jewel_bitmap,board[j][i]);
                        }
                    }

                    /* Desenha as Joias que estão se movendo */
                    animation_frame_counter= animation_frame_counter+2;
                    draw_jewel_move(pos0, (-1)*direction/2, direction % 2  ,jewel_bitmap, board[pos0.y][pos0.x],&animation_frame_counter);
                    draw_jewel_move(pos1, direction/2, (-1)*direction % 2  ,jewel_bitmap, board[pos1.y][pos1.x],&animation_frame_counter);
                    
                    /* Fim da animação de troca*/
                    if(animation_frame_counter >= JEWEL_PIX){
                        /* Faz a troca */
                        switch_jewels(board,pos0.y,pos0.x,pos1.y,pos1.x);
                        switch_jewels(board_aux,pos0.y,pos0.x,pos1.y,pos1.x);
                        if (state == RETROCA){
                            #ifdef DEBUGGER
                            printf("Retroca -> Neutro\n");
                            #endif
                            
                            pos0.x = 0;
                            pos0.y = 0;
                            pos1.x = 0;
                            pos1.y = 0;
                            click  = 0;
                            state = NEUTRO;
                        }
                        else{
                            /* Verifica se a troca foi valida*/
                            estoura_pri = estoura(board_aux,pos1.y,pos1.x, direction, board[pos1.y][pos1.x]);
                            estoura_aux = estoura(board_aux,pos0.y,pos0.x, -1*direction,board[pos0.y][pos0.x]);
                            if( (estoura_aux[0] || estoura_aux[1] || estoura_aux[2] || estoura_aux[3]) ||
                                (estoura_pri[0] || estoura_pri[1] || estoura_pri[2] || estoura_pri[3]) ){
                                #ifdef DEBUGGER
                                printf("Troca -> Destroi\n");
                                #endif

                                queda_pri = destroi_pontos(board_aux,pos1.y,pos1.x,estoura_pri, board[pos1.y][pos1.x],&pontos);
                                queda_aux = destroi_pontos(board_aux,pos0.y,pos0.x,estoura_aux, board[pos0.y][pos0.x],&pontos);
                                
                                /* Encontra o ponto vazio mais alto */
                                /* Queda principal recebe o ponto vazio mais alto */
                                /* Queda auxiliar recebe a quantidade de pontos vazios na coluna  */
                                for (int i = 0; i < 8; i++){
                                    queda_pri[i] = min(queda_pri[i],queda_aux[i]);  
                                    queda_aux[i] = 0;
                                    if (queda_pri[i] != 0){
                                        for (int j = queda_pri[i]-1; j < 8; j++){
                                            if(board_aux[j][i] == EMPTY)
                                                queda_aux[i]++;
                                        }
                                    }
                                }
                                
                                state = DESTROI;
                                al_play_sample(destroi_sound, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            }
                            else{
                                #ifdef DEBUGGER
                                printf("Troca -> Retroca\n");
                                #endif
                                al_play_sample(miss_sound, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                state = RETROCA;
                            }
                            
                        }
                        animation_frame_counter = 0;
                    }


                    al_flip_display();

                    redraw = false;
                    break;
                case QUEDA : 

                    /* ----------- Animação -------- */
                    al_clear_to_color(al_map_rgb(50, 50, 50));
                    draw_score(pontos,font,font_small, scores);
                    al_draw_filled_rectangle(BOARD_W -10,BOARD_H -10,BOARD_W+JEWEL_PIX*8 + 10,BOARD_H+JEWEL_PIX*8+10,al_map_rgb(80,80,80));        
                    al_draw_filled_rectangle(BOARD_W,BOARD_H,BOARD_W+JEWEL_PIX*8,BOARD_H+JEWEL_PIX*8,al_map_rgb(0,0,0));   
                    animation_frame_counter = animation_frame_counter+4;

                    for (int i = 0; i < 8; i++){
                        pos_aux.x = i;
                        for(int j = 7 ; j>-1;j--){
                            pos_aux.y = j;
                            if(board_aux[j][i] != EMPTY){
                                if( queda_pri[i] > j ){
                                    draw_jewel_move(pos_aux,0,-1,jewel_bitmap,board[j][i],&animation_frame_counter);
                                }
                                else{
                                    draw_jewel(i,j,jewel_bitmap,board[j][i]);
                                }
                            }
                        }

                        if (queda_pri[i] != 0){
                            pos_aux.x = i;
                            pos_aux.y = -1;
                            draw_jewel_move(pos_aux,0,-1,jewel_bitmap,buffer_jewels[i],&animation_frame_counter);                            
                        }
                        
                    }

                    /* Final da animação */
                    if(animation_frame_counter >= JEWEL_PIX){

                        /* Move todos os que foram animados */
                        for (int i = 0; i < 8; i++){    
                            if (queda_pri[i] != 0){
                                for (int j = queda_pri[i]-1; j > 0; j--){
                                    switch_jewels(board_aux,j-1,i,j,i);
                                }   
                                queda_pri[i]++;
                                queda_aux[i]--;
                            }
                        }

                        #ifdef DEBUGGER
                            printf("QUEDA\n");
                            printf("Auxiliar [0 1 2 3 4 5 6 7  ]\n");
                        #endif

                        /* Refill no Buffer */
                        for (int i = 0; i < 8; i++){
                            if (queda_pri[i] != 0 ){
                                if(queda_pri[i] > 8|| board_aux[queda_pri[i]-1][i] != EMPTY)
                                    queda_pri[i] = 0;
                                board_aux[0][i] =  buffer_jewels[i];
                                buffer_jewels[i] = rand_jewel(level);

                            }
                            
                        }
                    
                        update_board(board,board_aux);                        
                        printf_board(board);
                            #ifdef DEBUGGER
                                printf("QUEDA Teste de Saida\n");
                                printf("Auxiliar [0 1 2 3 4 5 6 7  ]\n");
                                printf("         [%i %i %i %i %i %i %i %i ]\n",queda_pri[0],queda_pri[1],queda_pri[2],queda_pri[3],queda_pri[4],queda_pri[5],queda_pri[6],queda_pri[7]);
                            #endif
                        animation_frame_counter = 0;
                        if( !(queda_pri[0] || queda_pri[1] ||queda_pri[2] ||queda_pri[3] ||queda_pri[4] ||queda_pri[5] ||queda_pri[6] || queda_pri[7] ) ){
                            #ifdef DEBUGGER
                                printf("QUEDA\n");
                                printf("Auxiliar [0 1 2 3 4 5 6 7  ]\n");
                                printf("         [%i %i %i %i %i %i %i %i ]\n",queda_aux[0],queda_aux[1],queda_aux[2],queda_aux[3],queda_aux[4],queda_aux[5],queda_aux[6],queda_aux[7]);
                            #endif
                            state = REFILL;
                            al_play_sample(fall_sound, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

                            #ifdef DEBUGGER
                                printf("QUEDA -> REFIL\n");
                            #endif

                        }
                    }


                    al_flip_display();

                    redraw = false;
                    break;
                case REFILL:
                    #ifdef DEBUGGER
                        printf("REFIL\n");
                        printf("Auxiliar [0 1 2 3 4 5 6 7  ]\n");
                        printf("         [%i %i %i %i %i %i %i %i ]\n",queda_aux[0],queda_aux[1],queda_aux[2],queda_aux[3],queda_aux[4],queda_aux[5],queda_aux[6],queda_aux[7]);
                    #endif
                    /* Verifica o caso de REFIL ter mandado duas ou mais quedas intercaladas para QUEDA previamente
                     * Se sim atualiza valores de queda primaria e continua para Queda
                     */
                    if( (queda_aux[0] || queda_aux[1] ||queda_aux[2] ||queda_aux[3] ||queda_aux[4] ||queda_aux[5] ||queda_aux[6] || queda_aux[7]) ){
                        printf("Entrou no intercala\n");
                        for (int i = 0; i < 8; i++){
                            printf("%i\n",queda_pri[i]);
                            if (queda_aux[i] != 0){
                                for (int j = 0; j < 8; j++){
                                    if(board_aux[j][i] == EMPTY){
                                        queda_pri[i] = j+1;
                                        break;
                                    }
                                }
                            }
                        } 
                        redraw = false;
                        state = QUEDA;
                        #ifdef DEBUGGER
                            printf("REFIL -> QUEDA\n");
                        #endif
                    }
                    else{  

                        /* Verifica se QUEDA causou a formação de novos conjuntos*/
                        queda_pri = seek_and_destroy(board,board_aux,&pontos);
                        if(queda_pri[0]  || queda_pri[1] || queda_pri[2] || queda_pri[3] || queda_pri[4] || queda_pri[5] || queda_pri[6] || queda_pri[7] ){
                            animation_frame_counter = 0;

                            for (int i = 0; i < 8; i++){
                                queda_aux[i] = 0;
                                if (queda_pri[i] != 0){
                                    for (int j = queda_pri[i]-1; j < 8; j++){
                                        if(board_aux[j][i] == EMPTY)
                                            queda_aux[i]++;
                                    }
                                }
                            }
                            al_play_sample(destroi_sound, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            #ifdef DEBUGGER
                                printf("REFIL\n");
                                printf("Auxiliar [0 1 2 3 4 5 6 7  ]\n");
                                printf("         [%i %i %i %i %i %i %i %i ]\n",queda_aux[0],queda_aux[1],queda_aux[2],queda_aux[3],queda_aux[4],queda_aux[5],queda_aux[6],queda_aux[7]);
                            #endif
                            #ifdef DEBUGGER
                                printf("REFIL -> DESTROI\n");
                            #endif
                            state = DESTROI;
                        }   
                        else{
                            #ifdef DEBUGGER
                                printf("REFIL -> FIM\n");
                            #endif
                            state = FIM;
                        }
                    }
                    break;
                case FIM : 
                    done = check_end(board);
                    printf("Xablau???\n");
                    if (done == 1){
                        //scores[1]=pontos;
                        insere(scores,pontos);
                        write_scores(SCORE_FILE,scores);
                        done = 0;
                    }
                    else{
                        state = NEUTRO;
                        
                    }
                    printf("Pontuação : %i \n",pontos);
                    if (pontos > 200){
                        level = 1;
                    }
                    if (pontos > 400){
                        level = 2;
                    }
                    if (pontos > 600){
                        level = 3;
                    }

                    redraw = true;
                    break;
                default:
                    redraw = true;
                    break;
                }



                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                #ifdef DEBUGGER            
                printf("Click\n");
                #endif

                if (state != NEUTRO)
                    break;                

                al_get_mouse_state(&m_state);
                switch(click){
                    case 0: 
                        x0 = m_state.x;
                        y0 = m_state.y;
                        pos0 = click_pos(x0,y0);
                        /* Verifica se foi um click valido */
                        if(pos0.x > -1 && pos0.y > -1 && pos0.x < 8 && pos0.y < 8){
                            click++;
                            /* Ativa o marcador de joia selecionada */
                            b[pos0.x][pos0.y] = 1 ;
                        }
                        break;
                    case 1 :
                        /* Desliga o Marcador de joia selecionada */
                        b[pos0.x][pos0.y] = 0;

                        x1 = m_state.x;
                        y1 = m_state.y;
                        pos1 = click_pos(x1,y1);

                        /* Verifica se Adjacente a click*/
                        if(pos1.x > -1 && pos1.y > -1 && pos1.x < 8 && pos1.y < 8){
                            /* Horizontal */
                            if(abs(pos0.x-pos1.x) == 1 && pos0.y == pos1.y){
                                #ifdef DEBUGGER
                                printf("Horizontal\n");
                                #endif
                                al_play_sample(move_sound, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                direction = 2 * (pos1.x-pos0.x);
                                state = TROCA;
                            }
                            else{
                                /* Vertical */
                                if(abs(pos0.y-pos1.y) == 1 && pos0.x == pos1.x ){
                                    #ifdef DEBUGGER
                                    printf("Vertical\n");
                                    #endif
                                    al_play_sample(move_sound, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                    direction = 1 * (pos0.y-pos1.y);
                                    state = TROCA;
                                }
                            }
                            #ifdef DEBUGGER
                            /* Mensagem Debbug*/
                            printf("pos0 [%i , %i ]\n", pos0.y, pos0.x);
                            printf("pos1 [%i , %i ]\n", pos1.y, pos1.x);
                            printf("Direction %i\n",direction);
                            printf("Color %i\n",board[pos0.y][pos0.x]);
                            #endif
                        }
                        click = 0;
                        break;
                }
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                    switch (event.keyboard.keycode)
                    {
                    case ALLEGRO_KEY_F1:
                    case ALLEGRO_KEY_H:
                       help = !help; 
                       rank = 0;
                       break;
                    case ALLEGRO_KEY_J:
                        if (state == MENU){
                            state = NEUTRO;
                            board = create_board();
                            if(board == NULL){
                                return 1;
                            }

                            fill_board(board);
                            validate_start(board);

                            board_aux = copy_board(board);
                            if(board_aux == NULL){
                                return 1;
                            }

                            for (int i = 0; i < 7; i++){
                                buffer_jewels[i] = rand_jewel(level);
                            }
                        }
                        break;
                    case ALLEGRO_KEY_C:
                        if (state == MENU)
                        break;
                    case ALLEGRO_KEY_S:
                        if (state == MENU && help == 1)
                            snap = !snap;
                        if (state == NEUTRO && snap == 1 ){
                            for (int i = 0; i < 4; i++){
                                for (int j = 0; j < 8; j++){
                                    board_aux[j][i] = -1;
                                }
                                
                            }
                            printf_board(board_aux);
                            queda_pri = malloc(sizeof(int)*8);
                            if (queda_pri == NULL)
                                return 1;

                            queda_aux = malloc(sizeof(int)*8);
                            if (queda_aux == NULL)
                                return 1;
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
                            pontos += 320;
                            al_play_sample(destroi_sound, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            state = DESTROI;
                            redraw = false;
                        }

                        break;
                    
                    case ALLEGRO_KEY_R:
                        if (state == MENU){
                            rank = !rank;
                            help = 0;
                        }
                        if (state == FIM){
                            state = MENU;
                        }
                        
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        if (state == NEUTRO){
                            state = MENU;
                            help = 0;
                            rank = 0;
                        }
                        else{
                            done = true;
                        }
                    default:
                        break;
                    }

                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done){
            free_board(board);
            break;
        }
        if(redraw && al_is_event_queue_empty(queue)){
            if (state == MENU){
                draw_menu_screen(font,font_small,rank,help,scores,snap);
            }
            else{
                draw_game_screen(font,font_small,pontos,scores,click,pos0,board,jewel_bitmap,help);
            }

            al_flip_display();

            redraw = false;
        }
    }

    

    al_destroy_sample(move_sound);
    al_destroy_sample(destroi_sound);
    al_destroy_sample(default_music);
    al_destroy_sample(miss_sound);
    al_destroy_sample(fall_sound);

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
    }