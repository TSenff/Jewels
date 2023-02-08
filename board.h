#ifndef BOARD_H_
#define BOARD_H_

typedef enum JEWEL_TYPE{
    EMPTY = -1,
    RED = 0,
    YELLOW,
    BLUE,
    GREEN,
    MAGENTA
}JEWEL_TYPE;


JEWEL_TYPE** create_board();

void switch_jewels(JEWEL_TYPE **board,int r1, int c1,int r2, int c2);
int check_trio_vertical(JEWEL_TYPE **board, int row, int column);
int check_trio_horizontal(JEWEL_TYPE **board, int row, int column);
int* estoura(JEWEL_TYPE **board, int row, int column,int direction, JEWEL_TYPE jt);
void cai(JEWEL_TYPE **board,int *info_queda);
void update_board(JEWEL_TYPE **board, JEWEL_TYPE ** current);
JEWEL_TYPE rand_jewel();
void refill(JEWEL_TYPE **board, int *info_queda);
int resolve_movement(JEWEL_TYPE **b);
int *seek_and_destroy(JEWEL_TYPE **b);

void free_board(JEWEL_TYPE **board);

void fill_board(JEWEL_TYPE **board);
JEWEL_TYPE ** copy_board(JEWEL_TYPE **board);
int* destroi(JEWEL_TYPE **board, int row, int column,int directions[],JEWEL_TYPE jt);

int min(int a, int b);

void draw_board(float x1, float y1, float x2, float y2, int **board);

void check_move(JEWEL_TYPE **board);

int valid_move(JEWEL_TYPE **board, int row, int column, int direction,JEWEL_TYPE jt);

int check_end(JEWEL_TYPE **board);

void printf_board(JEWEL_TYPE **board);

int validate_start(JEWEL_TYPE **board);

#endif