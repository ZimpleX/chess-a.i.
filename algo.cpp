#include "algo.h"
#include "rule.h"
#include "util.h"
#include <cstdlib>
#include <string>

using namespace std;


int search_depth;
// the chosen next move of a.i. at the end of current search
int ai_next_start_r, ai_next_start_c, ai_next_end_r, ai_next_end_c;


void init_ai_search(int &alpha, int &beta);


/**************************
 *     called by main     *
 **************************/
void ai_move(Board_Stat *bs) {
    int alpha;
    int beta;
    init_ai_search(alpha, beta);
    int chosen_cost = ai_pre_move(*bs, AI, alpha, beta);
    bs->player_move(ai_next_start_r, ai_next_start_c, 
            ai_next_end_r, ai_next_end_c);
}


/**********************************
 *     main body of algorithm     *
 *          -- DFS search         *
 *          -- alpha-beta         *
 **********************************/
/*
 * A.I. is the MAX node, PLAYER is the MIN node
 *
 * alpha: initial NEGA_INFINITY --> MAX node pruning
 * beta: initial POSI_INFINITY  --> MIN node pruning
 */
int ai_pre_move(Board_Stat cur_bs, int side, int alpha, int beta) {
    int cur_min = POSI_INFINITY;    // return value if side is AI
    int cur_max = NEGA_INFINITY;    // return value if side if PLAYER

    search_depth ++;
    if (cur_bs.terminate_test()) {
        search_depth --;
        return cur_bs.eval_board();
    }
    
    int prune_stat; // indicating whether pruning has taken place
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            int role = cur_bs.get_piece(r, c) * side;
            if (role <= 0)
                continue;
            switch (role) {
            // wrapper of all sweep_* functions
                case KING:
                    prune_stat = sweep_king(cur_bs, side, r, c, 
                            alpha, beta, cur_min, cur_max);
                    break;
                case QUEEN:
                    prune_stat = sweep_straight(cur_bs, side, r, c, 
                            alpha, beta, cur_min, cur_max);
                    prune_stat = sweep_diagonal(cur_bs, side, r, c, 
                            alpha, beta, cur_min, cur_max);
                    break;
                case ROOK:
                    prune_stat = sweep_straight(cur_bs, side, r, c, 
                            alpha, beta, cur_min, cur_max);
                    break;
                case BISHOP:
                    prune_stat = sweep_diagonal(cur_bs, side, r, c, 
                            alpha, beta, cur_min, cur_max);
                    break;
                case KNIGHT:
                    prune_stat = sweep_knight(cur_bs, side, r, c,
                            alpha, beta, cur_min, cur_max);
                    break;
                case PAWN:
                    prune_stat = sweep_pawn(cur_bs, side, r, c,
                            alpha, beta, cur_min, cur_max);
                    break;
                default:
                    cout << ">>>>>>>>>> Something is WRONG <<<<<<<<<<\n";
                    exit(1);
                    break;
            } // switch case end
            if (prune_stat != 0) {
                search_depth --;
                return prune_stat;
            }
        }
    }
    search_depth --;
    // return value will affect the parent behavior
    // For example (side == AI):
    //      cur_max < parent_beta, then parent_beta will be updated
    //      cur_max < parent_alpha, then parent node subtree is pruned
    if (side == AI)
        return cur_max;
    else
        return cur_min;
}

/**************************************
 *     update important variables     *
 *          -- alpha / beta           *
 *          -- current node min/max   *
 **************************************
 * Main body of alpha-beta pruning, update alpha beta accordingly
 *
 * MAX node: 
 *      prune beta, update alpha
 * MIN node: 
 *      prune aplta, update beta
 *
 * return
 *      0 if searching should be continued
 *      NEGA_INFINITY: pruned, return to MAX parent
 *      POSI_INFINITY: pruned, return to MIN parent
 */
int ab_pruning(int side, int cur_value, int &alpha, int &beta) {
    if (side == AI) {   // MAX node: alpha
        if (cur_value > beta)
            return POSI_INFINITY;
        if (cur_value > alpha)
            alpha = cur_value;
    } else {            // MIN node: beta
        if (cur_value < alpha)
            return NEGA_INFINITY;
        if (cur_value < beta)
            beta = cur_value;
    }
    return 0;
}

/*
 * update cur_max, cur_min,
 * record move r, c if we are at the root node (can be the A.I. final chosen move)
 */
void update_loc_minmax(int r, int c, int rr, int cc, 
        int cur_cost, int &cur_min, int &cur_max) {
    if (search_depth == 1
     && cur_cost > cur_max) {   // the first ai move
        ai_next_start_r = r;
        ai_next_start_c = c;
        ai_next_end_r = rr;
        ai_next_end_c = cc;
    }
    cur_max = (cur_max > cur_cost) ? cur_max : cur_cost;
    cur_min = (cur_min < cur_cost) ? cur_min : cur_cost;
}


/************************************************
 *      expand tree for specific piece type     *
 ************************************************
 * Core part of the algo: common to all sweep_* functions
 *      -- pruning
 *      -- recursion
 */
int core_prune_recursion(Board_Stat *moved_bs, 
        int side, int r, int c, int rr, int cc, 
        int &alpha, int &beta, int &cur_min, int &cur_max) {
    int op_side = (side==BLACK)?WHITE:BLACK;
    int cur_cost = ai_pre_move(*moved_bs, op_side, alpha, beta);
    int prune_stat = ab_pruning(side, cur_cost, alpha, beta);
    update_loc_minmax(r, c, rr, cc, cur_cost, cur_min, cur_max);
    return prune_stat;
}

// return prune_stat (see ab_pruning())
int sweep_king(Board_Stat cur_bs, int side, int r, int c,
        int &alpha, int &beta, int &cur_min, int &cur_max) {
    int r_s = (r>0)?(r-1):0;
    int r_e = (r<7)?(r+1):7;
    int c_s = (c>0)?(c-1):0;
    int c_e = (c<7)?(c+1):7;
    Board_Stat moved_bs = cur_bs;
    for (int rr=r_s; rr<=r_e; rr++) {
        for (int cc=c_s; cc<=c_e; cc++) {
            // move king
            if(!moved_bs.ai_direct_move(r, c, rr, cc))
                continue;
            int stat = core_prune_recursion(&moved_bs, side, r, c,
                    rr, cc, alpha, beta, cur_min, cur_max);
            if (stat != 0)
                return stat;
            moved_bs = cur_bs;
       }
    }
    return 0;
}

// return prune_stat (see ab_pruning())
int sweep_knight(Board_Stat cur_bs, int side, int r, int c,
        int &alpha, int &beta, int &cur_min, int &cur_max) {
    Board_Stat moved_bs = cur_bs;
    for (int dir = -1; dir <=1; dir += 2) {
        for (int off_r = 1; off_r <= 2; off_r ++) {
            int rr = r + dir*off_r;
            if (rr > 7 || rr < 0)
                continue;
            int off_c = 2 / off_r;
            for (int kc = -1; kc <= 1; kc += 2) {
                int cc = c + kc*off_c;
                if (cc > 7 || cc < 0)
                    continue;
                if (!moved_bs.ai_direct_move(r, c, rr, cc))
                    continue;
                int stat = core_prune_recursion(&moved_bs, side, r, c,
                        rr, cc, alpha, beta, cur_min, cur_max);
                if (stat != 0)
                    return stat;
                moved_bs = cur_bs;
            }
        }
    }
    return 0;
}

// return prune_stat (see ab_pruning())
int sweep_pawn(Board_Stat cur_bs, int side, int r, int c,
        int &alpha, int &beta, int &cur_min, int &cur_max) {
    int r_s = (r>1)?(r-2):0;
    int r_e = (r<6)?(r+2):7;
    int c_s = (c>0)?(c-1):0;
    int c_e = (c<7)?(c+1):7;
    Board_Stat moved_bs = cur_bs;
    for (int rr=r_s; rr<=r_e; rr++) {
        for (int cc=c_s; cc<=c_e; cc++) {
            // move pawn
            if(!moved_bs.ai_pawn_move(r, c, rr, cc))
                continue;
            int stat = core_prune_recursion(&moved_bs, side, r, c,
                    rr, cc, alpha, beta, cur_min, cur_max);
            if (stat != 0)
                return stat;
            moved_bs = cur_bs;
       }
    }
    return 0;
}

// return prune_stat (see ab_pruning())
int sweep_straight(Board_Stat cur_bs, int side, int r, int c, 
        int &alpha, int &beta, int &cur_min, int &cur_max) {
    Board_Stat moved_bs = cur_bs;
    for (int kr = -1; kr <= 1; kr ++) {
        for (int kc = -1; kc <= 1; kc ++) {
            if (kr * kc != 0)
                continue;
            for (int d = 1; d <= 7; d ++) {
                int rr = r + kr*d;
                int cc = c + kc*d;
                if (rr < 0 || rr > 7
                 || cc < 0 || cc > 7)
                    break;
                if (!moved_bs.ai_direct_move(r, c, rr, cc))
                    break;
                int stat = core_prune_recursion(&moved_bs, side, r, c,
                        rr, cc, alpha, beta, cur_min, cur_max);
                if (stat != 0)
                    return stat;
                moved_bs = cur_bs;
                if (cur_bs.get_piece(rr, cc) != EMPTY) // a piece has been taken in the previous move
                    break;

            }
        }
    }
    return 0;
}

// return prune_stat (see ab_pruning())
int sweep_diagonal(Board_Stat cur_bs, int side, int r, int c,
        int &alpha, int &beta, int &cur_min, int &cur_max) {
    Board_Stat moved_bs = cur_bs;
    for (int kr = -1; kr <= 1; kr += 2) {
        for (int kc = -1; kc <= 1; kc += 2) {
            for (int d = 1; d <=7; d++) {
                int rr = r + kr*d;
                int cc = c + kc*d;
                if (rr < 0 || cc < 0
                 || rr > 7 || cc > 7)
                    break;
                if (!moved_bs.ai_direct_move(r, c, rr, cc))
                    break;
                int stat = core_prune_recursion(&moved_bs, side, r, c,
                        rr, cc, alpha, beta, cur_min, cur_max);
                if (stat != 0)
                    return stat;
                moved_bs = cur_bs;
                if (cur_bs.get_piece(rr, cc) != EMPTY) // a piece has been taken in the previous move
                    break;
            }
        }
    }
    return 0;
}

/**************************
 *    Termination test    *
 **************************
 * simple cut-off
 */
bool Board_Stat::terminate_test() {
    if (is_terminate)
        return true;
    if (search_depth >= SEARCH_DEPTH)
        return true;
    else 
        return false;
}

/*******************************
 *     Evaluation function     *
 *   -- piece count (weighted) *
 *   -- pawn pattern detect    *
 *   -- mobility count         *
 *******************************
 * return large if BLACK (AI) is in advantage
 */
int Board_Stat::eval_board() {
    Piece_Count blk_count, wht_count;
    piece_count_weighted(&blk_count, &wht_count);

    int w_pawn_double, b_pawn_double;
    int w_pawn_backwd, b_pawn_backwd;
    int w_pawn_isoltd, b_pawn_isoltd;
    pawn_pattern(w_pawn_double, b_pawn_double, w_pawn_backwd, b_pawn_backwd, w_pawn_isoltd, b_pawn_isoltd);

    int w_mob, b_mob;
    eval_mobility(w_mob, b_mob);

    return (blk_count.num_k-wht_count.num_k)
         + (blk_count.num_q-wht_count.num_q)
         + (blk_count.num_r-wht_count.num_r)
         + (blk_count.num_b-wht_count.num_b)
         + (blk_count.num_n-wht_count.num_n)
         + (blk_count.num_p-wht_count.num_p)
         + KP_SPEC*(b_pawn_double+b_pawn_backwd+b_pawn_isoltd-w_pawn_double-w_pawn_backwd-w_pawn_isoltd)
         + KMOBILITY*(b_mob-w_mob);
}

// fill Piece_Count with weighted value
void Board_Stat::piece_count_weighted(Piece_Count *blk_count, Piece_Count *wht_count) {
    for (int r = 0; r <= 7; r++) {
        for (int c = 0; c <= 7; c++) {
            if (board_stat[r][c] == EMPTY)
                continue;
            Piece_Count *count;
            int side = (board_stat[r][c]*BLACK > 0) ? BLACK : WHITE;
            if (side==BLACK) {
                count = blk_count;
            } else {
                count = wht_count;
            }
            int role = abs(board_stat[r][c]);
            int rr = (side==BLACK) ? r: (7-r);
            int cc = c;
            switch(role) {
                case KING:
                    (count->num_k)+=KK;
                    (count->num_k)+=king_mat[rr][cc];
                    break;
                case QUEEN:
                    (count->num_q)+=KQ;
                    (count->num_q)+=queen_mat[rr][cc];
                    break;
                case ROOK:
                    (count->num_r)+=KR;
                    (count->num_r)+=rook_mat[rr][cc];
                    break;
                case BISHOP:
                    (count->num_b)+=KB;
                    (count->num_b)+=bishop_mat[rr][cc];
                    break;
                case KNIGHT:
                    (count->num_n)+=KN;
                    (count->num_n)+=knight_mat[rr][cc];
                    break;
                case PAWN:
                    (count->num_p)+=KP;
                    (count->num_p)+=pawn_mat[rr][cc];
                    break;
            }
        }
    }
}

// set input to be the count of these pawn patterns
void Board_Stat::pawn_pattern(int &w_double, int &b_double, int &w_backwd, int &b_backwd, int &w_isoltd, int &b_isoltd) {
    w_double = 0;
    b_double = 0;
    w_backwd = 0;
    b_backwd = 0;
    w_isoltd = 0;
    b_isoltd = 0;
    for (int r = 0; r <= 7; r ++) {
        for (int c = 0; c <= 7; c++) {
            if (abs(board_stat[r][c]) != PAWN)
                continue;
            int side = (board_stat[r][c]*BLACK > 0) ? BLACK : WHITE;
            int *dbl = (side==BLACK) ? (&b_double) : (&w_double);
            *dbl += pawn_double(side, r, c);
            int *bk = (side==BLACK) ? (&b_backwd) : (&w_backwd);
            *bk += pawn_backwd(side, r, c);
            int *iso = (side==BLACK) ? (&b_isoltd) : (&w_isoltd);
            *iso += pawn_isoltd(side, r, c);
        }
    }
    w_double /= 2;  // double pawn will be double counted
    b_double /= 2;
}

// count number of valid move of both sides
void Board_Stat::eval_mobility(int &w_mob, int &b_mob) {
    w_mob = 0;
    b_mob = 0;
    int *mob;
    for (int r = 0; r <= 7; r ++) {
        for (int c = 0; c <= 7; c++) {
            if (board_stat[r][c] == EMPTY)
                continue;
            int side = ((board_stat[r][c]*BLACK) > 0) ? BLACK : WHITE;
            mob = (side==BLACK) ? (&b_mob) : (&w_mob);
            int role = abs(board_stat[r][c]);
            switch(role) {
                case KING:
                    *mob += mob_king(side, r, c);
                    break;
                case QUEEN:
                    *mob += mob_straight(side, r, c);
                    *mob += mob_diagnal(side, r, c);
                    break;
                case ROOK:
                    *mob += mob_straight(side, r, c);
                    break;
                case BISHOP:
                    *mob += mob_diagnal(side, r, c);
                    break;
                case KNIGHT:
                    *mob += mob_knight(side, r, c);
                    break;
                case PAWN:
                    *mob += mob_pawn(side, r, c);
                    break;
            }
        }
    }
}



/**************************
 *     Initialization     *
 **************************/
void init_ai_search(int &alpha, int &beta) {
    search_depth = 0;

    ai_next_start_r = INVALID;
    ai_next_start_c = INVALID;
    ai_next_end_r = INVALID;
    ai_next_end_c = INVALID;

    alpha = NEGA_INFINITY;
    beta = POSI_INFINITY;
}



