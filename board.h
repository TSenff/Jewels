#ifndef BOARD_H_
#define BOARD_H_

typedef enum JEWEL_TYPE{
    RED = 0,
    YELLOW,
    BLUE,
    GREEN
}JEWEL_TYPE;


int** create_board();

void free_board(JEWEL_TYPE **board);

void fill_board(JEWEL_TYPE **board);

void draw_board(float x1, float y1, float x2, float y2, int **board);

void check_move(JEWEL_TYPE **board);

void check_end(JEWEL_TYPE **board, int i, int j);

void printf_board(JEWEL_TYPE **board);



#endif