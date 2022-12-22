#ifndef BOARD_H_
#define BOARD_H_

int** create_board();

void free_board(int **board);

void fill_board(int **board);

void draw_board(float x1, float y1, float x2, float y2, int **board);

void check_move(int **board);

void check_end(int **board, int i, int j);

void printf_board(int **board);



#endif