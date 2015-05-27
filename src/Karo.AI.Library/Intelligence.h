#pragma once

#include <BoardState.h>
#include <map>

#define ZOBRIST_TYPES	(5) //amount of different tile/piece types


using namespace std;
class Intelligence
{
private:
	BoardState * state_ = 0;
	int prune_count = 0;
	int iteration_count = 0;
	int zobrist_randoms[TILE_COUNT * TILE_COUNT][ZOBRIST_TYPES];
	map<int, int> trans_table;
public:
	Intelligence();
	Intelligence(BoardState*);
	~Intelligence();
    BoardState * state() const { return state_; }
	void apply_move(BoardMove move, BoardPlayer player);
    BoardMove choose_best_move(int depth, BoardPlayer player);
	int evaluate(BoardPlayer player);
	int evaluation_count = 0;
private:
	int alpha_beta(int depth, int alpha, int beta, BoardPlayer player);
	int zobrist_hash(BoardPlayer player);
	int piece_score(BoardPiece * piece);
};