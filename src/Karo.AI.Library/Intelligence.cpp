#include "Intelligence.h"
#include <iostream>
#include <algorithm>

using namespace std;

Intelligence::Intelligence() {
	state_ = new BoardState();
}

Intelligence::Intelligence(BoardState * state) {
	state_ = state;
}

Intelligence::~Intelligence() {

    delete state_;
}

void Intelligence::apply_move(BoardMove move, BoardPlayer player) {
    auto state = state_->with_move_applied(move, player);

    state_ = new BoardState(state);
}
BoardMove Intelligence::choose_best_move(int time, BoardPlayer player) {
	BoardMove * moves = new BoardMove[MOVE_COUNT];
	int move_count = state_->available_moves(player, moves, MOVE_COUNT);

	BoardMove bestMove;
	int bestScore = MIN_SCORE - 1;

	for (int i = 0; i < move_count; i++)
	{
		BoardState * innerState;

		innerState = new BoardState(state_->with_move_applied(moves[i], player));

		int newScore = alpha_beta(innerState, 3, MIN_SCORE - 1, MAX_SCORE + 1, OPPONENT(player));

		if (newScore > bestScore)
		{
			bestMove = moves[i];
			bestScore = newScore;
		}
	}

	cout << "iterations " << iteration_count << endl;
	cout << "prunes " << prune_count << endl;

	return bestMove;
}

BoardMove Intelligence::choose_best_move(BoardState * state, BoardMove move, int alpha, int beta, int time, BoardPlayer player, int& v) {
	BoardMove * moves = new BoardMove[MOVE_COUNT];
	int move_count = state->available_moves(player, moves, MOVE_COUNT);
	
	BoardMove bestMove;
	int bestScore = MIN_SCORE - 1;

	for (int i = 0; i < move_count; i++)
	{
		BoardState * innerState;

		innerState = new BoardState(state->with_move_applied(moves[i], player));

		int newScore = alpha_beta(innerState, time, MIN_SCORE - 1, MAX_SCORE + 1, OPPONENT(player));

		if (newScore > bestScore)
		{
			bestMove = moves[i];
			bestScore = newScore;
		}
	}

	return bestMove;
}

int Intelligence::alpha_beta(BoardState * state, int depth, int alpha, int beta, BoardPlayer player)
{
	iteration_count++;

	if (depth == 0)
	{
		return evaluate(state, player);
	}
	
	if (player == PLAYER_PLAYER1)
	{
		int v = MIN_SCORE - 1;

		BoardMove * moves = new BoardMove[MOVE_COUNT];
		int move_count = state->available_moves(player, moves, MOVE_COUNT);
		
		for (int i = 0; i < move_count; i++)
		{
			BoardState * innerState;

			innerState = new BoardState(state->with_move_applied(moves[i], player));

			v = max(v, alpha_beta(innerState, depth - 1, alpha, beta, OPPONENT(player)));

			alpha = max(alpha, v);

			if (beta <= alpha)
			{
				prune_count++;
				break;
			}

			delete innerState;
			innerState = 0;
		}

		return v;
	}
	else
	{
		int v = MAX_SCORE + 1;

		BoardMove * moves = new BoardMove[MOVE_COUNT];
		int move_count = state->available_moves(player, moves, MOVE_COUNT);

		for (int i = 0; i < move_count; i++)
		{
			BoardState * innerState;

			innerState = new BoardState(state->with_move_applied(moves[i], player));

			v = min(v, alpha_beta(innerState, depth - 1, alpha, beta, OPPONENT(player)));

			beta = min(beta, v);

			if (beta <= alpha)
			{
				prune_count++;
				break;
			}

			delete innerState;
			innerState = 0;
		}

		return v;
	}
}

int Intelligence::evaluate(BoardState * state, BoardPlayer player) {


//neighbourx = new int[]{ 1, 1, 0, -1 };
//neighboury = new int[]{ 1, 0, 1, 1 }; 

    BoardPiece* allPieces = state->pieces();

    int score = 0;

	if (state->piece_count() < PIECE_COUNT)
	{
		for (int i = 0; i < PIECE_COUNT; i++) {
			if (allPieces[i].player == player)
				score += 1;
			else
				score -= 1;
		}
	}

    for (int i = 0; i < PIECE_COUNT; i++) {
        //for each piece
        //is piece flipped
        if (!allPieces[i].is_face_up)
            continue;

        bool mypiece = (allPieces[i].player == player);

        BoardTile tile = allPieces[i].tile;

#define TRY_OFFSET(ox,oy, idx, oidx); int idx = state->row_length(tile.x, tile.y, (ox), (oy), player) + \
            1 + \
            state->row_length(tile.x, tile.y, -(ox), -(oy), player); \
			int oidx = state->row_length(tile.x, tile.y, (ox), (oy), OPPONENT(player)) + \
            1 + \
            state->row_length(tile.x, tile.y, -(ox), -(oy), OPPONENT(player)); \
            if(idx >= 4) { return MIN_SCORE; } else if (oidx >= 4) { return MAX_SCORE; } else { score += mypiece ? idx : -idx; }

        TRY_OFFSET(1, 1, d1, od1);
        TRY_OFFSET(1, 0, d2, od2);
        TRY_OFFSET(0, 1, d3, od3);
        TRY_OFFSET(-1, 1, d4, od4);

#undef TRY_OFFSET
    }

    return score;
}