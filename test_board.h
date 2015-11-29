#pragma once

#define EE EMPTY
#define BK BLACK*KING
#define BQ BLACK*QUEEN 
#define BB BLACK*BISHOP
#define BN BLACK*KNIGHT
#define BR BLACK*ROOK
#define BP BLACK*PAWN
#define WK WHITE*KING
#define WQ WHITE*QUEEN
#define WB WHITE*BISHOP
#define WN WHITE*KNIGHT
#define WR WHITE*ROOK
#define WP WHITE*PAWN

#include "rule.h"

/*
 * store the board configuration:
 *      normal board and other testing board
 * indexed by [row][column]
 */

int NORMAL_BOARD[8][8] = {
    {BR, BN, BB, BQ, BK, BB, BN, BR},
    {BP, BP, BP, BP, BP, BP, BP, BP},
    {EE, EE, EE, EE, EE, EE, EE, EE},
    {EE, EE, EE, EE, EE, EE, EE, EE},
    {EE, EE, EE, EE, EE, EE, EE, EE},
    {EE, EE, EE, EE, EE, EE, EE, EE},
    {WP, WP, WP, WP, WP, WP, WP, WP},
    {WR, WN, WB, WQ, WK, WB, WN, WR}
};
