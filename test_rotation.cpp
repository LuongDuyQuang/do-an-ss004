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

    return 0;
}