#include "util.h"
#include "algo.h"
#include "rule.h"
#include <iostream>

using namespace std;

int board_stat[8][8];

int main() {
    for (int r = 0; r < 8; r ++) {
        for (int c = 0; c < 8; c ++) {
            board_stat[r][c] = 0;
        }
    }
}
