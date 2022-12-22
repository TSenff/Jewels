#include <stdio.h>
#include <stdlib.h>

#include "board.h"

int main(){
    int **b = create_board();
    fill_board(b);
    printf_board(b);
    return 0;
}