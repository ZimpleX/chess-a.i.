#pragma once
#include "rule.h"
#include <stack>

#define NEGA_INFINITY -10000000
#define POSI_INFINITY 10000000

/*
 * High level description of the alpha-beta pruning:
 * Property after pruning:
 *      --  MAX node must be smaller than all the child MAX nodes
 *      --  MIN node must be larger than all the child MIN nodes
 * So we keep 2 global variables cur_min, cur_max as a threshold for pruning subtree
 * at any time, you can either prune max or min, not both. So you cannot apply both
 * variable at the same time --> one of them has to be set invalid
 */
// extern int cur_max = NEGA_INFINITY;
// extern int cur_min = POSI_INFINITY;
extern int search_depth;
extern int ai_next_start_r, ai_next_start_c, ai_next_end_r, ai_next_end_c;

/*
 * algo for chess A.I:
 *      - alpha-beta pruning
 *      - cutoff search
 *      - cost evaluation
 */

/*
 * ai is doing DFS (with certain cutoff criteria),
 * so we need this global variable in order to be able to rolling back
 */
extern std::stack<Board_Stat> ai_board_trace;

/*
 * stopping criteria for search tree
 */
bool stop_search();

/*
 * evaluate cost based on current board configuration
 * evaluation is making use of the info in ai_board_trace
 * return
 *      high value if AI is gaining advantage
 *      low value if AI is put into disadvantage
 */
int eval_boad(int side);

int ai_pre_move(Board_Stat cur_bs, int side, int alpha, int beta);
int sweep_straight(Board_Stat cur_bs, int side, int r, int c);
int sweep_diagonal(Board_Stat cur_bs, int side, int r, int c);

