#include "game_state.h"

void initGameState(GameState &state, ScreenState startScreen)
{
    state.score = 0;
    state.level = 1;
    state.isGameOver = false;
    state.isPaused = (startScreen == STATE_PAUSED);
    state.dropIntervalMs = 500.0;
    state.dropTimerMs = 0.0;
    state.screen = startScreen;
    state.needRedraw = true;
    state.isRunning = true;
}

void startGame(GameState &state)
{
    // Reset trạng thái khi bắt đầu chơi lại
    initGameState(state, STATE_PLAYING);
}

void togglePause(GameState &state)
{
    if (state.screen == STATE_PLAYING) {
        state.isPaused = true;
        state.screen = STATE_PAUSED;
        state.needRedraw = true;
    }
    else if (state.screen == STATE_PAUSED) {
        state.isPaused = false;
        state.screen = STATE_PLAYING;
        state.dropTimerMs = 0.0;
        state.needRedraw = true;
    }
}

void triggerGameOver(GameState &state)
{
    state.isGameOver = true;
    state.isPaused = false;
    state.screen = STATE_GAMEOVER;
    state.needRedraw = true;
}