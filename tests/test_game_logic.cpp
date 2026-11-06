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

    int notFullBoard[ROWS][COLS] = {};
    for (int col = 0; col < COLS - 1; ++col) {
        notFullBoard[ROWS - 1][col] = 1;
    }
    assert(clearFullLines(notFullBoard) == 0);
    assert(notFullBoard[ROWS - 1][COLS - 2] == 1);
    assert(notFullBoard[ROWS - 1][COLS - 1] == 0);

    int oneLineBoard[ROWS][COLS] = {};
    oneLineBoard[0][0] = 6;
    oneLineBoard[ROWS - 2][0] = 4;
    for (int col = 0; col < COLS; ++col) {
        oneLineBoard[ROWS - 1][col] = 1;
    }
    assert(clearFullLines(oneLineBoard) == 1);
    assert(oneLineBoard[0][0] == 0);
    assert(oneLineBoard[1][0] == 6);
    assert(oneLineBoard[ROWS - 1][0] == 4);

    int twoLinesBoard[ROWS][COLS] = {};
    twoLinesBoard[ROWS - 3][2] = 3;
    for (int col = 0; col < COLS; ++col) {
        twoLinesBoard[ROWS - 2][col] = 1;
        twoLinesBoard[ROWS - 1][col] = 2;
    }
    assert(clearFullLines(twoLinesBoard) == 2);
    assert(twoLinesBoard[ROWS - 1][2] == 3);
    for (int col = 0; col < COLS; ++col) {
        assert(twoLinesBoard[0][col] == 0);
    }

    assert(calculatePoints(0, 0) == 0);
    assert(calculatePoints(1, 0) == 100);
    assert(calculatePoints(2, 0) == 300);
    assert(calculatePoints(3, 0) == 500);
    assert(calculatePoints(4, 0) == 800);

    assert(calculatePoints(0, 1) == 0);
    assert(calculatePoints(1, 1) == 200);
    assert(calculatePoints(2, 1) == 600);
    assert(calculatePoints(3, 1) == 1000);
    assert(calculatePoints(4, 1) == 1600);

    assert(calculatePoints(-1, 0) == 0);
    assert(calculatePoints(5, 0) == 0);
    assert(calculatePoints(1, -1) == 0);

    return 0;
}
