#include "Intelligence.h"
#include <iostream>
#include <algorithm>
#include <stdio.h>

using namespace std;

Intelligence::Intelligence() {
	state_ = new BoardState();
}

Intelligence::~Intelligence() {

    delete state_;
}

void Intelligence::apply_move(BoardMove move, BoardPlayer player) {
    state_->apply_move(move, player);
}

BoardMove Intelligence::choose_best_move(int time, BoardPlayer player) {
	BoardMove moves[MOVE_COUNT];
	int move_count = state_->available_moves(player, moves, MOVE_COUNT);

	BoardMove bestMove;
	int bestScore = -20000000;

	for (int i = 0; i < move_count; i++)
    {
        state_->apply_move(moves[i], player);
        int newScore = alpha_beta(4, MIN_SCORE - 1, MAX_SCORE + 1, OPPONENT(player));
        state_->undo_move(moves[i], player);

		if (newScore > bestScore)

		{
			bestMove = moves[i];
			bestScore = newScore;
		}
	}

	cout << "iterations " << iteration_count << endl;
	cout << "prunes " << prune_count << endl;

    assert(bestScore != -20000000);

	return bestMove;
}

int Intelligence::alpha_beta(int depth, int alpha, int beta, BoardPlayer player)
{
	iteration_count++;

    // TODO: Stop if game is finished!
	if (depth == 0)
	{
#if defined _DEBUG
        if (evaluate(player) != -evaluate(OPPONENT(player))) {
            cout << "Evalulate " << player << ": " << evaluate(player) << ", evaluate " << OPPONENT(player) << ": " << evaluate(OPPONENT(player)) << endl;
            assert(evaluate(player) == -evaluate(OPPONENT(player)));
        }
#endif
		return evaluate(player);
	}
	
	if (player == PLAYER_PLAYER1)
	{
		int v = MIN_SCORE - 1;

		BoardMove moves[MOVE_COUNT];
		int move_count = state_->available_moves(player, moves, MOVE_COUNT);
		
		for (int i = 0; i < move_count; i++)
        {
            state_->apply_move(moves[i], player);
            v = max(v, alpha_beta(depth - 1, alpha, beta, OPPONENT(player)));

            state_->undo_move(moves[i], player);

			alpha = max(alpha, v);

			if (beta <= alpha)
			{
				prune_count++;
				break;
			}
		}

		return v;
	}
	else
	{
		int v = MAX_SCORE + 1;

		BoardMove moves[MOVE_COUNT];
		int move_count = state_->available_moves(player, moves, MOVE_COUNT);

		for (int i = 0; i < move_count; i++)
        {
            state_->apply_move(moves[i], player);
            v = min(v, alpha_beta(depth - 1, alpha, beta, OPPONENT(player)));

            state_->undo_move(moves[i], player);

			beta = min(beta, v);

			if (beta <= alpha)
			{
				prune_count++;
				break;
			}
		}

		return v;
	}
}

int Intelligence::evaluate(BoardPlayer player) {
    BoardPiece* allPieces = state_->pieces();

    int score = 0;

	if (state_->piece_count() < PIECE_COUNT)
	{
		for (int i = 0; i < PIECE_COUNT; i++) {
			if (allPieces[i].player == player)
				score += 1;
			else if (allPieces[i].player == OPPONENT(player))
				score -= 1;
		}

		return score;
	}

	for (int i = 0; i < PIECE_COUNT; i++) {
		//for each piece
		//is piece flipped
		if (!allPieces[i].is_face_up)
			continue;

		if (allPieces[i].player == player)
			score += best_score(&allPieces[i]);
		else
			score -= best_score(&allPieces[i]);
	}

    return score;
}

int Intelligence::best_score(BoardPiece * piece){
	int score = 0;

    if (!piece->is_face_up) {
        return 1;
    }

    for (int d = 0; d < DIRECTION_COUNT / 2; d++) {
        // Calculate the lenght of the line.
        int lenA = state_->row_length(piece, d, piece->player);
        int lenB = state_->row_length(piece, DIRECTION_FLIP(d), piece->player);

        // If row is over 4 long, this piece is worth max score.
        if ((score += lenA + 1 + lenB) >= 4) {
            return MAX_SCORE;
        }

        // Check if the edge of the line is blocked by an enemy piece or not.
        bool blockedA = state_->piece_in_direction(piece, d, lenA + 1, NULL);
        bool blockedB = state_->piece_in_direction(piece, DIRECTION_FLIP(d), lenA + 1, NULL);

        // If it's blocked we decrease the score.
        if (blockedA){
            score--;
        }
        if (blockedB){
            score--;
        }
        if (blockedA && blockedB){
            score = 0;
        }
    }

    return score;
}