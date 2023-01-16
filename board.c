#include "board.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

JEWEL_TYPE** create_board(){
    JEWEL_TYPE **board = (JEWEL_TYPE**)malloc(sizeof(JEWEL_TYPE*)*8);
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

void printf_board(JEWEL_TYPE **board){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            printf("%d ",board[i][j]);
        }
        printf("\n");
    }
}

#define UP  1
#define DOWN -1
#define RIGHT 2
#define LEFT -2

int* estoura(JEWEL_TYPE **board, int row, int column,int direction, JEWEL_TYPE jt){
    /*   res = [Esquerda,cima,baixo,direita]  (0 não estoura, 1 estoura na direção ) */
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

JEWEL_TYPE rand_jewel(){
    return rand()%5;
}

void switch_jewels(JEWEL_TYPE **board,int r1, int c1,int r2, int c2){
    JEWEL_TYPE temp = board[r1][c1];
    board[r1][c1] = board[r2][c2];
    board[r2][c2] = temp;
}

void cai(JEWEL_TYPE **board,int row, int column){
    /* Move todos os espaços vazios para o topo*/
    for (int i = row; i < 8; i++){
        if(board[i][column] != EMPTY)
            break;

        for (int j = i; j-1 > -1; j--)
            switch_jewels(board,j,column,j-1,column);
        
        /* Preenche os espaços vazio */
        board[0][column] = rand_jewel();
    }
}

void destroi(JEWEL_TYPE **board, int row, int column,int directions[],JEWEL_TYPE jt){
    int i;
    /* Esquerda */
    if (directions[0]){
        for (i = column; i > -1 ; i--){
            if (board[row][i] != jt)
                break;
            board[row][i] = EMPTY;    
            cai(board,row,i);
        }
    }
    
    /* Cima     */    
    if (directions[1]){
        for (i = row; i > -1 ; i--){
            if (board[i][column] != jt)
                break;
            board[i][column] = EMPTY;    
        }
        cai(board,i,column);
    }
    
    /* Baixo    */    
    if (directions[2]){
        for (i = row; i < 8; i++){
            if (board[i][column] != jt)
                break;
            board[i][column] = EMPTY;    
        }
        cai(board,i,column);
    }

    /* Direita  */
    if (directions[3]){
        for (int i = column; i < 8; i++){
            if (board[row][i] != jt)
                break;
            board[row][i] = EMPTY;
            cai(board,row,i);
        }
    }
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

    destroi(board,n_row,n_col,estoura_res,jt);
    if(estoura_res = estoura(board,row,column,-1*direction,board[row][column]))
        destroi(board,row,column,-1*direction,board[row][column]);
    
    return 1;
}

/* Verifica trio na vertical */
int check_trio_vertical(JEWEL_TYPE **board, int row, int column){
    JEWEL_TYPE jt = board[row][column];
    if (row-1 > -1 && row+1 < 8 && jt == board[row+1][column] && jt == board[row-1][column]){
        return 1;
    }
    else{
        /* Verifica direita */
        if (row+2 < 8 && jt == board[row+1][column] && jt == board[row+2][column])
            return 1;
        /* Verifica esquerda */
        if (row-2 > -1 && jt == board[row-1][column] && jt == board[row-2][column])
            return 1;
    }   
    return 0;
}

int check_trio_horizontal(JEWEL_TYPE **board, int row, int column){
    JEWEL_TYPE jt = board[row][column];
    if (column-1 > -1 && column+1 < 8 && jt == board[row][column+1] && jt == board[row][column-1]){
        return 1;
    }
    else{
        /* Verifica reto Direira */
        if (column+2 < 8 && jt == board[row][column+1] && jt == board[row][column+2]){
            return 1;
        }
        /* Verifica reto Esquerda */
        if (column-2 > -1 && jt == board[row][column-1] && jt == board[row][column-2]){
            return 1;
        }
    }   
    return 0;
}

int break_trio(JEWEL_TYPE **board, int row, int column){
    for (int i = 0; i < 5; i++){
        if ( row + 1 > 7 || i != board[row+1][column] )
            if ( column + 1 > 7 || i != board[row][column+1] )
                if ( row - 1 < 0 || i != board[row-1][column] )
                    if ( column - 1 < 0 || i != board[row][column-1] )
                        board[row][column] = i;          
    }
    return 0;
}

/* Garante que não exista nenhum trio ja formado */
int validate_start(JEWEL_TYPE **board){
    /*
        Como a busca por trios é feita: 
            X = ignorado
            V = Verifica na vertical
            H = Verifica na horizontal
            A = Verifica em ambos (H e V) 

        X X H X X H X X 
        X X H X X H X X 
        V V A V V A V V 
        X X H X X H X X 
        X X H X X H X X 
        V V A V V A v V 
        X X H X X H X X 
        X X H X X H X X 
    
    */
    
    for (int i = 0; i < 8; i++){
        if(check_trio_horizontal(board,i,2))
            break_trio(board,i,2);
        if(check_trio_horizontal(board,i,5))
            break_trio(board,i,5);
        if(check_trio_vertical(board,2,i))
            break_trio(board,2,i);
        if(check_trio_vertical(board,5,i))
            break_trio(board,5,i);
    }
    return 1;
}

/* Verifica se não existe nenhum movimento valido*/
int check_end(JEWEL_TYPE **board){
    /*
        Como a verificaçao é feita:

            Verifica de 3 em 3 posições buscando os seguintes casos,
                Sendo X joias da mesma cor e - Uma joia de cor diferente de X
                Caso 1.0) [-XX]
                Caso 1.1) [XX-]
                Caso 2) [X-X]

            Se nenhum caso for encontrado não existe jogada valida
            Se um dos casos for encontrado é necessario uma segunda verificação
                Sendo X joias da mesma cor e e - Uma joia de cor diferente de X 
                O são locais a serem verificados:

                Caso 1.0)
                     O   O
                   O[-XX]-O
                     O   O
                Caso 1.1)
                    O   O
                   O-[XX-]O
                    O   O
                Caso 2)
                     O
                   [X-X]    
                     O
            Se todos os O forem de cores diferentes da cor de X então não existe jogada
    */

    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 6; j++){
            /*                     Horizontal                     */
            /* Caso 1.0*/
            if ( board[i][j] == board[i][j+1]){
                    /* Lado Esquerdo */
                    if (i-1 >-1 && j-1 > -1 && board[i][j]== board[i-1][j-1])
                        return 0;   
                    if (i+1 < 8 && j-1 > -1 && board[i][j]== board[i+1][j-1])
                        return 0;   
                    if (j-2 > -1 && board[i][j]== board[i][j-2])
                        return 0;
                    /* Lado Direito  */                    
                    if (i-1 >-1 && board[i][j]== board[i-1][j+2])
                        return 0;   
                    if (i+1 < 8 && board[i][j]== board[i+1][j+2])
                        return 0;   
                    if (j+3 < 8 && board[i][j]== board[i][j+3])
                        return 0;                    
            }
            else{
                /*Caso 1.1*/
                if(board[i][j+1] == board[i][j+2]){
                    /* Lado Esquerdo */
                    if (i-1 >-1 && board[i][j]== board[i-1][j])
                        return 0;   
                    if (i+1 < 8 && board[i][j]== board[i+1][j])
                        return 0;   
                    if (j-1 > -1 && board[i][j]== board[i][j-1])
                        return 0;
                    /* Lado Direito  */                    
                    if (i-1 >-1 && j+3 < 8 && board[i][j]== board[i-1][j+3])
                        return 0;   
                    if (i+1 < 8 && j+3 < 8 && board[i][j]== board[i+1][j+3])
                        return 0;   
                    if (j+4 < 8 && board[i][j]== board[i][j+4])
                        return 0;      
                }
                else{
                    /* Caso 2*/
                    if (board[i][j] == board[i][j+2]){
                        if (i+1 < 8 && board[i][j]== board[i+1][j+1])
                            return 0;
                        if (i-1 >-1  && board[i][j]== board[i-1][j+1])
                            return 0;   
                    }
                }   
            } 
            /*                     Vertical                       */
            /* Caso 1.0*/
            if ( board[j][i] == board[j+1][i]){
                    /* Lado de Cima */
                    if (i-1 >-1 && j-1 > -1 && board[j][i] == board[j-1][i-1])
                        return 0;   
                    if (i+1 < 8 && j-1 > -1 && board[j][i] == board[j-1][i+1])
                        return 0;   
                    if (j-2 > -1 && board[j][i] == board[j-2][i])
                        return 0;
                    /* Lado de baixo  */                    
                    if (i-1 >-1 && board[j][i] == board[j+2][i-1])
                        return 0;   
                    if (i+1 < 8 && board[j][i] == board[j+2][i+1])
                        return 0;   
                    if (j+3 < 8 && board[j][i] == board[j+3][i])
                        return 0;                    
            }
            else{
                /*Caso 1.1*/
                if( board[j+1][i] == board[j+2][i]){
                    /* Lado de Cima */
                    if (i-1 >-1 && board[j+1][i] == board[j][i-1])
                        return 0;   
                    if (i+1 < 8 && board[j+1][i] == board[j][i+1])
                        return 0;   
                    if (j-1 > -1 && board[j+1][i] == board[j-1][i])
                        return 0;
                    /* Lado de Baixo  */                    
                    if (i-1 >-1 && j+3 < 8 && board[j+1][i] == board[j+3][i-1])
                        return 0;   
                    if (i+1 < 8 && j+3 < 8 && board[j+1][i] == board[j+3][i+1])
                        return 0;   
                    if (j+4 < 8 && board[j+1][i]== board[j+4][i])
                        return 0;      
                }
                else{
                    /* Caso 2*/
                    if (board[j][i] == board[j+2][i]){
                        if (i+1 < 8 && board[j][i]== board[j+1][i+1])
                            return 0;
                        if (i-1 >-1  && board[j][i]== board[j+1][i-1])
                            return 0;   
                    }
                }   
            } 
        
        
        }
    }
   
    return 1;
   
}