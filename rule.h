#pragma once
#include <iostream>
/*
 * rules for chess:
 *      - valid move of each element
 *      - current board status
 */

/*
 * Board coordinate:
 *      r: up to down --> 0 to 7
 *      c: left to right --> 0 to 7
 * Black is placed at top: i.e., r = 0, 1
 * White is placed at bottom: i.e., r = 6, 7
 */

#define BLACK -1
#define WHITE 1

#define INIT_MOVE BLACK
#define PLAYER WHITE
#define AI BLACK

#define INVALID -1

enum Role {EMPTY=0, KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN};

/*
 * up, down, right, left,
 * up-right, up-left, down-right, down-left.
 */
//enum Dir {UP=1, DN, RG, LF, UR, UL, DR, DL};

bool check_boarder(int r, int c);

class Board_Stat {
private:
    bool is_terminate;
    /*
     * indicate the role at that location
     * White side: positive
     * Black side: negative
     */
    int board_stat[8][8];

    /*
     * indicate if there could be an en-passant at next move:
     *      this is set to be the x of a pawn taking his first move 2 steps forward,
     *      and is set to be INVALID by the opponent move immediately after
     */
    int enpassant_c;

    // checking funtions: to be used with palyer_move()
    bool check_straight(int start_r, int start_c, int end_r, int end_c);
    bool check_diagnal(int start_r, int start_c, int end_r, int end_c);

    bool check_pawn(int start_r, int start_c, int end_r, int end_c, int side, int &new_enpassant_c);

public:
    Board_Stat(int init[8][8]);
    /*
     * move the pieces according to the rules, and update all status accordingly
     * return:
     *      true: if the move is legal
     *      false: if the move is illegal
     * if the king is dead, set is_terminate to true
     */
    bool player_move(int start_r, int start_c, int end_r, int end_c);
    void ai_move();
    // the actual move function, have to be used with ai_pre_move
    bool ai_direct_move(int r_s, int c_s, int r_e, int c_e);
    bool ai_pawn_move(int r_s, int c_s, int r_e, int c_e);

    // getter and setter
    bool get_is_terminate();
    int get_piece(int r, int c);

    friend std::ostream& operator<< (std::ostream& os, const Board_Stat& bs);
};

