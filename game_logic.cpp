#include "game_logic.h"

bool isCollision(const int board[ROWS][COLS],
                 const int shape[SIZE][SIZE], int row, int col) {
    for (int shapeRow = 0; shapeRow < SIZE; ++shapeRow) {
        for (int shapeCol = 0; shapeCol < SIZE; ++shapeCol) {
            if (shape[shapeRow][shapeCol] == 0) {
                continue;
            }

            int boardRow = row + shapeRow;
            int boardCol = col + shapeCol;

            // Kiem tra bien truoc khi doc o tren board.
            if (boardRow < 0 || boardRow >= ROWS ||
                boardCol < 0 || boardCol >= COLS) {
                return true;
            }

            if (board[boardRow][boardCol] != 0) {
                return true;
            }
        }
    }

    return false;
}

void lockPiece(int board[ROWS][COLS],
               const int shape[SIZE][SIZE], int row, int col, int value) {
    if (isCollision(board, shape, row, col)) {
        return;
    }

    for (int shapeRow = 0; shapeRow < SIZE; ++shapeRow) {
        for (int shapeCol = 0; shapeCol < SIZE; ++shapeCol) {
            if (shape[shapeRow][shapeCol] != 0) {
                board[row + shapeRow][col + shapeCol] = value;
            }
        }
    }
}
