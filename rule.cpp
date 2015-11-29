#include "rule.h"
#include "util.h"
#include <stdlib.h>
#include <cassert>

using namespace std;


/**********  Function declaration  **********/

/**********  Function definition  **********/
Board_Stat::Board_Stat(int init[8][8]) {
    for (int r = 0; r < 8; r ++) {
        for (int c = 0; c < 8; c ++){
            board_stat[r][c] = init[r][c];
        }
    }
    is_terminate = false;
    enpassant_c = INVALID;
}

/*
 * Assumption: r and c are within 0 ~ 7, and the side is correct
 *             the start location is not empty
 * don't check game result here
 * this function only checks if the move is valid, and update board_stat
 */
bool Board_Stat::player_move(int start_r, int start_c, int end_r, int end_c) {
    // life-cycle of enpassant is only one move
    int new_enpassant_c = INVALID; 
    int side = (board_stat[start_r][start_c] * BLACK > 0) ? BLACK : WHITE;
    int role = board_stat[start_r][start_c] * side; // rule is positive
    if (board_stat[end_r][end_c] * side > 0)
        return false;   // end point is occupied

    switch(role) {
        case KING:
            // Not implemented castling currently
            if (abs(start_r - end_r) > 1 || abs(start_c - end_c) > 1)
                return false;
            break;
        case QUEEN:
            if (!player_chk_straight(start_r, start_c, end_r, end_c)
             && !player_chk_diagnal(start_r, start_c, end_r, end_c))
                return false;
            break;
        case ROOK:
            if (!player_chk_straight(start_r, start_c, end_r, end_c))
                return false;
            break;
        case BISHOP:
            if (!player_chk_diagnal(start_r, start_c, end_r, end_c))
                return false;
            break;
        case KNIGHT:
            if (abs(start_r - end_r) * abs(start_c - end_c) != 2)  // dx, dy are both positive integers
                return false;
            break;
        case PAWN:
            if (!check_pawn(start_r, start_c, end_r, end_c, side, new_enpassant_c))
                return false;
            break;
    }
    // update board_stat
    // the move is valid if we reach here
    enpassant_c = new_enpassant_c;
    if (abs(board_stat[end_r][end_c]) == KING)
        is_terminate = true;
    board_stat[end_r][end_c] = board_stat[start_r][start_c];
    board_stat[start_r][start_c] = EMPTY;

    return true;
}



/*
 * Util functions to enforce the chess rule
 */
bool Board_Stat::player_chk_straight(int start_r, int start_c, int end_r, int end_c) {
    if (start_r == end_r) {
        int c_s = (start_c < end_c) ? start_c : end_c;
        int c_e = (start_c > end_c) ? start_c : end_c;
        for (int cc = c_s+1; cc < c_e; cc++) {
            if (board_stat[start_r][cc] != EMPTY)
                return false;
        }
    } else if (start_c == end_c) {
        int r_s = (start_r < end_r) ? start_r : end_r;
        int r_e = (start_r > end_r) ? start_r : end_r;
        for (int rr = r_s+1; rr < r_e; rr++) {
            if (board_stat[rr][start_c] != EMPTY)
                return false;
        }
    } else {
        return false;
    }
    return true;
}


bool Board_Stat::player_chk_diagnal(int start_r, int start_c, int end_r, int end_c) {
    if (abs(start_r - end_r) != abs(start_c - end_c))
        return false;
    int kr = (start_r < end_r) ? 1 : -1;
    int kc = (start_c < end_c) ? 1 : -1;
    int d = abs(start_r - end_r);
    for (int dd = 1; dd < d; dd++) {
        if (board_stat[start_r+kr*dd][start_c+kc*dd] != EMPTY)
            return false;
    }
    return true;
}

/*
 * special rule for pawn: en-passant / taking / non-taking / promote
 */
bool Board_Stat::check_pawn(int start_r, int start_c, int end_r, int end_c, int side, int &new_enpassant_c) {
    new_enpassant_c = INVALID;
    int dr = start_r - end_r;
    int dc = start_c - end_c;
    ///////////////
    //  check x  //
    ///////////////
    // check if the move is taking a piece
    bool is_take;
    if (dc == 0)
        is_take = false;
    else if (abs(dc) == 1)
        is_take = true;
    else
        return false;
    ///////////////
    //  check y  //
    ///////////////
    // check if y move is valid
    // Black pawn start from y=1, white pawn start from y=6
    if (dr * side == 1) {
        // move up or down one step
        if (!is_take) {
            if (board_stat[end_r][end_c] != EMPTY)
                return false;
        } else {
            // has already checked that end point is not occupied by piece of its own side
            if ((board_stat[end_r][end_c] == EMPTY)
                && (enpassant_c == end_c)
                && ((start_r + 0.5 * side) == 3.5)) {
                // start_y can only be 4 for side BLACK and 3 for side WHITE
                // enpassant is happening, take away the opponent's pawn
                assert(board_stat[start_r][end_c] == -side*PAWN);
                board_stat[start_r][end_c] = EMPTY;
            } else if (board_stat[end_r][end_c] != EMPTY) {
                // normal: take away the opponent's piece
            } else {
                return false;
            }
        }
    } else if (dr * side == 2) {
        if (is_take
         || (start_r-3.5) * side != 2.5
         || board_stat[end_r][end_c] != EMPTY
         || board_stat[(start_r+end_r)/2][start_c] != EMPTY)
            return false;
        // set enpassant_x
        new_enpassant_c = start_c;
    } else {
        return false;
    }
    // check promote 
    if (end_r == 0 || end_r == 7)
        board_stat[start_r][start_c] = QUEEN * side;    // the following code moves piece from start to end, so we set start pos here
    
    return true;
}


/*
 * This has to be called within sweep_straight / sweep_diagnal / ai_pre_move
 * Cuz it will not make strict check for validity of rule
 * --> the positions are pre-checked in the caller
 *  What it does:
 *      --  clear enpassant_c
 *      --  check if end position is occupied by piece from the same side
 *      --  update board_stat
 */
bool Board_Stat::ai_direct_move(int r_s, int c_s, int r_e, int c_e) {
    if (board_stat[r_s][c_s] * board_stat[r_e][c_e] > 0)
        return false;
    enpassant_c = INVALID;
    board_stat[r_e][c_e] = board_stat[r_s][c_s];
    board_stat[r_s][c_s] = EMPTY;
    return true;
}

/*
 * Unlike the ai_direct_move function, ai_pawn_move will make stricter check
 */
bool Board_Stat::ai_pawn_move(int r_s, int c_s, int r_e, int c_e) {
    if (board_stat[r_s][c_s]*board_stat[r_e][c_e] > 0)
        return false;
    int new_enpassant_c;
    int side = (board_stat[r_s][c_s]*BLACK > 0) ? BLACK : WHITE;
    if (!check_pawn(r_s, c_s, r_e, c_e, side, new_enpassant_c))
        return false;
    enpassant_c = new_enpassant_c;
    board_stat[r_e][c_e] = board_stat[r_s][c_s];
    board_stat[r_s][c_s] = EMPTY;
    return true;
}


/*****************************************/
int Board_Stat::mob_king(int side, int r, int c) {
    int ctr = 0;

    int r_s = (r>0)?(r-1):0;
    int r_e = (r<7)?(r+1):7;
    int c_s = (c>0)?(c-1):0;
    int c_e = (c<7)?(c+1):7;

    for (int rr=r_s; rr<=r_e; rr++) {
        for (int cc=c_s; cc<=c_e; cc++) {
            if (board_stat[rr][cc]*side > 0)
                continue;
            ctr ++;
        }
    }

    return ctr;
}

int Board_Stat::mob_knight(int side, int r, int c) {
    int ctr = 0;

    for (int dir = -1; dir <= 1; dir += 2) {
        for (int off_r = 1; off_r <= 2; off_r ++) {
            int rr = r + dir*off_r;
            if (rr > 7 || rr < 0)
                continue;
            int off_c = 2 / off_r;
            for (int kc = -1; kc <= 1; kc += 2) {
                int cc = c + kc*off_c;
                if (cc > 7 || cc <0)
                    continue;
                if (board_stat[rr][cc]*side > 0)
                    continue;
                ctr ++;
            }
        }
    }

    return ctr;
}

int Board_Stat::mob_pawn(int side, int r, int c) {
    int ctr = 0;
    
    int kr = (side==BLACK) ? -1 : 1;
    int cc = c;
    int rr = r+kr;
    // don't care about 2 step forward
    if (rr>=0 && rr<=7 && board_stat[rr][cc] == EMPTY)
        ctr++;
    rr = r+kr;
    cc = c+1;
    // don't care about enpassant
    if (rr>=0 && rr<=7 && cc>=0 && cc<=7 && board_stat[r][c]*board_stat[rr][cc]<0)
        ctr++;
    cc = c-1;
    if (rr>=0 && rr<=7 && cc>=0 && cc<=7 && board_stat[r][c]*board_stat[rr][cc]<0)
        ctr++;
    return ctr;
}

int Board_Stat::mob_straight(int side, int r, int c) {
    int ctr = 0;

    for (int kr = -1; kr <= 1; kr ++) {
        for (int kc = -1; kc <= 1; kc++) {
            if (kr*kc != 0)
                continue;
            for (int d = 1; d <= 7; d++) {
                int rr = r + kr*d;
                int cc = c + kc*d;
                if (rr < 0 || rr > 7
                 || cc < 0 || cc > 7)
                    break;
                if (board_stat[rr][cc]*side > 0) {
                    break;
                } else if (board_stat[rr][cc]*side < 0) {
                    ctr++;
                    break;
                } else {
                    ctr++;
                }

            }
        }
    }

    return ctr;
}

int Board_Stat::mob_diagnal(int side, int r, int c) {
    int ctr = 0;

    for (int kr = -1; kr <= 1; kr += 2) {
        for (int kc = -1; kc <= 1; kc += 2) {
            for (int d = 1; d <= 7; d++) {
                int rr = r + kr*d;
                int cc = c + kc*d;
                if (rr < 0 || cc < 0 
                 || rr > 7 || cc > 7)
                    break;
                if (board_stat[rr][cc]*side > 0) {
                    break;
                } else if (board_stat[rr][cc]*side < 0) {
                    ctr++;
                    break;
                } else {
                    ctr++;
                }
            }
        }
    }

    return ctr;
}

/*****************************************/
int Board_Stat::pawn_double(int side, int r, int c) {
    return 0;
}

int Board_Stat::pawn_backwd(int side, int r, int c) {
    return 0;
}

int Board_Stat::pawn_isoltd(int side, int r, int c) {
    return 0;
}

/*****************************************/
/*
 * Getter and setter
 */
bool Board_Stat::get_is_terminate() {
    return is_terminate;
}

int Board_Stat::get_piece(int r, int c) {
    return board_stat[r][c];
}

bool check_boarder(int r, int c) {
    if (r > 7 || r < 0 || c > 7 || c < 0)
        return false;
    else 
        return true;
}
