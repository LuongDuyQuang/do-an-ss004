#include "game_input.h"
#include <conio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <windows.h>

using namespace std;


KeyAction pollInput() {
    if (!_kbhit()) return KeyAction::NONE;
    int ch = _getch();

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

void playSound(SoundEffect effect) {
    switch (effect) {
        case SoundEffect::MOVE:       Beep(300, 30); break;
        case SoundEffect::ROTATE:     Beep(500, 40); break;
        case SoundEffect::DROP:       Beep(200, 60); break;
        case SoundEffect::CLEAR_LINE: Beep(700, 80); Beep(900, 100); break;
        case SoundEffect::GAME_OVER:  Beep(400, 150); Beep(300, 150); Beep(200, 300); break;
    }
}

bool showStartMenu(int highScore) {
    while (true) {
        system("cls");
        cout << "=========================================\n";
        cout << "            GAME TETRIS C++              \n";
        cout << "=========================================\n";
        cout << "  DIEM CAO NHAT (HIGH SCORE): " << highScore << "\n";
        cout << "-----------------------------------------\n";
        cout << "  [1] Bat dau choi\n";
        cout << "  [2] Huong dan phim bam\n";
        cout << "  [0] Thoat game\n";
        cout << "=========================================\n";
        cout << "  Lua chon cua ban: ";

        int choice = _getch();
        if (choice == '1') {
            playSound(SoundEffect::CLEAR_LINE);
            return true;
        } else if (choice == '2') {
            system("cls");
            cout << "=== HUONG DAN DIEU KHIEN ===\n";
            cout << " - A / Mui ten Trai  : Di chuyen trai\n";
            cout << " - D / Mui ten Phai  : Di chuyen phai\n";
            cout << " - W / Mui ten Len   : Xoay khoi\n";
            cout << " - S / Mui ten Xuong : Tha roi nhanh (Soft Drop)\n";
            cout << " - Phim Spacebar     : Tha roi tuc thi (Hard Drop)\n";
            cout << " - Phim P / ESC      : Tam dung (Pause)\n\n";
            cout << "Nhan phim bat ky de quay lai Menu...";
            _getch();
        } else if (choice == '0') {
            return false;
        }
    }
}

bool showPauseMenu() {
    system("cls");
    cout << "=========================================\n";
    cout << "          GAME DANG TAM DUNG             \n";
    cout << "=========================================\n";
    cout << "  [P] / [Enter] : Tiep tuc choi\n";
    cout << "  [Q]           : Thoat game\n";
    cout << "=========================================\n";

    while (true) {
        int ch = _getch();
        if (ch == 'p' || ch == 'P' || ch == 13 || ch == 27) return true;
        if (ch == 'q' || ch == 'Q') return false;
    }
}