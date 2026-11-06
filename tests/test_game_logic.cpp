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

    int lockBoard[ROWS][COLS] = {};
    lockPiece(lockBoard, shape, 4, 3, 5);
    assert(lockBoard[4][3] == 5);
    assert(lockBoard[4][4] == 5);
    assert(lockBoard[5][4] == 5);
    assert(lockBoard[5][5] == 5);
    assert(lockBoard[4][5] == 0);

    int occupiedBoard[ROWS][COLS] = {};
    occupiedBoard[5][4] = 7;
    lockPiece(occupiedBoard, shape, 4, 3, 5);
    assert(occupiedBoard[5][4] == 7);
    assert(occupiedBoard[4][3] == 0);

    return 0;
}
