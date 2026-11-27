#pragma once

#include "game_logic.h"

enum PieceType
{
    PIECE_NONE = 0,
    PIECE_I = 1,
    PIECE_J = 2,
    PIECE_L = 3,
    PIECE_O = 4,
    PIECE_S = 5,
    PIECE_T = 6,
    PIECE_Z = 7
};

struct Tetromino
{
    int x;
    int y;
    PieceType type;
    int rotation;
    int shape[SIZE][SIZE];
};