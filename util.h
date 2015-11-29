#pragma once
#include "rule.h"
/*
 * util functions such as
 *      - printing
 *      - debugging
 */

void print_board_move(Board_Stat bs, int side, int r, int c, int rr, int cc);

// util function checking if index is out of the chess board
bool check_border(int r, int c);
void print_check(Board_Stat bs);

