#include "board.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int** create_board(){
    int **board = (int**)malloc(sizeof(int*)*8);
    if ( board == NULL )
        return NULL;    
    for (int i = 0; i < 8; i++){
        board[i] = (int*)malloc(sizeof(int)*8);
        if ( board[i] == NULL )
            return NULL;   
    }
    return board;
}

void free_board(int **board){
    for (int i = 0; i < 8; i++)
        free(board[i]);
    free(board);
}

void fill_board(int **board){
    srand(time(NULL));
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            board[j][i]=rand()%5;
        }
    }
}
/*
void draw_board(float x1, float y1, float x2, float y2, int **board){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            al_draw_filled_rectangle(x1,y1,x2,y2,al_map_rgb(0,0,0));
        }
    }
}
*/

void jewel_move(int **board){

}

void printf_board(int **board){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            printf("%d ",board[i][j]);
        }
        printf("\n");
    }
}



void check_end(int **board, int i, int j){
}
