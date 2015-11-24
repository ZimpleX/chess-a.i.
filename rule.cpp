#include "rule.h"
#include "util.h"
#include <stdlib.h>
#include <cassert>

using namespace std;

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
 * don't check game result here
 * this function only checks if the move is valid, and update board_stat
 */
bool Board_Stat::move(int start_r, int start_c, int end_r, int end_c) {
    // life-cycle of enpassant is only one move
    int old_enpassant_c = enpassant_c;
    enpassant_c = INVALID; 
    if (board_stat[start_r][start_c] == EMPTY)
        return false;   // nothing at start point
    int side = (board_stat[start_r][start_c] * BLACK > 0) ? BLACK : WHITE;
    int role = board_stat[start_r][start_c] * side; // rule is positive
    if (board_stat[end_r][end_c] * side > 0)
        return false;   // end point is occupied
    // check if the move direciton and step is valid
    int dr = abs(start_r - end_r);
    int dc = abs(start_c - end_c);
    if (role == KING) {
        // Not implemented castling currently
        if (dr > 1 || dc > 1)
            return false;   // invalid move
    } else if (role == QUEEN) {
        if (dr * dc * (dr - dc) != 0)   // diagnotic or straight move is valid
            return false;
    } else if (role == ROOK) {
        if (dr * dc != 0)   // straight move
            return false;
    } else if (role == BISHOP) {
        if (dr != dc)   // diagnotic
            return false;
    } else if (role == KNIGHT) {
        if (dr * dc != 2)   // dx, dy are both positive integers
            return false;
    } else if (role == PAWN) {
        // PAWN is tricky 
        // check if pawn is taking
        // taking en-pasant
        // check promote: currently auto promote to queen
        bool is_enpassant;       // check first if this move will do the enpassant
        ///////////////
        //  check x  //
        ///////////////
        // check if the move is taking a piece
        bool is_take;
        if (dc == 0)
            is_take = false;
        else if (dc == 1)
            is_take = true;
        else
            return false;
        ///////////////
        //  check y  //
        ///////////////
        // check if y move is valid
        // Black pawn start from y=1, white pawn start from y=6
        if ((start_r - end_r) * side == 1) {
            // move up or down one step
            if (!is_take) {
                if (board_stat[end_r][end_c] != EMPTY)
                    return false;
            } else {
                // has already checked that end point is not occupied by piece of its own side
                if ((board_stat[end_r][end_c] == EMPTY)
                    && (old_enpassant_c == end_c)
                    && ((start_r + 0.5 * side) == 3.5)) {
                    // start_y can only be 4 for side BLACK and 3 for side WHITE
                    // enpassant is happening, take away the opponent's pawn
                    assert(board_stat[start_r][end_c] == -side*PAWN);
                    board_stat[start_r][end_c] = EMPTY;
                } else if (board_stat[end_r][end_c] != EMPTY) {
                    // normal: take away the opponent's piece
                } else {
                    // cannot 'take'
                    return false;
                }
            }
        } else if ((start_r - end_r) * side == 2) {
            if (is_take)
                return false;
            if ((start_r-3.5) * side != 2.5)
                return false;   // pawn has been moved before
            if (board_stat[end_r][end_c] != EMPTY)
                return false;
            // set enpassant_x
            enpassant_c = start_c;
        } else {
            return false;
        }
        // check promote 
        if (end_r == 0 || end_r == 7)
            board_stat[start_r][start_c] = QUEEN * side;    // the following code moves piece from start to end, so we set start pos here
    }
    // update board_stat
    // the move is valid if we reach here
    if (abs(board_stat[end_r][end_c]) == KING)
        is_terminate = true;
    board_stat[end_r][end_c] = board_stat[start_r][start_c];
    board_stat[start_r][start_c] = EMPTY;
}
