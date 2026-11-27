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

void rotateClockwise(Tetromino& piece)
{
    if (piece.type == PIECE_O)
    {
        return;
    }

    int temp[SIZE][SIZE];
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            temp[j][SIZE - 1 - i] = piece.shape[i][j];
        }
    }

    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            piece.shape[i][j] = temp[i][j];
        }
    }

    piece.rotation = (piece.rotation + 1) % 4;
}

namespace
{
    const int SRS_KICKS_JLSTZ[4][5][2] =
    {
        {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}},
        {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}},
        {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},
        {{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}}
    };

    const int SRS_KICKS_I[4][5][2] =
    {
        {{0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2}},
        {{0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1}},
        {{0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2}},
        {{0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1}}
    };
}

bool rotateWithWallKick(Tetromino& piece, const int board[ROWS][COLS])
{
    if (piece.type == PIECE_O)
    {
        return true;
    }

    int oldRotation = piece.rotation;
    Tetromino rotated = piece;
    rotateClockwise(rotated);

    const int (*kickTable)[2] = (piece.type == PIECE_I)
                                    ? SRS_KICKS_I[oldRotation]
                                    : SRS_KICKS_JLSTZ[oldRotation];

    for (int i = 0; i < 5; ++i)
    {
        int dx = kickTable[i][0];
        int dy = kickTable[i][1];

        if (!isCollision(board, rotated.shape, rotated.y + dy, rotated.x + dx))
        {
            rotated.x += dx;
            rotated.y += dy;
            piece = rotated;
            return true;
        }
    }

    return false;
}

PieceType getRandomPieceBag()
{
    static std::vector<PieceType> bag;
    static std::mt19937 rng(std::random_device{}());

    if (bag.empty())
    {
        bag = {PIECE_I, PIECE_J, PIECE_L, PIECE_O, PIECE_S, PIECE_T, PIECE_Z};
        std::shuffle(bag.begin(), bag.end(), rng);
    }

    PieceType nextPiece = bag.back();
    bag.pop_back();
    return nextPiece;
}