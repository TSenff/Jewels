#ifndef BOARD_H_
#define BOARD_H_

typedef enum JEWEL_TYPE{
    EMPTY = -1,
    RED = 0,
    YELLOW,
    BLUE,
    GREEN
}JEWEL_TYPE;


JEWEL_TYPE** create_board();

void free_board(JEWEL_TYPE **board);

void fill_board(JEWEL_TYPE **board);

void draw_board(float x1, float y1, float x2, float y2, int **board);

void check_move(JEWEL_TYPE **board);

int check_end(JEWEL_TYPE **board);

void printf_board(JEWEL_TYPE **board);

int validate_start(JEWEL_TYPE **board);

#endif