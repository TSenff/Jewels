#include "board.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int** create_board(){
    int **board = (JEWEL_TYPE**)malloc(sizeof(JEWEL_TYPE*)*8);
    if ( board == NULL )
        return NULL;    
    for (int i = 0; i < 8; i++){
        board[i] = (JEWEL_TYPE*)malloc(sizeof(JEWEL_TYPE)*8);
        if ( board[i] == NULL )
            return NULL;   
    }
    return board;
}

void free_board(JEWEL_TYPE **board){
    for (int i = 0; i < 8; i++)
        free(board[i]);
    free(board);
}

void fill_board(JEWEL_TYPE **board){
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


#define UP  1
#define DOWN -1
#define RIGHT 2
#define LEFT -2

int* estoura(JEWEL_TYPE **board, int row, int column,int direction, JEWEL_TYPE jt){
    int cont=0, dp = 1;
    /*          [Esquerda,cima,baixo,direita]*/
    int *res = malloc(sizeof(int)*4);

    /* Movimento Horizontal */
    if(direction != UP  && direction != DOWN){
        /* Verifica se esta no meio*/
        if (row-1 > -1 && row+1 < 8 && jt == board[row+1][column] && jt == board[row-1][column]){
            res[1] = 1;
            res[2] = 1;
        }
        else{
            /* Verifica reto baixo */
            if (row+2 < 8 && jt == board[row+1][column] && jt == board[row+2][column])
                res[2] = 1;
            /* Verifica reto Cima */
            if (row-2 > -1 && jt == board[row-1][column] && jt == board[row-2][column])
                res[1] = 2;
        }
    }
    /* Movimento Vertical */
    else{
        /* Verifica se esta no meio */
        if (column-1 > -1 && column+1 < 8 && jt == board[row][column+1] && jt == board[row][column-1]){
            res[0] = 1;
            res[3] = 1;
        }
        else{
            /* Verifica reto Direira */
            if (column+2 < 8 && jt == board[row][column+1] && jt == board[row][column+2]){
                res[3] = 1;
            }
            /* Verifica reto Esquerda */
            if (column-2 > -1 && jt == board[row][column-1] && jt == board[row][column-2]){
                res[0] = 1;
            }
        }

    }

    /* Verifica a unica direçao restante */
    switch (direction){
        case UP:
                if (row-2 > -1 && jt == board[row-1][column] && jt == board[row-2][column])
                    res[1] = 2;
            break;
        case DOWN:
                if (row+2 < 8 && jt == board[row+1][column] && jt == board[row+2][column])
                    res[2] = 1;
            break;
        case RIGHT:
                if (column+2 < 8 && jt == board[row][column+1] && jt == board[row][column+2])
                    res[3] = 1;
            break;
        case LEFT:
                if (column-2 > -1 && jt == board[row][column-1] && jt == board[row][column-2])
                    res[0] = 1;
            break;
    }    
    return res;
       
}

int valid_move(JEWEL_TYPE **board, int row, int column, int direction,JEWEL_TYPE jt){
    /* Verifica se a posição existe */
    int n_col = column, n_row = row;
    switch (direction){
        case UP:
            n_row++;
            break;
        case DOWN:
            n_row--;
            break;
        case RIGHT:
            n_col++;
            break;
        case LEFT:
            n_col--;
            break;
    }    
    if ( (n_col > 7 || n_col < 0 ) && (n_row > 7 || n_row < 0) )
        return 0;
    /* Verifica se existe +3*/
    int *estoura_res = estoura(board,row,column,direction,jt);

    /* Faz a troca */
    int temp  = board[row][column];
    board[row][column] = board[n_row][n_col];
    board[n_row][n_col] = temp;

}

void jewel_move(int **board, int row, int column, int direction){

}

void printf_board(JEWEL_TYPE **board){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            printf("%d ",board[i][j]);
        }
        printf("\n");
    }
}

void jewel_destroi(JEWEL_TYPE **board,int row,int column){
    int cont=0, dp = 1, res = 0;

    /* Horizontal */
    /* Direita */
    while(column+dp < 8 && board[row][column] == board[row][column+dp]){
        cont++;
        dp++;
    }
    /* Esquerda */
    dp = 1;
    while(column-dp >= 0 && board[row][column] == board[row][column-dp]){
        cont++;
        dp++;
    }
    /* Verifica se Mais de 3 joias iguais em sequencia na horizontal */
    if (cont >= 3){
        res = 1;
    }

    /* Vertical */
    dp = 1;
    cont = 0;
    /* Baixo */
    while(column+dp < 8 && board[row][column] == board[row+dp][column]){
        cont++;
        dp++;
    }
    /* Cima */
    dp = 1;
    while(column-dp >= 0 && board[row][column] == board[row-dp][column]){
        cont++;
        dp++;
    }
    /* Verifica se Mais de 3 joias iguais em sequencia na vertical ou na horizontal e na vertical */
    if (cont >= 3){
        if (res == 1)
            res = 3;
        else
            res = 2;
    }
    return res;
}


void check_end(JEWEL_TYPE **board, int i, int j){
}
