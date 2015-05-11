#pragma once

#include "BoardState.h"

class Intelligence
{
public:
    Intelligence();
    ~Intelligence();
    void apply_move(BoardMove move, BoardPlayer player);
    BoardMove choose_best_move(int depth, BoardPlayer player);
    int evaluate(BoardState * state, BoardPlayer player);

private:
	BoardState * state_;
    BoardMove choose_best_move(BoardState * state, int depth, BoardPlayer player);
};