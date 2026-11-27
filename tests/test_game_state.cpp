#include <cassert>
#include <iostream>

#include "../game_state.h"

int main()
{
    GameState state;
    initGameState(state);
    assert(state.score == 0);
    assert(state.level == 1);
    assert(!state.isGameOver);
    assert(!state.isPaused);
    assert(state.dropIntervalMs == 500.0);
    assert(state.dropTimerMs == 0.0);
    assert(state.screen == STATE_MENU);
    assert(state.needRedraw);
    assert(state.isRunning);


    togglePause(state);
    assert(state.screen == STATE_MENU);
    assert(!state.isPaused);


    // Test chuyen trang thai playing -> paused -> playing
    startGame(state);
    assert(state.screen == STATE_PLAYING);
    assert(!state.isPaused);

    state.needRedraw = false;
    togglePause(state);
    assert(state.screen == STATE_PAUSED);
    assert(state.isPaused);
    assert(state.needRedraw);

    state.dropTimerMs = 250.0; // Gia lap dong ho dang dem do
    state.needRedraw = false;
    togglePause(state);
    assert(state.screen == STATE_PLAYING);
    assert(!state.isPaused);
    assert(state.dropTimerMs == 0.0); // Tiep tuc choi phai reset timer
    assert(state.needRedraw);


    // Test chuyen sang GAMEOVER khi dang PAUSE hoac PLAYING
    togglePause(state);
    state.needRedraw = false;
    triggerGameOver(state);
    assert(state.isGameOver);
    assert(!state.isPaused);
    assert(state.screen == STATE_GAMEOVER);
    assert(state.needRedraw);

    // Test Delta Time khong chay khi dang GAMEOVER hoac MENU
    assert(!consumeGravityTick(state, 1000.0));
    GameState timerState;
    initGameState(timerState); // Dang o MENU
    assert(!consumeGravityTick(timerState, 600.0));
    assert(timerState.dropTimerMs == 0.0);


    // Test tich luy Delta Time khi dang PLAYING
    startGame(timerState);
    assert(!consumeGravityTick(timerState, -10.0));
    assert(!consumeGravityTick(timerState, 0.0));
    assert(!consumeGravityTick(timerState, 200.0));
    assert(!consumeGravityTick(timerState, 200.0));
    assert(timerState.dropTimerMs == 400.0);

    timerState.needRedraw = false;
    assert(consumeGravityTick(timerState, 150.0)); // Tong 550ms >= 500ms
    assert(timerState.dropTimerMs == 50.0);        // Giu lai 50ms du
    assert(timerState.needRedraw);


    // Test resetDropTimer va chong tut nhieu o khi lag lau
    resetDropTimer(timerState);
    assert(timerState.dropTimerMs == 0.0);
    assert(consumeGravityTick(timerState, 1500.0));
    assert(timerState.dropTimerMs == 0.0);

    // Test dang PAUSE thi bo dem khong duoc cong don
    consumeGravityTick(timerState, 300.0);
    togglePause(timerState);
    assert(!consumeGravityTick(timerState, 400.0));
    assert(timerState.dropTimerMs == 300.0);


    std::cout << "PASS: Toan bo unit test GameState thanh cong!\n";
    return 0;

}

