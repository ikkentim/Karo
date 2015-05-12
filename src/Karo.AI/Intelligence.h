#pragma once

#include "BoardState.h"

#define NEIGHBOUR_COUNT     (4)

#define NEIGHBOUR_COUNT (4)

#define MAX_SCORE (9001)
#define MIN_SCORE (-9001)

class Intelligence
{
public:
    Intelligence();
    ~Intelligence();
    void apply_move(BoardMove move, BoardPlayer player);
    BoardMove choose_best_move(int depth, BoardPlayer player);
    int evaluate(BoardState * state, BoardPlayer player);

private:
    const int* neighbourx = new int[]{ 1, 1, 0, -1 };
    const int* neighboury = new int[]{ 1, 0, 1, 1 };

    BoardMove choose_best_move(BoardState * state, int depth, BoardPlayer player);

	BoardState * state_ = 0;
};