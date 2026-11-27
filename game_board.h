#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include "game_logic.h"

// Khoi tao ban co
void initBoard(int board[ROWS][COLS]);

// Di chuyen con tro Console
void setCursorPosition(int x, int y);

// An con tro nhap nhay
void hideCursor();

// Hien thi ban co
void renderBoard(
    const int board[ROWS][COLS],
    const int activeShape[SIZE][SIZE],
    int activeRow,
    int activeCol
);

// Hien thi Score va Level
void renderHUD(int score, int level);

// Hien thi khoi tiep theo
void renderNextPiece(
    const int nextShape[SIZE][SIZE]
);

// Hien thi toan bo giao dien
void renderFrame(
    const int board[ROWS][COLS],
    int score,
    int level,
    const int activeShape[SIZE][SIZE],
    int activeRow,
    int activeCol,
    const int nextShape[SIZE][SIZE]
);

#endif
