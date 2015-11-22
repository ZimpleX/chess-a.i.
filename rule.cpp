#include "rule.h"
#include "util.h"

using namespace std;

bool move(int start_x, int start_y, int end_x, int end_y) {
    if (board_stat[start_x][start_y] == 0)
        return false;   // nothing at start point
    int side = (board_stat[start_x][start_y] * BLACK > 0) ? BLACK : WHITE;
    int role = board_stat[start_x][start_y] * side; // rule is positive
    if (board_stat[end_x][end_y] * side > 0)
        return false;   // end point is occupied
    if (role == KING) {
        //if (abs())
    } else if (role == QUEEN) {

    } else if (role == ROOK) {

    } else if (role == BISHOP) {

    } else if (role == KNIGHT) {

    } else if (role == PAWN) {

    }
}
