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

int loadHighScore(const string& filename) {
    ifstream inFile(filename);
    int highScore = 0;
    if (inFile.is_open()) {
        inFile >> highScore;
        inFile.close();
    }
    return highScore;
}

bool saveHighScore(int newScore, const string& filename) {
    int currentHigh = loadHighScore(filename);
    if (newScore > currentHigh) {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            outFile << newScore;
            outFile.close();
            return true;
        }
    }
    return false;
}