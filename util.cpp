#include "util.h"
#include "rule.h"
#include <string>
#include <iostream>
#include <cstdio>


using namespace std;

ostream& operator<< (ostream& os, const Board_Stat& bs) {
    os << "========== BOARD STATUS ==========\n";
    for (int r = 0; r < 8; r++) {
        char role_str[8][4];
        for (int c = 0; c < 8; c++) {
            int cur_piece = bs.board_stat[r][c];
            int side = (cur_piece * BLACK > 0) ? BLACK : WHITE;
            string container = (side == BLACK) ? "<%s>" : "[%s]";
            int role = cur_piece * side;
            string r_str;
            if (role == EMPTY) {
                //string sp = "   ";
                //role_str[c] = sp.c_str();
                //role_str[c] = 
                role_str[c][0] = ' ';
                role_str[c][1] = ' ';
                role_str[c][2] = ' ';
                role_str[c][3] = '\0';
            } else {
                switch(role) {
                    case KING:
                        r_str = 'K';
                        break;
                    case QUEEN:
                        r_str = 'Q';
                        break;
                    case BISHOP:
                        r_str = 'B';
                        break;
                    case KNIGHT:
                        r_str = 'N';
                        break;
                    case ROOK:
                        r_str = 'R';
                        break;
                    case PAWN:
                        r_str = 'P';
                        break;
                }
                char role_contained[3];
                sprintf(role_contained, container.c_str(), r_str.c_str());
                role_str[c][0] = role_contained[0];
                role_str[c][1] = role_contained[1];
                role_str[c][2] = role_contained[2];
                role_str[c][3] = '\0';
            }
        }
        char line[200];
        sprintf(line, "+---+---+---+---+---+---+---+---+\n|%s|%s|%s|%s|%s|%s|%s|%s|\n", role_str[0], role_str[1], role_str[2], role_str[3], role_str[4], role_str[5], role_str[6], role_str[7]);
        os << line;
    }
    os << "+---+---+---+---+---+---+---+---+\n";
    os << "==========  END STATUS  ==========\n";
}
