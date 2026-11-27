#ifndef GAME_STATE_H
#define GAME_STATE_H

enum ScreenState {
    STATE_MENU,
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAMEOVER
};

struct GameState {
    int score;
    int level;
    bool isGameOver;
    bool isPaused;
    double dropIntervalMs;
    double dropTimerMs;
    ScreenState screen;
    bool needRedraw;
    bool isRunning;
};

void initGameState(GameState &state, ScreenState startScreen = STATE_MENU);

void startGame(GameState &state);

void togglePause(GameState &state);

void triggerGameOver(GameState &state);

bool consumeGravityTick(GameState &state, double deltaTimeMs);

void resetDropTimer(GameState &state);

#endif



