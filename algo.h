#pragma once
#include "rule.h"
#include <stack>
/*
 * algo for chess A.I:
 *      - alpha-beta pruning
 *      - cutoff search
 *      - cost evaluation
 */

#define NEGA_INFINITY -10000000
#define POSI_INFINITY 10000000

/*
 * coefficients for different pieces
 */
#define KK 20000
#define KQ 900
#define KR 500
#define KB 330
#define KN 320
#define KP 100
#define KP_SPEC -50
#define KMOBILITY 10

#define SEARCH_DEPTH 7

/*
 * Cost matrix related to specific position
 * this is for from the perspective of BLACK (up part)
 */
extern int pawn_mat[8][8];

extern int knight_mat[8][8];

extern int bishop_mat[8][8];

extern int rook_mat[8][8];

extern int queen_mat[8][8];

extern int king_mat[8][8];

/*
 * High level description of the alpha-beta pruning:
 * Property after pruning:
 *      --  MAX node must be smaller than all the child MAX nodes
 *      --  MIN node must be larger than all the child MIN nodes
 */
extern int search_depth;
extern int ai_next_start_r, ai_next_start_c, ai_next_end_r, ai_next_end_c;


void ai_move(Board_Stat *bs);

int ai_pre_move(Board_Stat cur_bs, int side, int alpha, int beta);

int core_prune_recursion(Board_Stat *moved_bs, 
        int side, int r, int c, int rr, int cc, 
        int &alpha, int &beta, int &cur_min, int &cur_max);
int sweep_king(Board_Stat cur_bs, int side, int r, int c,
        int &alpha, int &beta, int &cur_min, int &cur_max);
int sweep_straight(Board_Stat cur_bs, int side, int r, int c,
        int &alpha, int &beta, int &cur_min, int &cur_max);
int sweep_diagonal(Board_Stat cur_bs, int side, int r, int c,
        int &alpha, int &beta, int &cur_min, int &cur_max);
int sweep_knight(Board_Stat cur_bs, int side, int r, int c,
        int &alpha, int &beta, int &cur_min, int &cur_max);
int sweep_pawn(Board_Stat cur_bs, int side, int r, int c,
        int &alpha, int &beta, int &cur_min, int &cur_max);

