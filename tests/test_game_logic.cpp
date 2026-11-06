#include <cassert>

#include "../game_logic.h"

int main() {
    const int shape[SIZE][SIZE] = {
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };

    int emptyBoard[ROWS][COLS] = {};
    assert(!isCollision(emptyBoard, shape, 0, 0));

    int leftBoard[ROWS][COLS] = {};
    assert(isCollision(leftBoard, shape, 0, -1));

    int rightBoard[ROWS][COLS] = {};
    assert(isCollision(rightBoard, shape, 0, COLS - 1));

    int bottomBoard[ROWS][COLS] = {};
    assert(isCollision(bottomBoard, shape, ROWS - 1, 0));

    int fixedCellBoard[ROWS][COLS] = {};
    fixedCellBoard[1][1] = 2;
    assert(isCollision(fixedCellBoard, shape, 0, 0));

    return 0;
}
