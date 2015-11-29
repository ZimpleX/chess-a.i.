#include "util.h"
#include "algo.h"
#include "rule.h"
#include "test_board.h"
#include <iostream>
#include <cstdio>
#include <string>

using namespace std;

int board_stat[8][8];

int main() {
    Board_Stat bs(NORMAL_BOARD);
    string str_play_side = (PLAYER == BLACK) ? "BLACK":"WHITE";
    cout  << "======================\n"
          << "=    BLACK upside    =\n"
          << "=    WHITE dnside    =\n"
          << "=--------------------=\n"
          << "=    PLAYER " << str_play_side << "    =\n"
          << "======================\n\n\n";
    cout << bs;
    cout << "\n\n";
    bool is_player_move = (INIT_MOVE == PLAYER) ? true : false;
    while (!bs.get_is_terminate()) {
        if (is_player_move) {
            bool move_valid = false;
            while (!move_valid) {
                int from_r, from_c, to_r, to_c;
                cout << " ____________\n"
                     << "/ User Input \\\n";
                cout << "|____________|________________________________ \n"
                     << "| from_row   from_column   to_row   to_column \\\n";
                cin >> from_r >> from_c >> to_r >> to_c;
                cout << "\\_____________________________________________/\n";
                if (check_boarder(from_r, from_c)
                 && check_boarder(to_r, to_c)
                 && (bs.get_piece(from_r, from_c)*PLAYER) > 0
                 && bs.player_move(from_r, from_c, to_r, to_c)) {
                    print_board_move(bs, PLAYER, from_r, from_c, to_r, to_c);
                    move_valid = true;
                } else {
                    cout <<"*******  CANNOT MOVE  ********\n";
                }
            }
        } else {
            int ai_side = (PLAYER == BLACK) ? WHITE:BLACK;
            // no need to check validity here
            ai_move(&bs);
            print_board_move(bs, AI, ai_next_start_r, ai_next_start_c, ai_next_end_r, ai_next_end_c);
        }
        is_player_move = !is_player_move;
    }
}

