#pragma once

#include <BoardState.h>

class Intelligence
{
private:
	BoardState * state_ = 0;
	int prune_count = 0;
	int iteration_count = 0;

public:
	Intelligence();
	Intelligence(BoardState*);
	~Intelligence();
    BoardState * state() const { return state_; }
	void apply_move(BoardMove move, BoardPlayer player);
	BoardMove choose_best_move(int depth, BoardPlayer player);
private:
	int evaluate(BoardPlayer player);
	int alpha_beta(int depth, int alpha, int beta, BoardPlayer player);
	int piece_score(BoardPiece * piece);
};