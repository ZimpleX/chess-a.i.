#include "util.h"
#include "rule.h"
#include <string>
#include <iostream>
#include <cstdio>

// #define DEBUG

using namespace std;

ostream& operator<< (ostream& os, const Board_Stat& bs) {
    os << "==========  BOARD  STATUS  ==========     ~\n";
    os << "    0   1   2   3   4   5   6   7         ~\n";
    string border_line = "  +-------------------------------+       ~";
    string format_line = "%d |%s %s %s %s %s %s %s %s| %d     ~";
    string middle_line = "  |   +   +   +   +   +   +   +   |       ~";

    for (int r = 0; r < 8; r++) {
        char role_str[8][4];
        for (int c = 0; c < 8; c++) {
            int cur_piece = bs.board_stat[r][c];
            int side = (cur_piece * BLACK > 0) ? BLACK : WHITE;
            string container = (side == BLACK) ? "<%s>" : "[%s]";
            int role = cur_piece * side;
            string r_str;
            if (role == EMPTY) {
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
        string comb_line;
        if (r == 0) {
            comb_line = border_line+"\n"+format_line+"  Status\n";
        } else if (r == 1) {
            char num_str[4];
            sprintf(num_str, "%d", bs.enpassant_c);
            comb_line = middle_line+"\n"+format_line+"     Enpassant pawn: "+num_str+"\n";
        } else {
            comb_line = middle_line+"\n"+format_line+"\n";
        }
        sprintf(line, comb_line.c_str(),
                r, role_str[0], role_str[1], role_str[2], role_str[3], role_str[4], role_str[5], role_str[6], role_str[7], r);

        os << line;
    }
    os << border_line << "\n";
    os << "    0   1   2   3   4   5   6   7         ~\n";
    os << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
}


// wrapper of Board_Stat cout
void print_board_move(Board_Stat bs, int side, int r, int c, int rr, int cc) {
    char line[44];
    sprintf(line, "  From [r%d , c%d], To [r%d , c%d]            ~", 
            r, c, rr, cc);
    string side_str = (side==AI)?"<A.I.>  ":"<PLAYER>";
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
         << side_str << "move:                             ~\n"
         << "-------------                             ~\n"
         << line << "\n";
    cout << bs;
    cout << "\n\n\n";
}
