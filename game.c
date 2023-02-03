#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

#include "auxiliar.h"
#include "board.h"

/* Tela */
#define DISPLAY_W  1024
#define DISPLAY_H  768

/* Tabuleiro */
#define JEWEL_PIX 80
#define BOARD_W 350
#define BOARD_H 50



int main(){
    /* Inicializando Alegro  */
    check_init(al_init(), "Allegro");
    check_init(al_install_keyboard(),"Keyboard");
    check_init(al_install_mouse(),"Mouse");
    check_init(al_init_primitives_addon(),"Primitives");
    check_init(al_init_image_addon(),"Imagens");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    check_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    check_init(queue, "queue");

    ALLEGRO_DISPLAY* disp = al_create_display(DISPLAY_W, DISPLAY_H);
    check_init(disp, "display");

    ALLEGRO_FONT* font = al_create_builtin_font();
    check_init(font, "font");


    /* Iniciando tabuleiro */
    int **board = create_board();
    if(board == NULL){
        return 1;
    }
    fill_board(board);
    validate_start(board);

    /* Inicializando Bitmaps */
    ALLEGRO_BITMAP* blue = al_load_bitmap("Assets/blue_jewel_no_bkg.png");
    ALLEGRO_BITMAP* green = al_load_bitmap("Assets/green_jewel_no_bkg.png");
    ALLEGRO_BITMAP* yellow = al_load_bitmap("Assets/yellow_jewel_no_bkg.png");
    ALLEGRO_BITMAP* red = al_load_bitmap("Assets/red_jewel_no_bkg.png");
    ALLEGRO_BITMAP* magenta = al_load_bitmap("Assets/magenta_jewel_no_bkg.png");

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


    /* Mouse*/
    short int click = 0;
    float x0 = -1 , y0 = -1, x1 = -1 , y1 = -1;
    coord pos0, pos1;
    pos0.x = 0;
    pos0.y = 0;

    pos1.x = 0;
    pos1.y = 0;

    /* Auxiliar de Movimento */
    int direction = 0;



    /* Tabuleiro */
    int **b;
    b = malloc(sizeof(int*)*8);
    for(int i = 0; i < 8; i++)
        b[i] = calloc(8,sizeof(int));

    /* Loop*/
    al_start_timer(timer);
    while(1){
        al_wait_for_event(queue, &event);

        switch(event.type){
            case ALLEGRO_EVENT_TIMER:
                // game logic goes here.
                redraw = true;
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                printf("Click\n");
                al_get_mouse_state(&m_state);
                switch(click){
                    case 0: 
                        b[pos0.x][pos0.y] = 0;
                        x0 = m_state.x;
                        y0 = m_state.y;
                        pos0 = click_pos(x0,y0);
                        if(pos0.x > -1 && pos0.y > -1 && pos0.x < 8 && pos0.y < 8){
                            click++;
                            b[pos0.x][pos0.y] = 1 ;
                        }
                        break;
                    case 1 :
                        b[pos1.x][pos1.y] = 0;
                        x1 = m_state.x;
                        y1 = m_state.y;
                        pos1 = click_pos(x1,y1);
                        if(pos1.x > -1 && pos1.y > -1 && pos1.x < 8 && pos1.y < 8){
                            /* Verifica Adjacente */
                            if(abs(pos0.x-pos1.x) == 1 || abs(pos0.y-pos1.y) == 1){
                                /* Movimento Vertical*/
                                if(pos0.x == pos1.x){
                                    direction = 1 * pos0.x-pos1.x;
                                }
                                /* Movimento Horizontal*/
                                if(pos0.y == pos1.y){
                                    direction = 2 * pos0.x-pos1.x;
                                }
                            }
                        }
                        break;
                }

                printf("pos0 [%i , %i ]\n", pos0.x, pos1.y);
                printf("pos1 [%i , %i ]\n", pos1.x, pos1.y);

                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                done = true;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done)
            break;

        if(redraw && al_is_event_queue_empty(queue)){
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_rectangle(BOARD_W,BOARD_H,BOARD_W+JEWEL_PIX*8,BOARD_H+JEWEL_PIX*8,al_map_rgb(255,255,255),1);        

            for(int i= 0 ; i<8;i++){
                for(int j= 0 ; j<8;j++){
                    if(b[i][j]){
                        al_draw_filled_rectangle(BOARD_W+JEWEL_PIX*i-5,BOARD_H+JEWEL_PIX*j-5,BOARD_W+JEWEL_PIX*(i+1)-5,BOARD_H+JEWEL_PIX*(j+1)-5,al_map_rgba_f(0.2,0.2,0.2,0.1));        
                    }
                    switch(board[j][i]){
                        case RED : 
                            al_draw_bitmap(red, BOARD_W+JEWEL_PIX*i,BOARD_H+JEWEL_PIX*j, 0);
                            break;
                        case BLUE :
                                al_draw_bitmap(blue, BOARD_W+JEWEL_PIX*i,BOARD_H+JEWEL_PIX*j, 0);
                            break;
                        case YELLOW : 
                                al_draw_bitmap(yellow, BOARD_W+JEWEL_PIX*i,BOARD_H+JEWEL_PIX*j, 0);
                            break;
                        case GREEN :
                                al_draw_bitmap(green, BOARD_W+JEWEL_PIX*i,BOARD_H+JEWEL_PIX*j, 0);
                            break;
                        case MAGENTA :
                                al_draw_bitmap(magenta, BOARD_W+JEWEL_PIX*i,BOARD_H+JEWEL_PIX*j, 0);
                            break;
                    }
                }
            }
            al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "X0: %.1f Y0: %.1f", x0, y0);
            al_draw_textf(font, al_map_rgb(255, 255, 255), 50, 50, 0, "X1: %.1f Y1: %.1f", x1, y1);



            al_flip_display();

            redraw = false;
        }
    }

    free_board(board);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
    }