#include "game_input.h"
#include <conio.h>

KeyAction pollInput() {
    if (!_kbhit()) return KeyAction::NONE;
    int ch = _getch();

    switch (ch) {
        case 'a': case 'A': return KeyAction::MOVE_LEFT;
        case 'd': case 'D': return KeyAction::MOVE_RIGHT;
        case 'w': case 'W': return KeyAction::ROTATE;
        case 'p': case 'P': case 27: return KeyAction::PAUSE;
        case 'q': case 'Q': return KeyAction::QUIT;
        default: return KeyAction::NONE;
    }
    }

    switch (ch) {
        case 'a': case 'A': return KeyAction::MOVE_LEFT;
        case 'd': case 'D': return KeyAction::MOVE_RIGHT;
        case 'w': case 'W': return KeyAction::ROTATE;
        case 's': case 'S': return KeyAction::SOFT_DROP;
        case ' ':           return KeyAction::HARD_DROP;
        case 'p': case 'P': case 27: return KeyAction::PAUSE;
        case 'q': case 'Q': return KeyAction::QUIT;
        default: return KeyAction::NONE;
    }
}