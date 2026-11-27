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
    std::cout << "PASS: Phan 1 test trang thai GameState thanh cong!\n";
    return 0;

}