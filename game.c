#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "auxiliar.h"
#include "board.h"


#define DISPLAY_W  1024//800//
#define DISPLAY_H  768//600//768
#define JEWEL_PIX 70



int main()
{
    /* Inicializando */
    check_init(al_init(), "Allegro");
    check_init(al_install_keyboard(),"Keyboard");
    check_init(al_install_mouse(),"Mouse");
    check_init(al_init_primitives_addon(),"Primitives");
    
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    check_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    check_init(queue, "queue");

    ALLEGRO_DISPLAY* disp = al_create_display(DISPLAY_W, DISPLAY_H);
    check_init(disp, "display");

    ALLEGRO_FONT* font = al_create_builtin_font();
    check_init(font, "font");

    int **board = create_board();
    if(board == NULL){
        return 1;
    }
    fill_board(board);

    /* Eventos */
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());
    
    /* Variaveis e Preparaçoes para o loop */
    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;
    ALLEGRO_MOUSE_STATE m_state;
    float m_x = -1 , m_y = -1 ;
    coord pos;
    pos.x = -1;
    pos.y = -1;

    int **b;
    b = malloc(sizeof(int*)*8);
    for(int i = 0; i < 8; i++)
        b[i] = calloc(8,sizeof(int));

    /* Loop*/
    al_start_timer(timer);
    while(1)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                // game logic goes here.
                redraw = true;
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                printf("Click\n");
                al_get_mouse_state(&m_state);
                //if(pos.x != -1 && pos.y != -1)
                //    b[pos.x][pos.y] = 0;
                m_x = m_state.x;
                m_y = m_state.y;
                pos = click_pos(m_x,m_y);
                printf("Pos [%i , %i ]\n", pos.x, pos.y);

                if(pos.x > -1 && pos.y > -1)
                    b[pos.x][pos.y] = !b[pos.x][pos.y] ;
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

        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            for(int i= 0 ; i<8;i++){
                for(int j= 0 ; j<8;j++){
                    if(b[i][j])
                        al_draw_filled_rectangle(400+JEWEL_PIX*i,50+JEWEL_PIX*j,400+JEWEL_PIX+JEWEL_PIX*i,50+JEWEL_PIX+JEWEL_PIX*j,al_map_rgb(0,0,255));        
                    else{
                        al_draw_rectangle(400+JEWEL_PIX*i,50+JEWEL_PIX*j,400+JEWEL_PIX+JEWEL_PIX*i,50+JEWEL_PIX+JEWEL_PIX*j,al_map_rgb(0,0,255),1);        
                    }
                }
            }
            al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "X: %.1f Y: %.1f", m_x, m_y);
            /* Tabuleio tem 560x560*/
            //al_draw_rectangle(400,50,960,610,al_map_rgb(100,0,0),1);


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