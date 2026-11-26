#ifndef GAME_INPUT_H
#define GAME_INPUT_H

enum class KeyAction {
    NONE,
    MOVE_LEFT,
    MOVE_RIGHT,
    ROTATE,
    SOFT_DROP,
    HARD_DROP,
    PAUSE,
    QUIT
};

KeyAction pollInput();
int loadHighScore(const std::string& filename = "highscore.txt");
bool saveHighScore(int newScore, const std::string& filename = "highscore.txt");
void playSound(SoundEffect effect);
bool showStartMenu(int highScore);
bool showPauseMenu();

#endif