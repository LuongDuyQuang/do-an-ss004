#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>

#include "game_state.h"
#include "game_logic.h"
#include "tetromino.h"
#include "game_board.h"
#include "game_input.h"

using namespace std;

struct GameSession {
    int board[ROWS][COLS];
    Tetromino currentPiece;
    Tetromino nextPiece;
    GameState state;
    int linesClearedTotal;
    int highScore;
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
    session.highScore = loadHighScore();
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
        playSound(SoundEffect::CLEAR_LINE);
    }

    spawnNextPiece(session);
    if (isCollision(session.board, session.currentPiece.shape,
                    session.currentPiece.y, session.currentPiece.x)) {
        saveHighScore(session.state.score);
        session.highScore = loadHighScore();
        playSound(SoundEffect::GAME_OVER);
        triggerGameOver(session.state);
    }
    session.state.needRedraw = true;
}





void drawGameOverScreen(const GameSession &session) {
    system("cls");
    cout << "=================================\n";
    cout << "            GAME OVER!           \n";
    cout << "=================================\n";
    cout << "  Diem so : " << session.state.score << "\n";
    cout << "  Ky luc  : " << session.highScore << "\n";
    cout << "  Cap do  : " << session.state.level << "\n";
    cout << "---------------------------------\n";
    cout << "  Nhan [Space] de choi lai       \n";
    cout << "  Nhan [Q]     de thoat          \n";
    cout << "=================================\n";
}


// XU LY PHIM BAM 
void processInput(GameSession &session) {
    if (session.state.screen != STATE_PLAYING &&
        session.state.screen != STATE_GAMEOVER) {
        return;
    }

    KeyAction action = pollInput();
    if (action == KeyAction::NONE) return;

    if (session.state.screen == STATE_GAMEOVER) {
        if (action == KeyAction::HARD_DROP) {
            system("cls");
            startNewSession(session, STATE_PLAYING);
        } else if (action == KeyAction::QUIT) {
            session.state.isRunning = false;
        }
        return;
    }

    switch (action) {
        case KeyAction::MOVE_LEFT:
            if (!isCollision(session.board, session.currentPiece.shape,
                             session.currentPiece.y, session.currentPiece.x - 1)) {
                session.currentPiece.x--;
                playSound(SoundEffect::MOVE);
                session.state.needRedraw = true;
            }
            break;
        case KeyAction::MOVE_RIGHT:
            if (!isCollision(session.board, session.currentPiece.shape,
                             session.currentPiece.y, session.currentPiece.x + 1)) {
                session.currentPiece.x++;
                playSound(SoundEffect::MOVE);
                session.state.needRedraw = true;
            }
            break;
        case KeyAction::ROTATE:
            if (rotateWithWallKick(session.currentPiece, session.board)) {
                playSound(SoundEffect::ROTATE);
                session.state.needRedraw = true;
            }
            break;
        case KeyAction::SOFT_DROP:
            if (!isCollision(session.board, session.currentPiece.shape,
                             session.currentPiece.y + 1, session.currentPiece.x)) {
                session.currentPiece.y++;
                resetDropTimer(session.state);
                session.state.needRedraw = true;
            }
            break;
        case KeyAction::HARD_DROP:
            while (!isCollision(session.board, session.currentPiece.shape,
                                session.currentPiece.y + 1, session.currentPiece.x)) {
                session.currentPiece.y++;
            }
            playSound(SoundEffect::DROP);
            lockAndSpawn(session);
            resetDropTimer(session.state);
            break;
        case KeyAction::PAUSE:
            togglePause(session.state);
            break;
        case KeyAction::QUIT:
            saveHighScore(session.state.score);
            session.highScore = loadHighScore();
            playSound(SoundEffect::GAME_OVER);
            triggerGameOver(session.state);
            break;
        case KeyAction::NONE:
            break;
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
            if (showStartMenu(session.highScore)) {
                system("cls");
                startNewSession(session, STATE_PLAYING);
            } else {
                session.state.isRunning = false;
            }
            break;
        case STATE_PLAYING:
            renderFrame(session.board, session.state.score, session.state.level,
                        session.currentPiece.shape, session.currentPiece.y,
                        session.currentPiece.x, session.nextPiece.shape);
            setCursorPosition(0, ROWS + 3);
            cout << "[A/D] Di chuyen | [W] Xoay | [S] Xuong | [Space] Tha | [P] Pause\n";
            break;
        case STATE_PAUSED:
            if (showPauseMenu()) {
                system("cls");
                togglePause(session.state);
            } else {
                startNewSession(session, STATE_MENU);
            }
            break;
        case STATE_GAMEOVER:
            drawGameOverScreen(session);
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
