#include "tetromino.h"
#include <cassert>
#include <iostream>
#include <set>

void printShape(const Tetromino& piece)
{
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            std::cout << (piece.shape[i][j] != 0 ? '#' : '.') << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

bool isSameShape(const Tetromino& a, const Tetromino& b)
{
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            if (a.shape[i][j] != b.shape[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

int main()
{
    std::cout << "=== TEST 1: XOAY KHOI T 4 LAN ===\n";
    Tetromino originalT = createTetromino(PIECE_T);
    Tetromino testT = originalT;

    std::cout << "Ban dau:\n";
    printShape(testT);

    for (int step = 1; step <= 4; ++step)
    {
        rotateClockwise(testT);
        std::cout << "Sau lan xoay " << step << ":\n";
        printShape(testT);
    }

    assert(isSameShape(originalT, testT));
    assert(testT.rotation == 0);
    std::cout << "[PASS] Xoay 4 lan tro ve trang thai ban dau.\n\n";

    std::cout << "=== TEST 2: SRS WALL KICK VOI game_logic.cpp ===\n";
    int board[ROWS][COLS] = {};
    Tetromino pieceI = createTetromino(PIECE_I, 7, 5);
    rotateClockwise(pieceI);

    bool rotated = rotateWithWallKick(pieceI, board);
    assert(rotated);
    assert(pieceI.x == 6);
    assert(!isCollision(board, pieceI.shape, pieceI.y, pieceI.x));
    std::cout << "[PASS] Wall Kick thanh cong, toa do x lui ve: " << pieceI.x << "\n\n";

    std::cout << "=== TEST 3: 7-BAG RANDOMIZER ===\n";
    for (int bagIndex = 1; bagIndex <= 2; ++bagIndex)
    {
        std::set<PieceType> seen;
        std::cout << "Bag " << bagIndex << ": ";
        for (int i = 0; i < 7; ++i)
        {
            PieceType p = getRandomPieceBag();
            seen.insert(p);
            std::cout << static_cast<int>(p) << ' ';
        }
        std::cout << '\n';
        assert(seen.size() == 7);
    }
    std::cout << "[PASS] Moi tui 7 khoi deu du 7 loai khac nhau.\n";
    return 0;
}

