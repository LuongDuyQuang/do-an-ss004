#include "tetromino.h"
#include <algorithm>
#include <random>
#include <vector>

namespace
{
    const int SHAPES[8][SIZE][SIZE] =
    {
        {
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 0, 0, 0},
            {1, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 0, 0, 0},
            {2, 0, 0, 0},
            {2, 2, 2, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 0, 0, 0},
            {0, 0, 3, 0},
            {3, 3, 3, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 0, 0, 0},
            {0, 4, 4, 0},
            {0, 4, 4, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 0, 0, 0},
            {0, 5, 5, 0},
            {5, 5, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 0, 0, 0},
            {0, 6, 0, 0},
            {6, 6, 6, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 0, 0, 0},
            {7, 7, 0, 0},
            {0, 7, 7, 0},
            {0, 0, 0, 0}
        }
    };
}

Tetromino createTetromino(PieceType type, int startX, int startY)
{
    Tetromino piece;
    piece.x = startX;
    piece.y = startY;
    piece.type = type;
    piece.rotation = 0;

    int idx = static_cast<int>(type);
    if (idx < 0 || idx > 7)
    {
        idx = 0;
    }

    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            piece.shape[i][j] = SHAPES[idx][i][j];
        }
    }

    return piece;
}