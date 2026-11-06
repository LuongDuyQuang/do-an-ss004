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

    int edgeCollisionBoard[ROWS][COLS] = {};
    assert(!isCollision(edgeCollisionBoard, shape, 0, COLS - 3));
    assert(!isCollision(edgeCollisionBoard, shape, ROWS - 2, 0));
    assert(isCollision(edgeCollisionBoard, shape, -1, 0));

    const int sparseShape[SIZE][SIZE] = {
        {0, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };
    int sparseBoard[ROWS][COLS] = {};
    sparseBoard[0][0] = 7;
    assert(!isCollision(sparseBoard, sparseShape, 0, 0));
    sparseBoard[1][1] = 7;
    assert(isCollision(sparseBoard, sparseShape, 0, 0));

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

    int edgeLockBoard[ROWS][COLS] = {};
    lockPiece(edgeLockBoard, shape, ROWS - 2, COLS - 3, 6);
    assert(edgeLockBoard[ROWS - 2][COLS - 3] == 6);
    assert(edgeLockBoard[ROWS - 2][COLS - 2] == 6);
    assert(edgeLockBoard[ROWS - 1][COLS - 2] == 6);
    assert(edgeLockBoard[ROWS - 1][COLS - 1] == 6);

    int invalidLockBoard[ROWS][COLS] = {};
    invalidLockBoard[0][0] = 3;
    lockPiece(invalidLockBoard, shape, ROWS - 1, 0, 6);
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            if (row == 0 && col == 0) {
                assert(invalidLockBoard[row][col] == 3);
            } else {
                assert(invalidLockBoard[row][col] == 0);
            }
        }
    }

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

    int threeLinesBoard[ROWS][COLS] = {};
    threeLinesBoard[ROWS - 4][1] = 3;
    for (int row = ROWS - 3; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            threeLinesBoard[row][col] = 1;
        }
    }
    assert(clearFullLines(threeLinesBoard) == 3);
    assert(threeLinesBoard[ROWS - 1][1] == 3);

    int fourLinesBoard[ROWS][COLS] = {};
    fourLinesBoard[ROWS - 5][4] = 6;
    for (int row = ROWS - 4; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            fourLinesBoard[row][col] = 2;
        }
    }
    assert(clearFullLines(fourLinesBoard) == 4);
    assert(fourLinesBoard[ROWS - 1][4] == 6);

    int separateLinesBoard[ROWS][COLS] = {};
    separateLinesBoard[ROWS - 2][0] = 3;
    separateLinesBoard[ROWS - 4][COLS - 1] = 4;
    for (int col = 0; col < COLS; ++col) {
        separateLinesBoard[ROWS - 1][col] = 1;
        separateLinesBoard[ROWS - 3][col] = 2;
    }
    assert(clearFullLines(separateLinesBoard) == 2);
    assert(separateLinesBoard[ROWS - 1][0] == 3);
    assert(separateLinesBoard[ROWS - 2][COLS - 1] == 4);

    int preserveBoard[ROWS][COLS] = {};
    preserveBoard[ROWS - 2][1] = 4;
    preserveBoard[ROWS - 3][COLS - 2] = 5;
    for (int col = 0; col < COLS; ++col) {
        preserveBoard[ROWS - 1][col] = 1;
    }
    assert(clearFullLines(preserveBoard) == 1);
    assert(preserveBoard[ROWS - 1][1] == 4);
    assert(preserveBoard[ROWS - 2][COLS - 2] == 5);

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
    assert(calculatePoints(1, 5) == 600);
    assert(calculatePoints(4, 5) == 4800);
    assert(calculatePoints(2, 10) == 3300);

    return 0;
}
