#include <stdio.h>
#include <stdlib.h>

#include "board.h"

int main(){
    int row, collum, direction;
    char *t = malloc(sizeof(char)*4);
    JEWEL_TYPE **b = create_board();
    fill_board(b);
    validate_start(b);
    printf_board(b);
    while (!check_end(b)){
        printf("De o seu movimento: Row / Collum / Direction\n");
        
        scanf("%d",&row);
        scanf("%d",&collum);
        scanf("%s",t);
        switch(t[2]){
            case 'D':
                direction = -2;
                break;
            case 'C':
                direction = 2;
                break;
            case 'B':
                direction = -1;
                break;
            case 'A':
                direction = 1;
                break;
            default:
                printf("%c\n",t[2]);
                return 1;
        }
        valid_move(b,row,collum,direction,b[row][collum]);

        system("clear");
        printf_board(b);
    }
    free(t);
    
    return 0;
}