#include <stdio.h>
#include <stdlib.h>

#include "board.h"

int main(){
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
    
    return 0;
}