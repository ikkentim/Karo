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
	int bestScore = MIN_SCORE - 1;

	for (int i = 0; i < move_count; i++)
    {
        state_->apply_move(moves[i], player);
        int newScore = alpha_beta(3, MIN_SCORE - 1, MAX_SCORE + 1, OPPONENT(player));
        state_->undo_move(moves[i], player);

		if (newScore > bestScore)

		{
			bestMove = moves[i];
			bestScore = newScore;
		}
	}

	cout << "iterations " << iteration_count << endl;
	cout << "prunes " << prune_count << endl;

    assert(bestScore != MIN_SCORE - 1);

	return bestMove;
}

int Intelligence::alpha_beta(int depth, int alpha, int beta, BoardPlayer player)
{
	iteration_count++;

	if (depth == 0)
	{
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
			else
				score -= 1;
		}
	}

	for (int i = 0; i < PIECE_COUNT; i++) {
		//for each piece
		//is piece flipped
		if (!allPieces[i].is_face_up)
			continue;

	//	bool mypiece = (allPieces[i].player == player);


		score += best_score(player, allPieces[i]);
	}
    return score;
}

int Intelligence::best_score(BoardPlayer player, BoardPiece piece){

	int idx = 0;
    bool mypiece = piece.player == player;

    BoardTile * tile = piece.tile;

	//Array who store all diagonal
	int diag[2][4];
	//Diag1
	diag[0][0] = diag[1][0] = 1;
	//diag2
	diag[0][1] = 1;
	diag[1][1] = 0;
	//diag3
	diag[0][2] = 0;
	diag[1][2] = 1;
	//diag4
	diag[0][3] = -1;
	diag[1][3] = 1;

	
	for (int j=0; j<4; j++){
		//Calculate the lenght of the line
        int lenA = state_->row_length(tile->position.x, tile->position.y, diag[0][j], diag[1][j], piece.player);
		
        int lenB = state_->row_length(tile->position.x, tile->position.y, -diag[0][j], -diag[1][j], piece.player);

		idx += lenA+1+lenB;
		
		if (idx >= 4){
			return mypiece ? MAX_SCORE : MIN_SCORE;
		}
		
		
		//Check if the edge of the line is blocked by an enemy piece or not
        bool blockedA = state_->piece((tile->position.x + diag[0][j] + lenA) * diag[0][j], (tile->position.y + diag[1][j] + lenA) * diag[1][j], NULL);
        bool blockedB = state_->piece((tile->position.x - diag[0][j] - lenB) * diag[0][j], (tile->position.y - diag[1][j] - lenB) * diag[1][j], NULL);
		
		//if it's blocked we decrease the score
		if (blockedA){
			idx--;
		}
		if (blockedB){
			idx--;
		}
		if (blockedA && blockedB){
			idx = 0;
		}
	}

	return mypiece ? idx : -idx;
}