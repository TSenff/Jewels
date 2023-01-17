#include <stdio.h>
#include <stdlib.h>

#include "board.h"

int main(){
    /*
    JEWEL_TYPE **b = create_board();
    fill_board(b);
    validate_start(b);
    printf_board(b);
    if (check_end(b)){
        printf("O Jogo acabou \n");
    }
    else{
        printf("O Jogo não acabou \n");
    }
    */
    int row, collum, direction;
    JEWEL_TYPE **b = create_board();
    fill_board(b);
    validate_start(b);
    printf_board(b);
    while (!check_end(b)){
        printf("De o seu movimento: Row / Collum / Direction ");
        
        scanf("%d",&row);
        scanf("%d",&collum);
        scanf("%d",&direction);
        valid_move(b,row,collum,direction,b[row][collum]);
        printf_board(b);
    }
    
    return 0;
}