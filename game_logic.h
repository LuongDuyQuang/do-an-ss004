#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

const int ROWS = 20;
const int COLS = 10;
const int SIZE = 4;

// Tra ve true neu khoi cham bien hoac cham o da co dinh.
bool isCollision(const int board[ROWS][COLS],
                 const int shape[SIZE][SIZE], int row, int col);

// Co dinh cac o cua khoi vao board khi vi tri hop le.
void lockPiece(int board[ROWS][COLS],
               const int shape[SIZE][SIZE], int row, int col, int value);

#endif
