#include "algo.h"
#include "rule.h"
#include "util.h"

using namespace std;

// #define DUMMY


void get_to_loc_radius(int role, int from_r, int from_c,
    int &to_r_lim0, int &to_r_lim1, int &to_c_lim0, int &to_c_lim1);



void Board_Stat::ai_move() {
#ifdef DUMMY
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            int role = board_stat[r][c] * AI;
            if (role <= 0)
                continue;
            int to_r_lim0, to_r_lim1, to_c_lim0, to_c_lim1;
            get_to_loc_radius(role, r, c, to_r_lim0, to_r_lim1, to_c_lim0, to_c_lim1);
            for (int rr = to_r_lim0; rr <= to_r_lim1; rr++) {
                for (int cc = to_c_lim0; cc <= to_c_lim1; cc++) {
                    if (this->player_move(r, c, rr, cc)) { 
                        // break
                        r = 8;
                        c = 8;
                        rr = 8;
                        cc = 8;
                    }
                }
            }
        }
    }
#else
    // push initial state to stack
#endif
}

/*
 * alpha: initial NEGA_INFINITY --> AI side (MAX)
 * beta: initial POSI_INFINITY  --> player side (MIN)
 */
int ai_pre_move(Board_Stat cur_bs, int side, int alpha, int beta) {
    int cur_min = POSI_INFINITY;    // for AI return value
    int cur_max = NEGA_INFINITY;    // for player return value
    search_depth ++;
    Board_Stat moved_bs = cur_bs;

    if (cur_bs.stop_search()) {
        search_depth --;
        return cur_bs.eval_board(side);
    }
    
    int prune_stat; // indicating whether pruning has taken place
    int op_side = (side==BLACK) ? WHITE : BLACK;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            int role = cur_bs.get_piece(r, c) * AI;
            switch (role) {
                case KING:
                    int r_s = (r>0)?(r-1):0;
                    int r_e = (r<7)?(r+1):7;
                    int c_s = (c>0)?(c-1):0;
                    int c_e = (c<7)?(c+1):7;
                    for (int rr=r_s; rr<=r_e; rr++) {
                        for (int cc=c_s; cc<=c_e; cc++) {
                            // move king
                            if(!moved_bs.ai_direct_move())
                                continue;
                            int cur_cost = ai_pre_move(moved_bs, op_side, alpha, beta);
                            
                            prune_stat = ab_pruning(side, cur_cost, alpha, beta);
                            if (prune_stat != 0) {
                                search_depth --;
                                return prune_stat;
                            }
                            update_loc_minmax(r, c, rr, cc, cur_cost, cur_min, cur_max);
                            moved_bs = cur_bs;
                       }
                    }
                    break;
                case QUEEN:
                    prune_stat = sweep_straight(cur_bs, side, r, c, alpha, beta, cur_min, cur_max);
                    if (prune_stat != 0) {
                        search_depth --;
                        return prune_stat;
                    }
                    prune_stat = sweep_diagonal(cur_bs, side, r, c, alpha, beta, cur_min, cur_max);
                    if (prune_stat != 0) {
                        search_depth --;
                        return prune_stat;
                    }
                    break;
                case ROOK:
                    // wrapper for a bunch of pre_move of current board status
                    prune_stat = sweep_straight(cur_bs, side, r, c, alpha, beta, cur_min, cur_max);
                    if (prune_stat != 0) {
                        search_depth --;
                        return prune_stat;
                    }
                    break;
                case BISHOP:
                    prune_stat = sweep_diagonal(cur_bs, side, r, c, alpha, beta, cur_min, cur_max);
                    if (prune_stat != 0) {
                        search_depth --;
                        return prune_stat;
                    }
                    break;
                case KNIGHT:
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
                                if (!moved_bs.ai_direct_move(rr, cc))
                                    continue;
                                int cur_cost = ai_pre_move(moved_bs, op_side, alpha, beta);
                                prune_stat = ab_pruning(side, cur_cost, alpha, beta);
                                if (prune_stat != 0) {
                                    search_depth --;
                                    return prune_stat;
                                }
                                update_loc_minmax(r, c, rr, cc, cur_cost, cur_min, cur_max);
                                moved_bs = cur_bs;
                            }
                        }
                    }
                    break;
                case PAWN:
                    int r_s = (r>1)?(r-2):0;
                    int r_e = (r<6)?(r+2):7;
                    int c_s = (c>0)?(c-1):0;
                    int c_e = (c<7)?(c+1):7;
                    for (int rr=r_s; rr<=r_e; rr++) {
                        for (int cc=c_s; cc<=c_e; cc++) {
                            // move pawn
                            if(!moved_bs.ai_pawn_move())
                                continue;
                            int cur_cost = ai_pre_move(moved_bs, op_side, alpha, beta);
                            
                            prune_stat = ab_pruning(side, cur_cost, alpha, beta);
                            if (prune_stat != 0) {
                                search_depth --;
                                return prune_stat;
                            }
                            update_loc_minmax(r, c, rr, cc, cur_cost, cur_min, cur_max);
                            moved_bs = cur_bs;
                       }
                    }
                    break;
            }
        }
    }
    search_depth --;
    if (side == AI) {
        return cur_max; //(hope to be less than alpha, or to update beta)
    } else {
        return cur_min; //(hope to let parent AI to update alpha, or to be less than beta)
    }

}


/*
 * Main body of alpha-beta pruning, update alpha beta accordingly
 *
 * MAX node: prune beta, update alpha
 * MIN node: prune aplta, update beta
 *
 * return
 *      0 if searching of current node children should be continued
 *      NEGA_INFINITY or POSI_INFINITY if the node is pruned
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
 * record start and end row, column if we go back to root
 */
void update_loc_minmax(int r, int c, int rr, int cc, 
        int cur_cost, int &cur_min, int &cur_max) {
    if (search_depth == 1
     && cur_cost > cur_max) {   // the first ai move
        ai_next_start_r = r;
        ai_next_start_c = c;
        ai_next_end_r = rr;
        ai_next_start_c = cc;
    }
    cur_max = (cur_max > cur_cost) ? cur_max : cur_value;
    cur_min = (cur_min < cur_cost) ? cur_min : cur_value;
}


/*
 * return prune_stat
 */
int sweep_straight(Board_Stat cur_bs, int side, int r, int c) {
    Board_Stat moved_bs = cur_bs;
    // row
    for (int rr = r; rr <= 7; rr++) {
       if (cur_bs.get_piece(rr, c) * AI > 0)
           break;
       // move piece
       // ai_pre_move()
       // pop stack
       if (cur_bs.get_piece(rr, c) != EMPTY) // a piece has been taken in the previous move
           break;
    }
    for (int rr = r; rr >= 0; rr--) {
        
    }
    // column
    for (int cc = c; cc <= 7; cc++) {
        if (cur_bs.get_piece(r, cc) * AI > 0)
            break;
        // move piece
        // push stack
        // ai_pre_move()
        // pop stack
        if (cur_bs.get_piece(r, cc) != EMPTY)
            break;
    }
    for (int cc = c; cc >= 0; cc--) {

    }
}

int sweep_diagonal(Board_Stat cur_bs, int side, int r, int c) {
    for (int kr = -1; kr <= 1; kr += 2) {
        for (int kc = -1; kc <= 1; kc += 2) {
            for (int d = 1; d <=7; d++) {
                int rr = r + kr*d;
                int cc = c + kc*d;
                if (rr < 0 || cc < 0
                 || rr > 7 || cc > 7)
                    break;
                if (cur_bs.get_piece(rr, cc) * AI > 0)
                    break;
                // move piece
                // push stack
                // ai_pre_move
                // pop stack
                if (cur_bs.get_piece(rr, cc) != EMPTY)
                    break;
            }
        }
    }
}

/*
 * get the limit for search "to" location, based on role and from_r, from_c.
 * return:
 *      to_r_lim0: search bound for row start
 *      to_r_lim1: search bound for row end
 *      to_c_lim0: search bound for col start
 *      to_c_lim1: search bound for col end
 */
void get_to_loc_radius(int role, int from_r, int from_c, 
    int &to_r_lim0, int &to_r_lim1, int &to_c_lim0, int &to_c_lim1) {
    int radius;
    switch(role) {
        case KING:
            radius = 1;
            break;
        case QUEEN:
            radius = 7;
            break;
        case BISHOP:
            radius = 7;
            break;
        case KNIGHT:
            radius = 2;
            break;
        case ROOK:
            radius = 7;
            break;
        case PAWN:
            radius = 2;
            break;
    }
    to_r_lim0 = ((from_r-radius) >= 0) ? (from_r-radius):0;
    to_r_lim1 = ((from_r+radius) <= 7) ? (from_r+radius):7;
    to_c_lim0 = ((from_c-radius) >= 0) ? (from_c-radius):0;
    to_c_lim1 = ((from_c+radius) <= 7) ? (from_c+radius):7;
}
