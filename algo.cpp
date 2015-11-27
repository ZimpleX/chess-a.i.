#include "algo.h"
#include "rule.h"
#include "util.h"

using namespace std;

#define DUMMY


void get_to_loc_radius(int role, int from_r, int from_c,
    int &to_r_lim0, int &to_r_lim1, int &to_c_lim0, int &to_c_lim1);



void Board_Stat::ai_move(int ai_side) {
#ifdef DUMMY
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            int role = board_stat[r][c] * ai_side;
            if (role <= 0)
                continue;
            int to_r_lim0, to_r_lim1, to_c_lim0, to_c_lim1;
            get_to_loc_radius(role, r, c, to_r_lim0, to_r_lim1, to_c_lim0, to_c_lim1);
            for (int rr = to_r_lim0; rr <= to_r_lim1; rr++) {
                for (int cc = to_c_lim0; cc <= to_c_lim1; cc++) {
                    if (this->move(r, c, rr, cc)) { 
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

#endif
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
