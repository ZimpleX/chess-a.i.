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
            if (!check_straight(start_r, start_c, end_r, end_c)
             && !check_diagnal(start_r, start_c, end_r, end_c))
                return false;
            break;
        case ROOK:
            if (!check_straight(start_r, start_c, end_r, end_c))
                return false;
            break;
        case BISHOP:
            if (!check_diagnal(start_r, start_c, end_r, end_c))
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
bool Board_Stat::check_straight(int start_r, int start_c, int end_r, int end_c) {
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


bool Board_Stat::check_diagnal(int start_r, int start_c, int end_r, int end_c) {
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
    int dr = start_r - end_r;
    int dc = start_c - end_c;
    bool is_enpassant;       // check first if this move will do the enpassant
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

}

/*
 * Unlike the ai_direct_move function, ai_pawn_move will make stricter check
 */
bool Board_Stat::ai_pawn_move(int r_s, int c_s, int r_e, int c_e) {

}


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
