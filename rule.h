#pragma once
/*
 * rules for chess:
 *      - valid move of each element
 *      - current board status
 */

#define BLACK -1
#define WHITE 1

enum Role {KING=1, QUEEN, ROOK, BISHOP, KNIGHT, PAWN};

/*
 * up, down, right, left,
 * up-right, up-left, down-right, down-left.
 */
//enum Dir {UP=1, DN, RG, LF, UR, UL, DR, DL};

/*
 * indicate the role at that location
 * White side: positive
 * Black side: negative
 */
extern int board_stat[8][8];

/*
 * move the roles on board
 * return of move():
 *      true if the move is legal
 *      false if the move is illegal
 *
 * this function will update the board_stat accordingly
 * if the king is eaten, then the program terminates immediately
 */
bool move(int start_x, int start_y, int end_x, int end_y);
