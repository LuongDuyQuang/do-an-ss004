#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <conio.h>

#include "game_state.h"
#include "game_logic.h"
#include "tetromino.h"
#include "game_board.h"

using namespace std;

struct GameSession {
    int board[ROWS][COLS];
    Tetromino currentPiece;
    Tetromino nextPiece;
    GameState state;
    int linesClearedTotal;
};


void spawnNextPiece(GameSession &session) {
    session.currentPiece = session.nextPiece;
    session.currentPiece.x = 3;
    session.currentPiece.y = 0;
    session.nextPiece = createTetromino(getRandomPieceBag());
}


void startNewSession(GameSession &session, ScreenState startScreen = STATE_MENU) {
    initBoard(session.board);
    initGameState(session.state, startScreen);
    session.linesClearedTotal = 0;
    session.nextPiece = createTetromino(getRandomPieceBag());
    spawnNextPiece(session);
}

void drawMenu() 
{
    system("cls");
    cout << "=================================\n";
    cout << "        TETRIS CONSOLE C++       \n";
    cout << "=================================\n";
    cout << "  [Enter / Space] : Bat dau choi \n";
    cout << "  [A / D / X]     : Trai/Phai/Xuong\n";
    cout << "  [P]             : Tam dung     \n";
    cout << "  [Q]             : Thoat / Dung \n";
    cout << "=================================\n";
}

void drawPauseScreen(const GameState &state)
{
    draw();
    cout << "\n[TAM DUNG] Score: " << state.score
         << " | Level: " << state.level << "\n";
    cout << "Nhan [P] de tiep tuc, [Q] de ve Menu.\n";
}

void drawGameOverScreen(const GameState &state) {
    system("cls");
    cout << "=================================\n";
    cout << "            GAME OVER!           \n";
    cout << "=================================\n";
    cout << "  Diem so : " << state.score << "\n";
    cout << "  Cap do  : " << state.level << "\n";
    cout << "---------------------------------\n";
    cout << "  Nhan [R] de choi lai           \n";
    cout << "  Nhan [Q] de thoat              \n";
    cout << "=================================\n";
}

void removeLine() {}

// BUOC 1 XU LY PHIM BAM THEO TUNG MAN HINH
void processInput(GameState &state) {
    if (!kbhit()) return;
    char c = getch();
    if (state.screen == STATE_MENU) {
        if (c == '\r' || c == ' ') {
            initBoard();
            spawnNewPiece();
            startGame(state);
        } else if (c == 'q' || c == 'Q') {
            state.isRunning = false;
        }
        return;
    }
    if (state.screen == STATE_PAUSED) {
        if (c == 'p' || c == 'P') {
            togglePause(state);
        } else if (c == 'q' || c == 'Q') {
            initGameState(state, STATE_MENU);
        }
        return;
    }
    if (state.screen == STATE_GAMEOVER) {
        if (c == 'r' || c == 'R') {
            initBoard();
            spawnNewPiece();
            startGame(state);
        } else if (c == 'q' || c == 'Q') {
            state.isRunning = false;
        }
        return;
    }
    // Trang thai STATE_PLAYING
    if ((c == 'a' || c == 'A') && canMove(-1, 0)) {
        x--;
        state.needRedraw = true;
    } else if ((c == 'd' || c == 'D') && canMove(1, 0)) {
        x++;
        state.needRedraw = true;
    } else if ((c == 'x' || c == 'X') && canMove(0, 1)) {
        y++;
        resetDropTimer(state);
        state.needRedraw = true;
    } else if (c == 'p' || c == 'P') {
        togglePause(state);
    } else if (c == 'q' || c == 'Q') {
        triggerGameOver(state);
    }
}

// BUOC 2 CAP NHAT TRANG THAI & NHIP ROI DELTA TIME
void updateGameState(GameState &state, double deltaTimeMs) {
    if (!consumeGravityTick(state, deltaTimeMs)) {
        return;
    }
    if (canMove(0, 1)) {
        y++;
    } else {
        block2Board();
        removeLine();
        spawnNewPiece();
        // Khoi moi sinh ra o dinh da bi chan -> Thua cuoc
        if (!canMove(0, 0)) {
            triggerGameOver(state);
        }
    }
}

// BUOC 3 VE MAN HINH KHI CO THAY DOI
void renderGame(GameState &state) {
    if (!state.needRedraw) return;
    state.needRedraw = false;
    switch (state.screen) {
        case STATE_MENU:
            drawMenu();
            break;
        case STATE_PLAYING:
            block2Board();
            draw();
            cout << "Score: " << state.score << " | Level: " << state.level
                 << " | [P] Pause | [Q] Ket thuc\n";
            boardDelBlock();
            break;
        case STATE_PAUSED:
            block2Board();
            drawPauseScreen(state);
            boardDelBlock();
            break;
        case STATE_GAMEOVER:
            drawGameOverScreen(state);
            break;
    }
}


void runGameLoop() {
    GameState state;
    initGameState(state, STATE_MENU);
    initBoard();
    spawnNewPiece();

    auto lastTime = chrono::steady_clock::now();

    while (state.isRunning) {
        auto currentTime = chrono::steady_clock::now();
        chrono::duration<double, milli> elapsed = currentTime - lastTime;
        lastTime = currentTime;

        processInput(state);
        updateGameState(state, elapsed.count());
        renderGame(state);

        this_thread::sleep_for(chrono::milliseconds(16));
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    runGameLoop();
    return 0;
}

