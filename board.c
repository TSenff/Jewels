#include "board.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COLRED   "\x1B[31m"
#define COLGRE   "\x1B[32m"
#define COLYEL   "\x1B[33m"
#define COLLBL   "\x1B[34m"
#define COLMAG   "\x1B[35m"
#define RESET "\x1B[37m"//"\x1B[0m"

#define UP  1
#define DOWN -1
#define RIGHT 2
#define LEFT -2

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

JEWEL_TYPE ** copy_board(JEWEL_TYPE **board){
    JEWEL_TYPE ** b_copy;
    b_copy = create_board();
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            b_copy[i][j]=board[i][j];
        }
    }   
    return b_copy;
}

void update_board(JEWEL_TYPE **board, JEWEL_TYPE ** current){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            board[i][j]=current[i][j];
        }
    }   
}



void free_board(JEWEL_TYPE **board){
    for (int i = 0; i < 8; i++)
        free(board[i]);
    free(board);
}

JEWEL_TYPE rand_jewel(int level){
    return rand()%(5+level);
}


void fill_board(JEWEL_TYPE **board){
    srand(time(NULL)/*0*/);
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            board[j][i]=rand_jewel(0);
        }
    }
}

void printf_board(JEWEL_TYPE **board){
    for (int i = 0; i < 8; i++){
        printf("               #|");
        printf("-------------------------------|\n");
        printf("               %i",i);

        for (int j = 0; j < 8; j++){
            switch(board[i][j]){
                case RED:
                    printf(RESET "|");
                    printf(COLRED " %d ",board[i][j]);
                    break;
                case YELLOW:
                    printf(RESET "|");
                    printf(COLYEL " %d ",board[i][j]);
                    break;
                case BLUE:
                    printf(RESET "|");
                    printf(COLLBL " %d ",board[i][j]);
                    break;
                case GREEN:
                    printf(RESET "|");
                    printf(COLGRE " %d ",board[i][j]);
                    break;
                case MAGENTA:
                    printf(RESET "|");
                    printf(COLMAG " %d ",board[i][j]);
                    break;
                default:
                    printf(RESET "|");
                    printf(RESET " %d ",board[i][j]);
            }
        }
        printf(RESET "|\n" );
    }
    printf("                |-------------------------------|\n");
    printf("                # 0 # 1 # 2 # 3 # 4 # 5 # 6 # 7 #\n");
    printf("\n");

}

/* Verifica trio na vertical */
int check_trio_vertical(JEWEL_TYPE **board, int row, int column){
    JEWEL_TYPE jt = board[row][column];
    if (jt == EMPTY)
        return 0;
    
    if (row-1 > -1 && row+1 < 8 && jt == board[row+1][column] && jt == board[row-1][column]){
        return 1;
    }
    else{
        /* Verifica baixo */
        if (row+2 < 8 && jt == board[row+1][column] && jt == board[row+2][column])
            return 2;
        /* Verifica cima */
        if (row-2 > -1 && jt == board[row-1][column] && jt == board[row-2][column])
            return 3;
    }   
    return 0;
}

int check_trio_horizontal(JEWEL_TYPE **board, int row, int column){
    JEWEL_TYPE jt = board[row][column];
    if (jt == EMPTY)
        return 0;    
    if (column-1 > -1 && column+1 < 8 && jt == board[row][column+1] && jt == board[row][column-1]){
        return 1;
    }
    else{
        /* Verifica reto Direira */
        if (column+2 < 8 && jt == board[row][column+1] && jt == board[row][column+2]){
            return 2;
        }
        /* Verifica reto Esquerda */
        if (column-2 > -1 && jt == board[row][column-1] && jt == board[row][column-2]){
            return 3;
        }
    }   
    return 0;
}

int* estoura(JEWEL_TYPE **board, int row, int column,int direction, JEWEL_TYPE jt){
    /*   res = [Esquerda,cima,baixo,direita]  (0 não estoura, 1 estoura na direção ) */
    int *res = calloc(4,sizeof(int)*4);
    if (res == NULL)
        return NULL;

    if (jt == EMPTY){
        return res;
    }
    
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
                res[1] = 1;
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
                    res[1] = 1;
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

void switch_jewels(JEWEL_TYPE **board,int r1, int c1,int r2, int c2){
    JEWEL_TYPE temp = board[r1][c1];
    board[r1][c1] = board[r2][c2];
    board[r2][c2] = temp;
}

void refill(JEWEL_TYPE **board, int *info_queda){
    int j;
    for (int i = 0; i < 8; i++){
        j=0;
        while (board[j][i] == EMPTY){
            board[j][i] = rand_jewel(0);    
            j++;
        }
    }
}

int min(int a, int b){
    if( (b != 0 && a > b) || a == 0)
        return b;
    return a;
}

void cai(JEWEL_TYPE **board,int *info_queda){
    /* Move todos os espaços vazios para o topo*/
    for (int i = 0; i < 8; i++){    
        if (info_queda[i]){
            for (int j = info_queda[i]-1; j < 8; j++){
                if ( board[j][i] == EMPTY ){
                    /* Leva o espaço vazio para o topo */
                    for (int k = j; k-1 > -1; k--)
                        switch_jewels(board,k,i,k-1,i);
                }
            }   
        }
        
    }
}

int* destroi(JEWEL_TYPE **board, int row, int column,int directions[],JEWEL_TYPE jt){

    int *queda = calloc(8,sizeof(int));
    if(queda == NULL)
        return NULL;
    
    if (jt == EMPTY){
        return queda;
    }
    
    if ( !(directions[0] || directions[1] || directions[2]|| directions[3]) )
        return queda;



    int i;

    /* Esquerda */
    if (directions[0]){
        for (i = column-1; i > -1 ; i--){
            if (board[row][i] != jt)
                break;
            board[row][i] = EMPTY; 
            queda[i]=row+1;   
        }
    }

    /* Direita  */
    if (directions[3]){

        for (i = column+1; i < 8; i++){

            if (board[row][i] != jt)
                break;
            board[row][i] = EMPTY;
            queda[i]=row+1;   
        }
    }

    
    /* Cima     */    
    if (directions[1]){
        queda[column]=row+1;
        for (i = row-1; i > -1 ; i--){
            if (board[i][column] != jt)
                break;
            board[i][column] = EMPTY;
            queda[column]=i+1;
        }
    }
    
    /* Baixo    */    
    if (directions[2]){

        for (i = row+1; i < 8; i++){
            if (board[i][column] != jt)
                break;
            board[i][column] = EMPTY;   
        }
    }

    if (!directions[1])
        queda[column] = row+1;
    
    board[row][column] = EMPTY;
    return queda;   
}


int* destroi_pontos(JEWEL_TYPE **board, int row, int column,int directions[],JEWEL_TYPE jt, int *points){

    int *queda = calloc(8,sizeof(int));
    if(queda == NULL)
        return NULL;
    
    if (jt == EMPTY){
        return queda;
    }
    
    if ( !(directions[0] || directions[1] || directions[2]|| directions[3]) )
        return queda;



    int i;

    /* Esquerda */
    if (directions[0]){
        for (i = column-1; i > -1 ; i--){
            if (board[row][i] != jt)
                break;
            board[row][i] = EMPTY; 
            queda[i]=row+1; 
            *points+=10;  
        }
    }

    /* Direita  */
    if (directions[3]){

        for (i = column+1; i < 8; i++){

            if (board[row][i] != jt)
                break;
            board[row][i] = EMPTY;
            queda[i]=row+1;   
            *points+=10;  
        }
    }

    
    /* Cima     */    
    if (directions[1]){
        queda[column]=row+1;
        for (i = row-1; i > -1 ; i--){
            if (board[i][column] != jt)
                break;
            board[i][column] = EMPTY;
            queda[column]=i+1;
            *points+=10;  
        }
    }
    
    /* Baixo    */    
    if (directions[2]){

        for (i = row+1; i < 8; i++){
            if (board[i][column] != jt)
                break;
            board[i][column] = EMPTY;   
            *points+=10; 
        }
    }

    if (!directions[1])
        queda[column] = row+1;
    
    board[row][column] = EMPTY;
    *points+=10;  

    return queda;   
}

void res_destroi_cons(int *res1 , int *res2){

    for(int i = 0; i < 8; i++){
            if( (res2[i] != 0 && res1[i] == 0) || (res2[i] != 0 && (res1[i] > res2[i])) )
                res1[i] = res2[i];
    }
}


/*


int *seek_and_destroy(JEWEL_TYPE **b, int *pontos){


    int *res_queda = calloc(8,sizeof(int));
    if(res_queda == NULL)
        return NULL;

    int *direction_res = calloc(4,sizeof(int));
    if(direction_res == NULL)
        return NULL;

    int *res_aux, direction;

    for (int i = 0; i < 8; i++){
        if( (direction = check_trio_horizontal(b,i,2)) ){
            direction_res[0] = (direction == 2) ? 0 : 1 ;
            direction_res[3] = (direction == 3) ? 0 : 1 ;

            res_aux = destroi_pontos(b,i,2,direction_res,b[i][2],pontos);
            res_destroi_cons(res_queda, res_aux);
            free(res_aux);
        }
        if( (direction = check_trio_horizontal(b,i,5)) ){
            direction_res[0] = (direction == 2) ? 0 : 1 ;
            direction_res[3] = (direction == 3) ? 0 : 1 ;

            res_aux = destroi_pontos(b,i,5,direction_res,b[i][5],pontos);
            res_destroi_cons(res_queda, res_aux);
            free(res_aux);
        }
        if( (direction = check_trio_vertical(b,2,i)) ){
            direction_res[1] = (direction == 2) ? 0 : 1 ;
            direction_res[2] = (direction == 3) ? 0 : 1 ;

            res_aux = destroi_pontos(b,2,i,direction_res,b[2][i],pontos);
            res_destroi_cons(res_queda,res_aux);
            free(res_aux);
        }
        if( (direction = check_trio_vertical(b,5,i)) ){
            
            direction_res[1] = (direction == 2) ? 0 : 1 ;
            direction_res[2] = (direction == 3) ? 0 : 1 ;

            res_aux = destroi_pontos(b,5,i,direction_res,b[5][i],pontos);
            res_destroi_cons(res_queda,res_aux);
            free(res_aux);
        }

    }

    free(direction_res);
    return res_queda;
}




*/


int *seek_and_destroy(JEWEL_TYPE **b, JEWEL_TYPE **b_aux, int *pontos){

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

    int *res_queda = calloc(8,sizeof(int));
    if(res_queda == NULL)
        return NULL;

    int *direction_res = calloc(4,sizeof(int));
    if(direction_res == NULL)
        return NULL;

    int *res_aux, direction;
    for (int n = 2; n <=5 ; n+=3){
        for (int i = 0; i < 8; i++){
            if( (direction = check_trio_horizontal(b,i,n)) ){
                direction_res[0] = (direction == 2) ? 0 : 1 ;
                direction_res[3] = (direction == 3) ? 0 : 1 ;

                res_aux = destroi_pontos(b_aux,i,n,direction_res,b[i][n],pontos);
                res_destroi_cons(res_queda, res_aux);
                free(res_aux);
            }
            if( (direction = check_trio_vertical(b,n,i)) ){
                direction_res[1] = (direction == 2) ? 0 : 1 ;
                direction_res[2] = (direction == 3) ? 0 : 1 ;

                res_aux = destroi_pontos(b_aux,n,i,direction_res,b[n][i],pontos);
                res_destroi_cons(res_queda,res_aux);
                free(res_aux);
            }

        }
    }
    free(direction_res);
    return res_queda;
}




/*
int resolve_movement(JEWEL_TYPE **b){
    int i = 0;
    int *res_queda;
    res_queda = seek_and_destroy(b,&i);
    if(res_queda[0]  || res_queda[1] || res_queda[2] || res_queda[3] || res_queda[4] || res_queda[5] || res_queda[6] || res_queda[7] ){
        //printf_board(b);
        cai(b, res_queda);
        refill(b, res_queda);
        resolve_movement(b);
    }
    free(res_queda);
    return 0;
}
*/


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
            Essa verificação é feita tanto horizontalmente quanto verticalmente
    */

    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 6; j++){
            printf("[%i, %i]", i, j);
            /*                     Horizontal                     */
            /*        caso 1.1        */
            if ( board[i][j] == board[i][j+1]){
                printf("Verificando caso 1.1 na Horizontal em [%i,%i]\n", i , j );
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
                /*Caso 1.0*/
                if(board[i][j+1] == board[i][j+2]){
                    printf("Verificando caso 1.0 na Horizontal em [%i,%i]\n", i , j );
                    /* Lado Esquerdo */
                    if (i-1 >-1 && board[i][j+1]== board[i-1][j])
                        return 0;   
                    if (i+1 < 8 && board[i][j+1]== board[i+1][j])
                        return 0;   
                    if (j-1 > -1 && board[i][j+1]== board[i][j-1])
                        return 0;
                    /* Lado Direito  */                    
                    if (i-1 >-1 && j+3 < 8 && board[i][j+1]== board[i-1][j+3])
                        return 0;   
                    if (i+1 < 8 && j+3 < 8 && board[i][j+1]== board[i+1][j+3])
                        return 0;   
                    if (j+4 < 8 && board[i][j+1]== board[i][j+4])
                        return 0;      
                }
                else{
                    /* Caso 2 */
                    if (board[i][j] == board[i][j+2]){
                        printf("Verificando caso 2.0 na Horizontal em [%i,%i]\n", i , j );
                        if (i+1 < 8 && board[i][j]== board[i+1][j+1])
                            return 0;
                        if (i-1 >-1  && board[i][j]== board[i-1][j+1])
                            return 0;   
                    }
                }   
            } 
            /*                     Vertical           */
            /* Caso 1.0 */
            if ( board[j+1][i] == board[j+2][i]){
                    printf("Verificando caso 1.0 na Vertical em [%i,%i]\n", j , i );
                    /* Lado de cima */
                    if (i-1 >-1 && j-1 > -1 && board[j+1][i] == board[j][i-1])
                        return 0;   
                    if (i+1 < 8 && j-1 > -1 && board[j+1][i] == board[j][i+1])
                        return 0;  
                    if (j-1 > -1 &&  board[j+1][i] == board[j-1][i])
                        return 0;
                    /* Lado de baixo */
                    if (i-1 >-1 && j+3 < 8 && board[j+1][i] == board[j+3][i-1])
                        return 0;   
                    if (i+1 < 8 && j+3 < 8 && board[j+1][i] == board[j+3][i+1])
                        return 0;  
                    if (j+4 < 8 &&  board[j+1][i] == board[j+4][i])
                        return 0;                  
            }
            else{
                /* caso 1.1 */
                if ( board[j][i] == board[j+1][i] ){
                    printf("Verificando caso 1.1 na Vertical em [%i,%i]\n", j , i );
                    /* Lado de cima */
                    if (i-1 >-1 && j-1 > -1 && board[j][i] == board[j-1][i-1])
                        return 0;   
                    if (i+1 < 8 && j-1 > -1 && board[j][i] == board[j-1][i+1])
                        return 0;  
                    if (j-2 > -1 &&  board[j][i] == board[j-2][i])
                        return 0;
                    /* Lado de baixo */
                    if (i-1 >-1 && board[j][i] == board[j+2][i-1])
                        return 0;   
                    if (i+1 < 8 && board[j][i] == board[j+2][i+1])
                        return 0;  
                    if (j+3 < 8 &&  board[j][i] == board[j+3][i])
                        return 0;                                
                }
                else{
                    /*Caso 2*/
                    if( board[j][i] == board[j+2][i] ){
                        printf("Verificando caso 2.0 na Vertical em [%i,%i]\n", j , i );
                        /* Lado de Cima */
                        if (i-1 >-1 && board[j][i] == board[j+1][i-1])
                            return 0;   
                        if (i+1 < 8 && board[j][i] == board[j+1][i+1])
                            return 0;   
                    }
                }
            } 
        
        
        }
    }
   printf("\n");
   return 1;
   
}
