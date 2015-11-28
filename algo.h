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
 * High level description of the alpha-beta pruning:
 * Property after pruning:
 *      --  MAX node must be smaller than all the child MAX nodes
 *      --  MIN node must be larger than all the child MIN nodes
 */
extern int search_depth;
extern int ai_next_start_r, ai_next_start_c, ai_next_end_r, ai_next_end_c;


/*
 * ai is doing DFS (with certain cutoff criteria),
 * so we need this global variable in order to be able to rolling back
 */
// extern std::stack<Board_Stat> ai_board_trace;

/*
 * stopping criteria for search tree
 */
// bool stop_search(int side, Board_Stat cur_bs);

/*
 * evaluate cost based on current board configuration
 * evaluation is making use of the info in ai_board_trace
 * return
 *      high value if AI is gaining advantage
 *      low value if AI is put into disadvantage
 */
// int eval_boad(int side, Board_Stat cur_bs);

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

