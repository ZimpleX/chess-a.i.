#include "util.h"
#include "algo.h"
#include "rule.h"
#include "test_board.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

int board_stat[8][8];

double ai_single_move_duration = -1;

int main() {
    Board_Stat bs(NORMAL_BOARD);
    string str_play_side = (PLAYER == BLACK) ? "BLACK":"WHITE";
    string str_ai_side = (PLAYER == BLACK) ? "WHITE" : "BLACK";
    cout << "\n\n"
         << "**********************\n"
         << " WELCOME TO CHESS A.I.\n"
         << "======================\n"
         << "=    BLACK upside    =\n"
         << "=    WHITE dnside    =\n"
         << "=--------------------=\n"
         << "=    PLAYER " << str_play_side << "    =\n"
         << "=      A.I. " << str_ai_side << "    =\n"
         << "======================\n\n\n";
    cout << bs;
    cout << "\n\n";

    // start playing
    bool is_player_move = (INIT_MOVE == PLAYER) ? true : false;
    while (!bs.get_is_terminate()) {
        if (is_player_move) {
            ai_single_move_duration = -1;
            bool move_valid = false;
            while (!move_valid) {
                int from_r, from_c, to_r, to_c;
                cout << " ____________\n"
                     << "/ User Input \\\n";
                cout << "|____________|________________________________ \n"
                     << "| from_row   from_column   to_row   to_column \\\n";
                cin >> from_r >> from_c >> to_r >> to_c;
                if (cin.fail()) {
                    cout << "please input integer\n";
                    cin.clear();
                    cin.ignore();
                    from_r = INVALID;
                }
                cin.ignore(100000, '\n');
                cout << "\\_____________________________________________/\n";
                if (check_border(from_r, from_c)
                 && check_border(to_r, to_c)
                 && (bs.get_piece(from_r, from_c)*PLAYER) > 0
                 && bs.player_move(from_r, from_c, to_r, to_c)) {
                    print_board_move(bs, PLAYER, from_r, from_c, to_r, to_c);
                    move_valid = true;
                } else {
                    cout <<"*******  CANNOT MOVE  ********\n";
                }
            }
        } else {
            clock_t start = clock();
            cout << "\n    A.I is thinking ... \n\n";
            int ai_side = (PLAYER == BLACK) ? WHITE:BLACK;
            // no need to check validity here
            ai_move(&bs);
            ai_single_move_duration = (clock() - start) / (double) CLOCKS_PER_SEC;
            print_board_move(bs, AI, ai_next_start_r, ai_next_start_c, ai_next_end_r, ai_next_end_c);
        }
        print_check(bs);
        is_player_move = !is_player_move;
    }

    // print result
    for (int r = 0; r <= 7; r++) {
        for (int c = 0; c <= 7; c++) {
            if (abs(bs.get_piece(r,c)) != KING)
                continue;
            int win_side = (bs.get_piece(r,c)*AI > 0) ? AI : PLAYER;
            if (win_side == PLAYER)
                cout << "\n\n    YOU WIN !!!\n";
            else
                cout << "\n\n    YOU LOST !!!\n";
            break;
        }
    }
}

