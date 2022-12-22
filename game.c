#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "auxiliar.h"
#include "board.h"


#define DISPLAY_W 960
#define DISPLAY_H 600


#define BORDER_W 400
#define BORDER_H 75
#define BORDER_S 500

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
            //al_draw_rectangle(BORDER_W,BORDER_H,BORDER_W+BORDER_S,BORDER_H+BORDER_S,al_map_rgb(200, 0, 0),1);
            //al_draw_rectangle(300,75,750,525,al_map_rgb(200, 0, 0),1);
            draw_board(300,75,750,525,board);



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