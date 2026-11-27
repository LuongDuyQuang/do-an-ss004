#include <iostream>
#include <conio.h>

#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include "game_state.h"

using namespace std;
#define H 20
#define W 15
char board[H][W] = {};

int x, y, b;
char blocks[][4][4] ={
        {{' ','I',' ',' '},
         {' ','I',' ',' '},
         {' ','I',' ',' '},
         {' ','I',' ',' '}},
        {{' ','I',' ',' '},
         {' ','I',' ',' '},
         {' ','I',' ',' '},
         {' ','I',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {'I','I','I','I'},
         {' ',' ',' ',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','T',' ',' '},
         {'T','T','T',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','S','S',' '},
         {'S','S',' ',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {'Z','Z',' ',' '},
         {' ','Z','Z',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {'J',' ',' ',' '},
         {'J','J','J',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ',' ','L',' '},
         {'L','L','L',' '},
         {' ',' ',' ',' '}}
};
bool canMove(int dx, int dy){
    for (int i = 0; i < 4; i++ )
        for (int j = 0; j < 4; j++ )
            if (blocks[b][i][j] != ' ') {
                int xt = x + j + dx;
                int yt = y + i + dy;
                if (xt < 1 || xt >= W-1 || yt >= H-1 ) return false;
                if (board[yt][xt] != ' ') return false;
            }
    return true;
}
void block2Board(){
    for (int i = 0; i < 4; i++ )
        for (int j = 0; j < 4; j++ )
            if (blocks[b][i][j] != ' ')
                board[y+i][x+j] = blocks[b][i][j];
}
void boardDelBlock(){
    for (int i = 0; i < 4; i++ )
        for (int j = 0; j < 4; j++ )
            if (blocks[b][i][j] != ' ')
                board[y+i][x+j] = ' ';
}
void initBoard(){
    for (int i = 0 ; i < H ; i++)
        for (int j = 0 ; j < W ; j++)
            if (i == 0 || i == H-1 || j ==0 || j == W-1) board[i][j] = '#';
            else board[i][j] = ' ';
}
void draw(){
    system("cls");

    for (int i = 0 ; i < H ; i++, cout<<endl)
        for (int j = 0 ; j < W ; j++) cout<<board[i][j];
}

void spawnNewPiece() 
{
    x = 5;
    y = 0;
    b = rand() % 7;
}

void drawMenu() 
{
    system("cls");
    cout << "=================================\n";
    cout << "        TETRIS CONSOLE C++       \n";
    cout << "=================================\n";
    cout << "  [Enter / Space] : Bat dau choi \n";
    cout << "  [A / D / X]     : Trai/Phai/Xuong\n";
    cout << "  [P]             : Tam dung     \n";
    cout << "  [Q]             : Thoat / Dung \n";
    cout << "=================================\n";
}

void drawPauseScreen(const GameState &state)
{
    draw();
    cout << "\n[TAM DUNG] Score: " << state.score
         << " | Level: " << state.level << "\n";
    cout << "Nhan [P] de tiep tuc, [Q] de ve Menu.\n";
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

void removeLine() {}

// BUOC 1 XU LY PHIM BAM THEO TUNG MAN HINH
void processInput(GameState &state) {
    if (!kbhit()) return;
    char c = getch();
    if (state.screen == STATE_MENU) {
        if (c == '\r' || c == ' ') {
            initBoard();
            spawnNewPiece();
            startGame(state);
        } else if (c == 'q' || c == 'Q') {
            state.isRunning = false;
        }
        return;
    }
    if (state.screen == STATE_PAUSED) {
        if (c == 'p' || c == 'P') {
            togglePause(state);
        } else if (c == 'q' || c == 'Q') {
            initGameState(state, STATE_MENU);
        }
        return;
    }
    if (state.screen == STATE_GAMEOVER) {
        if (c == 'r' || c == 'R') {
            initBoard();
            spawnNewPiece();
            startGame(state);
        } else if (c == 'q' || c == 'Q') {
            state.isRunning = false;
        }
        return;
    }
    // Trang thai STATE_PLAYING
    if ((c == 'a' || c == 'A') && canMove(-1, 0)) {
        x--;
        state.needRedraw = true;
    } else if ((c == 'd' || c == 'D') && canMove(1, 0)) {
        x++;
        state.needRedraw = true;
    } else if ((c == 'x' || c == 'X') && canMove(0, 1)) {
        y++;
        resetDropTimer(state);
        state.needRedraw = true;
    } else if (c == 'p' || c == 'P') {
        togglePause(state);
    } else if (c == 'q' || c == 'Q') {
        triggerGameOver(state);
    }
}

// BUOC 2 CAP NHAT TRANG THAI & NHIP ROI DELTA TIME
void updateGameState(GameState &state, double deltaTimeMs) {
    if (!consumeGravityTick(state, deltaTimeMs)) {
        return;
    }
    if (canMove(0, 1)) {
        y++;
    } else {
        block2Board();
        removeLine();
        spawnNewPiece();
        // Khoi moi sinh ra o dinh da bi chan -> Thua cuoc
        if (!canMove(0, 0)) {
            triggerGameOver(state);
        }
    }
}

// BUOC 3 VE MAN HINH KHI CO THAY DOI
void renderGame(GameState &state) {
    if (!state.needRedraw) return;
    state.needRedraw = false;
    switch (state.screen) {
        case STATE_MENU:
            drawMenu();
            break;
        case STATE_PLAYING:
            block2Board();
            draw();
            cout << "Score: " << state.score << " | Level: " << state.level
                 << " | [P] Pause | [Q] Ket thuc\n";
            boardDelBlock();
            break;
        case STATE_PAUSED:
            block2Board();
            drawPauseScreen(state);
            boardDelBlock();
            break;
        case STATE_GAMEOVER:
            drawGameOverScreen(state);
            break;
    }
}


int main()
{
    srand(time(0));
    x = 5; y = 0; b = rand()%7;
    initBoard();
    while (1){
        boardDelBlock();
        if (kbhit()){
            char c = getch();
            if (c == 'a' && canMove(-1,0)) x--;
            if (c == 'd' && canMove( 1,0)) x++;
            if (c == 'x' && canMove( 0,1)) y++;
            if (c == 'q') break;
        }
        if (canMove(0,1)) y++;
        else{
            block2Board();
            removeLine();
            x = 5; y = 0; b = rand()%7;
        }
        block2Board();
        draw();
        _sleep(500);
    }
    return 0;
}

