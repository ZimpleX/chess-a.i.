#include "util.h"
#include "algo.h"
#include "rule.h"
#include "test_board.h"
#include <iostream>

using namespace std;

int board_stat[8][8];

int main() {
    cout << "test output\n";
    Board_Stat bs(NORMAL_BOARD);
    cout << bs;
}

