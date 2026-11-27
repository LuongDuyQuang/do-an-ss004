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


void lockAndSpawn(GameSession &session) {
    lockPiece(session.board, session.currentPiece.shape,
              session.currentPiece.y, session.currentPiece.x,
              static_cast<int>(session.currentPiece.type));

    int cleared = clearFullLines(session.board);
    if (cleared > 0) {
        session.state.score += calculatePoints(cleared, session.state.level);
        session.linesClearedTotal += cleared;
        session.state.level = 1 + session.linesClearedTotal / 10;

        double nextInterval = 500.0 - (session.state.level - 1) * 40.0;
        session.state.dropIntervalMs = (nextInterval > 100.0) ? nextInterval : 100.0;
    }

    spawnNextPiece(session);
    if (isCollision(session.board, session.currentPiece.shape,
                    session.currentPiece.y, session.currentPiece.x)) {
        triggerGameOver(session.state);
    }
    session.state.needRedraw = true;
}

void drawMenu() {
    system("cls");
    cout << "=================================\n";
    cout << "        TETRIS CONSOLE C++       \n";
    cout << "=================================\n";
    cout << "  [Enter]       : Bat dau choi   \n";
    cout << "  [A / D]       : Trai / Phai    \n";
    cout << "  [W]           : Xoay khoi (SRS)\n";
    cout << "  [S / X]       : Roi nhanh      \n";
    cout << "  [Space]       : Tha tuc thi    \n";
    cout << "  [P]           : Tam dung       \n";
    cout << "  [Q]           : Thoat / Dung   \n";
    cout << "=================================\n";
}

void drawPauseScreen(const GameSession &session) {
    renderFrame(session.board, session.state.score, session.state.level,
                session.currentPiece.shape, session.currentPiece.y,
                session.currentPiece.x, session.nextPiece.shape);
    setCursorPosition(0, ROWS + 3);
    cout << "[TAM DUNG] Nhan [P] de tiep tuc, [Q] de ve Menu.\n";
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


// XU LY PHIM BAM 
void processInput(GameSession &session) {
    if (!_kbhit()) return;
    int c = _getch();

    if (session.state.screen == STATE_MENU) {
        if (c == '\r' || c == ' ') {
            system("cls");
            startNewSession(session, STATE_PLAYING);
        } else if (c == 'q' || c == 'Q') {
            session.state.isRunning = false;
        }
        return;
    }

    if (session.state.screen == STATE_PAUSED) {
        if (c == 'p' || c == 'P') {
            system("cls");
            togglePause(session.state);
        } else if (c == 'q' || c == 'Q') {
            startNewSession(session, STATE_MENU);
        }
        return;
    }

    if (session.state.screen == STATE_GAMEOVER) {
        if (c == 'r' || c == 'R') {
            system("cls");
            startNewSession(session, STATE_PLAYING);
        } else if (c == 'q' || c == 'Q') {
            session.state.isRunning = false;
        }
        return;
    }

    if ((c == 'a' || c == 'A') &&
        !isCollision(session.board, session.currentPiece.shape,
                     session.currentPiece.y, session.currentPiece.x - 1)) {
        session.currentPiece.x--;
        session.state.needRedraw = true;
    } else if ((c == 'd' || c == 'D') &&
               !isCollision(session.board, session.currentPiece.shape,
                            session.currentPiece.y, session.currentPiece.x + 1)) {
        session.currentPiece.x++;
        session.state.needRedraw = true;
    } else if (c == 'w' || c == 'W') {
        if (rotateWithWallKick(session.currentPiece, session.board)) {
            session.state.needRedraw = true;
        }
    } else if ((c == 's' || c == 'S' || c == 'x' || c == 'X') &&
               !isCollision(session.board, session.currentPiece.shape,
                            session.currentPiece.y + 1, session.currentPiece.x)) {
        session.currentPiece.y++;
        resetDropTimer(session.state);
        session.state.needRedraw = true;
    } else if (c == ' ') {
        while (!isCollision(session.board, session.currentPiece.shape,
                            session.currentPiece.y + 1, session.currentPiece.x)) {
            session.currentPiece.y++;
        }
        lockAndSpawn(session);
        resetDropTimer(session.state);
    } else if (c == 'p' || c == 'P') {
        togglePause(session.state);
    } else if (c == 'q' || c == 'Q') {
        triggerGameOver(session.state);
    }
}


// CAP NHAT TRANG THAI & NHIP ROI 
void updateGameState(GameSession &session, double deltaTimeMs) {
    if (!consumeGravityTick(session.state, deltaTimeMs)) {
        return;
    }

    if (!isCollision(session.board, session.currentPiece.shape,
                     session.currentPiece.y + 1, session.currentPiece.x)) {
        session.currentPiece.y++;
    } else {
        lockAndSpawn(session);
    }
}

// VE MAN HINH 
void renderGame(GameSession &session) {
    if (!session.state.needRedraw) return;
    session.state.needRedraw = false;

    switch (session.state.screen) {
        case STATE_MENU:
            drawMenu();
            break;
        case STATE_PLAYING:
            renderFrame(session.board, session.state.score, session.state.level,
                        session.currentPiece.shape, session.currentPiece.y,
                        session.currentPiece.x, session.nextPiece.shape);
            setCursorPosition(0, ROWS + 3);
            cout << "[A/D] Di chuyen | [W] Xoay | [S] Xuong | [Space] Tha | [P] Pause\n";
            break;
        case STATE_PAUSED:
            drawPauseScreen(session);
            break;
        case STATE_GAMEOVER:
            drawGameOverScreen(session.state);
            break;
    }
}

void runGameLoop() {
    hideCursor();
    GameSession session;
    startNewSession(session, STATE_MENU);

    auto lastTime = chrono::steady_clock::now();

    while (session.state.isRunning) {
        auto currentTime = chrono::steady_clock::now();
        chrono::duration<double, milli> elapsed = currentTime - lastTime;
        lastTime = currentTime;

        processInput(session);
        updateGameState(session, elapsed.count());
        renderGame(session);

        this_thread::sleep_for(chrono::milliseconds(16));
    }
}

int main() {
    runGameLoop();
    return 0;
}
