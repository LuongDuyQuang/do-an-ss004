#include "game_board.h"

#include <iostream>
#include <iomanip>

using namespace std;


// =====================================
// 1. KHOI TAO BAN CO
// =====================================

void initBoard(int board[ROWS][COLS]) {

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = 0;
        }
    }
}


// =====================================
// 2. DI CHUYEN CON TRO CONSOLE
// =====================================

void setCursorPosition(int x, int y) {
    cout << "\033[" << y + 1 << ";" << x + 1 << "H";
}

// =====================================
// 3. AN CON TRO NHAP NHAY
// =====================================

void hideCursor() {
    cout << "\033[?25l";
}

// =====================================
// 4. HIEN THI BAN CO
// =====================================

void renderBoard(
    const int board[ROWS][COLS],
    const int activeShape[SIZE][SIZE],
    int activeRow,
    int activeCol
) {

    setCursorPosition(0, 0);

    // Ve vien tren
    cout << "+";

    for (int j = 0; j < COLS; j++) {
        cout << "--";
    }

    cout << "+\n";


    // Ve noi dung ban co
    for (int i = 0; i < ROWS; i++) {

        cout << "|";

        for (int j = 0; j < COLS; j++) {

            bool isActive = false;

            // Kiem tra o hien tai co thuoc khoi dang roi khong
            int shapeRow = i - activeRow;
            int shapeCol = j - activeCol;

            if (shapeRow >= 0 && shapeRow < SIZE &&
                shapeCol >= 0 && shapeCol < SIZE) {

                if (activeShape[shapeRow][shapeCol] != 0) {
                    isActive = true;
                }
            }

            // Hien thi khoi dang roi hoac khoi da co dinh
            if (isActive || board[i][j] != 0) {
                cout << "[]";
            }
            else {
                cout << "  ";
            }
        }

        cout << "|\n";
    }


    // Ve vien duoi
    cout << "+";

    for (int j = 0; j < COLS; j++) {
        cout << "--";
    }

    cout << "+\n";
}


// =====================================
// 5. HIEN THI SCORE VA LEVEL
// =====================================

void renderHUD(int score, int level) {

    setCursorPosition(25, 2);

    cout << "===== TETRIS =====";

    setCursorPosition(25, 4);

    cout << "SCORE: "
         << setw(10) << left << score;

    setCursorPosition(25, 6);

    cout << "LEVEL: "
         << setw(10) << left << level;
}


// =====================================
// 6. HIEN THI KHOI TIEP THEO
// =====================================

void renderNextPiece(
    const int nextShape[SIZE][SIZE]
) {

    setCursorPosition(25, 9);

    cout << "NEXT PIECE:       ";

    for (int i = 0; i < SIZE; i++) {

        setCursorPosition(25, 11 + i);

        for (int j = 0; j < SIZE; j++) {

            if (nextShape[i][j] != 0) {
                cout << "[]";
            }
            else {
                cout << "  ";
            }
        }

        // Xoa phan du cua dong cu
        cout << "        ";
    }
}


// =====================================
// 7. HIEN THI TOAN BO GIAO DIEN
// =====================================

void renderFrame(
    const int board[ROWS][COLS],
    int score,
    int level,
    const int activeShape[SIZE][SIZE],
    int activeRow,
    int activeCol,
    const int nextShape[SIZE][SIZE]
) {

    // Hien thi ban co va khoi dang roi
    renderBoard(
        board,
        activeShape,
        activeRow,
        activeCol
    );

    // Hien thi diem va cap do
    renderHUD(score, level);

    // Hien thi khoi tiep theo
    renderNextPiece(nextShape);

    cout.flush();
}
