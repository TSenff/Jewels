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


enum {NEUTRO, TROCA, DESTROI, RETROCA, QUEDA, REFILL, FIM} state ;


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
    ALLEGRO_BITMAP** jewel_bitmap = malloc(sizeof(ALLEGRO_BITMAP*)*5);
    if(jewel_bitmap == NULL)
        return 1;
    
    jewel_bitmap[RED] = al_load_bitmap("Assets/red_jewel_no_bkg.png");
    jewel_bitmap[BLUE]= al_load_bitmap("Assets/blue_jewel_no_bkg.png");   
    jewel_bitmap[GREEN]= al_load_bitmap("Assets/green_jewel_no_bkg.png"); 
    jewel_bitmap[YELLOW]= al_load_bitmap("Assets/yellow_jewel_no_bkg.png");  
    jewel_bitmap[MAGENTA]= al_load_bitmap("Assets/magenta_jewel_no_bkg.png");

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
    coord pos0, pos1, pos_aux;
    pos0.x = 0;
    pos0.y = 0;

    pos1.x = 0;
    pos1.y = 0;

    /* Auxiliar de Movimento */
    int direction = 0;

    /* animação */
    int an_counter = 0;

    /* Tabuleiro */
    int **b;
    b = malloc(sizeof(int*)*8);
    for(int i = 0; i < 8; i++)
        b[i] = calloc(8,sizeof(int));

    /* Flags */
    state = NEUTRO; 

    /* Loop*/
    al_start_timer(timer);
    while(1){
        al_wait_for_event(queue, &event);

        switch(event.type){
            case ALLEGRO_EVENT_TIMER:
                switch (state){
                /* Movimento  */
                case  DESTROI:
                
                    if( check_trio_horizontal(board,pos1.y,pos1.x) || check_trio_vertical(board,pos1.y,pos1.x) || check_trio_horizontal(board,pos0.y,pos0.x) || check_trio_vertical(board,pos0.y,pos0.x) ){
                    /* Reset variaveis de click */
                        pos0.x = 0;
                        pos0.y = 0;
                        pos1.x = 0;
                        pos1.y = 0;
                        click  = 0;
                        redraw = true;
                        state  = NEUTRO;
                    }
                    else{
                        state  = RETROCA;
                        #ifdef DEBUGGER
                        printf("Retroca\n");
                        printf("pos0 [%i , %i ]\n", pos0.y, pos0.x);
                        printf("pos1 [%i , %i ]\n", pos1.y, pos1.x);
                        printf("Direction %i\n",direction);
                        printf("Color %i\n",board[pos0.y][pos0.x]);
                        #endif
                    }
                    /* Reset An_count*/
                    redraw = false;
                    an_counter = 0;
                    break;
                /* Faz a troca */
                case RETROCA:
                    #ifdef DEBUGGER
                    printf("RE");
                    #endif
                case TROCA:
                    #ifdef DEBUGGER                
                    printf("TROCA\n");
                    #endif
                    /* ----------- Animação --------*/
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    al_draw_rectangle(BOARD_W,BOARD_H,BOARD_W+JEWEL_PIX*8,BOARD_H+JEWEL_PIX*8,al_map_rgb(255,255,255),1);        
                    /* Desenha Marcador de Joia */
                    if(click)
                        al_draw_filled_rectangle(BOARD_W+JEWEL_PIX*(pos0.x)-5,BOARD_H+JEWEL_PIX*(pos0.y)-5,BOARD_W+JEWEL_PIX*(pos0.x+1)-5,BOARD_H+JEWEL_PIX*(pos0.y+1)-5,al_map_rgba_f(0.2,0.2,0.2,0.1));        

                    /* Desenha todas as joias menos as que estão se movendo */
                    for(int i= 0 ; i<8;i++){
                        for(int j= 0 ; j<8;j++){
                            if( (i != pos0.x || j != pos0.y) && (i != pos1.x || j != pos1.y) )
                                draw_jewel(i,j,jewel_bitmap,board[j][i]);
                        }
                    }
                    
                    an_counter= an_counter+2;
                    if (state == TROCA){
                        draw_jewel_move(pos0, (-1)*direction/2, direction % 2  ,jewel_bitmap, board[pos0.y][pos0.x],&an_counter);
                        draw_jewel_move(pos1, direction/2, (-1)*direction % 2  ,jewel_bitmap, board[pos1.y][pos1.x],&an_counter);
                    }
                    else{
                        draw_jewel_move(pos0, (-1)*direction/2, direction % 2  ,jewel_bitmap, board[pos0.y][pos0.x],&an_counter);
                        draw_jewel_move(pos1, direction/2, (-1)*direction % 2  ,jewel_bitmap, board[pos1.y][pos1.x],&an_counter);
                    }
                    

                    if(an_counter >= JEWEL_PIX){
                        switch_jewels(board,pos0.y,pos0.x,pos1.y,pos1.x);
                        if (state == RETROCA){
                            #ifdef DEBUGGER
                            printf("Retroca -> Neutro\n");
                            #endif
                            
                            pos0.x = 0;
                            pos0.y = 0;
                            pos1.x = 0;
                            pos1.y = 0;
                            click  = 0;
                            an_counter = 0;
                            state = NEUTRO;
                        }
                        else{
                            #ifdef DEBUGGER
                            printf("Troca -> Destroi\n");
                            #endif

                            state = DESTROI;
                        }
                        
                    }

                    al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "X0: %.1f Y0: %.1f", x0, y0);
                    al_draw_textf(font, al_map_rgb(255, 255, 255), 50, 50, 0, "X1: %.1f Y1: %.1f", x1, y1);
                    al_flip_display();

                    redraw = false;
                    break;
                case QUEDA : 
                    break;
                case FIM : 
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
                                direction = 2 * (pos1.x-pos0.x);
                                state = TROCA;
                            }
                            else{
                                /* Vertical */
                                if(abs(pos0.y-pos1.y) == 1 && pos0.x == pos1.x ){
                                    #ifdef DEBUGGER
                                    printf("Vertical\n");
                                    #endif
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
            /* Desenha Marcador de Joia */
            if(click)
                al_draw_filled_rectangle(BOARD_W+JEWEL_PIX*(pos0.x)-5,BOARD_H+JEWEL_PIX*(pos0.y)-5,BOARD_W+JEWEL_PIX*(pos0.x+1)-5,BOARD_H+JEWEL_PIX*(pos0.y+1)-5,al_map_rgba_f(0.2,0.2,0.2,0.1));        
            
            /* Desenha todas as joias */
            for(int i= 0 ; i<8;i++){
                for(int j= 0 ; j<8;j++){
                    draw_jewel(i,j,jewel_bitmap,board[j][i]);
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