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
int insere(int* score_board, int score){
    int i = 0 , a,b;
    while (i < SCORE_BOARD_SIZE){
        if(score > score_board[i]){
            a = score_board[i];
            score_board[i] = score;
            i++;
            while ( i < SCORE_BOARD_SIZE ){
                b = score_board[i];
                score_board[i] = a; 
                a = b;
                i++;
            }
            break;
        }
        i++;
    }
    return 1;
    
}


int write_scores(char *path, int *score){
    FILE *F;
    F = fopen(path,"w");
    if(F == NULL)
        return 0;

    char *temp = malloc( sizeof(char)*10 );
    if(temp == NULL)
        return NULL; 


    for (int i = 0; i < 5; i++){
        if (score[i] == 0)
            break;
        sprintf(temp, "%i\n",score[i]);      
        fputs(temp,F);
    }
    fclose(F);
    return 1;
}

int* read_scores(char *path){
    FILE *F;
    printf("Reading Score\n");
    F = fopen(path,"r");
    if(F == NULL)
        return NULL;
    printf("Reading Score\n");

    char *temp = malloc( sizeof(char)*10 );
    if(temp == NULL)
        return NULL; 

    int *records = calloc( 5, sizeof(int) );
    if(records == NULL)
        return NULL;
        
    
    for (int i = 0; i < 5; i++){
        if( ( fgets(temp, 10, F) == NULL ))
            break;
        records[i] = atoi(temp);
    }
    fclose(F);
    free(temp);
    return records;
}