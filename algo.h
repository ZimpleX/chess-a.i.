#pragma once
#include "rule.h"
#include <stack>
/*
 * algo for chess A.I:
 *      - alpha-beta pruning
 *      - cutoff search
 *      - cost evaluation
 */
/*
 * Property after pruning:
 *      --  MAX node must be smaller than all the child MAX nodes
 *      --  MIN node must be larger than all the child MIN nodes
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

#define SEARCH_DEPTH 6
/*
 * cost matrix related to specific positions of the piece.
 * reference from: 
 *      https://chessprogramming.wikispaces.com/Simplified+evaluation+function
 */
const int pawn_mat[8][8] = {
    {0,0,0,0,0,0,0,0},
    {5,10,10,-20,-20,10,10,5},
    {5,-5,-10,0,0,-10,-5,5},
    {0,0,0,20,20,0,0,0},
    {5,5,10,25,25,10,5,5},
    {10,10,20,30,30,20,10,10},
    {50,50,50,50,50,50,50,50},
    {0,0,0,0,0,0,0,0}
};

const int knight_mat[8][8] = {
    {-50,-40,-30,-30,-30,-30,-40,-50},
    {-40,-20,0,5,5,0,-20,-40},
    {-30,5,10,15,15,10,5,-30},
    {-30,0,15,20,20,15,0,-30},
    {-30,5,15,20,20,15,5,-30},
    {-30,0,10,15,15,10,0,-30},
    {-40,-20,0,0,0,0,-20,-40},
    {-50,-40,-30,-30,-30,-30,-40,-50}
};

const int bishop_mat[8][8] = {
    {-20,-10,-10,-10,-10,-10,-10,-20},
    {-10,5,0,0,0,0,5,-10},
    {-10,10,10,10,10,10,10,-10},
    {-10,0,10,10,10,10,0,-10},
    {-10,5,5,10,10,5,5,-10},
    {-10,0,5,10,10,5,0,-10},
    {-10,0,0,0,0,0,0,-10},
    {-20,-10,-10,-10,-10,-10,-10,-20}
};

const int rook_mat[8][8] = {
    {0,0,0,5,5,0,0,0},
    {-5,0,0,0,0,0,0,-5},
    {-5,0,0,0,0,0,0,-5},
    {-5,0,0,0,0,0,0,-5},
    {-5,0,0,0,0,0,0,-5},
    {-5,0,0,0,0,0,0,-5},
    {5,10,10,10,10,10,10,5},
    {0,0,0,0,0,0,0,0}
};

const int queen_mat[8][8] = {
    {-20,-10,-10,-5,-5,-10,-10,-20},
    {-10,0,5,0,0,0,0,-10},
    {-10,5,5,5,5,5,0,-10},
    {0,0,5,5,5,5,0,-5},
    {-5,0,5,5,5,5,0,-5},
    {-10,0,5,5,5,5,0,-10},
    {-10,0,0,0,0,0,0,-10},
    {-20,-10,-10,-5,-5,-10,-10,-20}
};

const int king_mat[8][8] = {
    {20,30,10,0,0,10,30,20},
    {20,20,0,0,0,0,20,20},
    {-10,-20,-20,-20,-20,-20,-20,-10},
    {-20,-30,-30,-40,-40,-30,-30,-20},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30}
};


extern int search_depth;
// r,c are set at the end of the A.I. search, indicating next move chosen
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

