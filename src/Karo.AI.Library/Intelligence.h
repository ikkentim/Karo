#pragma once

#include "BoardState.h"

#define NEIGHBOUR_COUNT     (4)
#define MAX_SCORE           (9001)
#define MIN_SCORE           (-9001)

class Intelligence
{
public:
	Intelligence();
	Intelligence(BoardState*);
	~Intelligence();
	void apply_move(BoardMove move, BoardPlayer player);
	BoardMove choose_best_move(int depth, BoardPlayer player);
	int evaluate(BoardPlayer player);
    BoardState * state() const { return state_; }
private:
	int alpha_beta(int depth, int alpha, int beta, BoardPlayer player);


	BoardState * state_ = 0;

	int prune_count = 0;
	int iteration_count = 0;

	int best_score(BoardPiece * piece);
};