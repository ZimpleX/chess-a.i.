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

#define INIT_MOVE BLACK // a.i. is moving first
#define PLAYER WHITE
#define AI BLACK

#define INVALID -1

enum Role {EMPTY=0, KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN};

/*
 * To be used in evaluation function.
 * the value in the member variables has already taken into 
 * account the weight for different type of pieces
 * For example:
 *      if there is 1 pawn, then num_p will be 100
 */
struct Piece_Count {
    int num_k;
    int num_q;
    int num_r;
    int num_b;
    int num_n;
    int num_p;

    Piece_Count() {
        num_k = 0;
        num_q = 0;
        num_r = 0;
        num_b = 0;
        num_n = 0;
        num_p = 0;
    }
};

// util function checking if index is out of the chess board
bool check_boarder(int r, int c);


/*
 * class about:
 *      --  current board information
 *      --  move rule
 *      --  some algo related functions
 */
class Board_Stat {

private:
    bool is_terminate;      // set to true if one king is eaten
    int board_stat[8][8];   // value = side * piece_type
                            // e.g.: BLACK * PAWN
    int enpassant_c;        // indicate enpassant by column number.
                            // No need to store other information, 
                            // cuz lifecycle of enpassant is only 1.

    // checking funtions: to be used with palyer_move()
    bool player_chk_straight(int start_r, int start_c, int end_r, int end_c);
    bool player_chk_diagnal(int start_r, int start_c, int end_r, int end_c);
    bool check_pawn(int start_r, int start_c, int end_r, int end_c, int side, int &new_enpassant_c);
    // evaluation related functions:
    //   -- mobility is counting the num of valid choice of next move
    //   -- piece_count_weighted has already thought about coefficient
    //   -- pawn_* is finding special pawn patterns
    void piece_count_weighted(Piece_Count *blk_count, Piece_Count *wht_count);
    void eval_mobility(int &w_mob, int &b_mob);
    void pawn_pattern(int &w_double, int &b_double, int &w_backwd, int &b_backwd, int &w_isoltd, int &b_isoltd);

    int mob_king(int side, int r, int c);
    int mob_knight(int side, int r, int c);
    int mob_pawn(int side, int r, int c);
    int mob_straight(int side, int r, int c);
    int mob_diagnal(int side, int r, int c);
    
    int pawn_double(int side, int r, int c);
    int pawn_backwd(int side, int r, int c);
    int pawn_isoltd(int side, int r, int c);

public:
    Board_Stat(int init[8][8]);
    // move and update board_stat, return true if move succeeded.
    bool player_move(int start_r, int start_c, int end_r, int end_c);
    // a.i. move function, *HAVE TO* be used with ai_pre_move()
    bool ai_direct_move(int r_s, int c_s, int r_e, int c_e);
    bool ai_pawn_move(int r_s, int c_s, int r_e, int c_e);
    
    // algo-related functions
    //   -- terminate_test: indicate when should the a.i. search stop
    //   -- eval_board: return cost function based on current board
    //                  high if a.i. is taking advantages
    bool terminate_test();
    int eval_board();

    // getter and setter
    bool get_is_terminate();
    int get_piece(int r, int c);

    friend std::ostream& operator<< (std::ostream& os, const Board_Stat& bs);
};

