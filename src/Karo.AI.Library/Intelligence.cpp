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

		int newScore = alpha_beta(innerState, 4, MIN_SCORE - 1, MAX_SCORE + 1, OPPONENT(player));

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

int Intelligence::alpha_beta(BoardState * state, int depth, int alpha, int beta, BoardPlayer player)
{
	iteration_count++;

	if (depth == 0)
	{
		// TODO: this if statement is debug
		if (evaluate(state, player) != evaluate(state, OPPONENT(player)) * -1)
		{
			cout << "current player " << evaluate(state, player) << endl;
			cout << "current player " << evaluate(state, OPPONENT(player)) << endl;

			assert(0 && "Evaluation is not zero sum");
		}
			

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

	//	bool mypiece = (allPieces[i].player == player);


		if (allPieces[i].player == player)
		{
			score += best_score(state, allPieces, i);
		}
		else
		{
			score -= best_score(state, allPieces, i);
		}
	}

    return score;
}

int Intelligence::best_score(BoardState * state, BoardPiece* allPieces, int i){

	int idx = 0;

	BoardTile tile = allPieces[i].tile;

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
		int lenA = state->row_length(tile.x, tile.y, diag[0][j], diag[1][j], allPieces[i].player);
		
		int lenB = state->row_length(tile.x, tile.y, -diag[0][j], -diag[1][j], allPieces[i].player);

		idx += lenA+1+lenB;
		
		if (idx >= 4){
			return MAX_SCORE;
		}
		
		
		//Check if the edge of the line is blocked by an enemy piece or not
		bool blockedA = state->piece((tile.x + diag[0][j] + lenA) * diag[0][j], (tile.y + diag[1][j] + lenA) * diag[1][j], NULL);
		bool blockedB = state->piece((tile.x  -diag[0][j] - lenB) * diag[0][j], (tile.y - diag[1][j] - lenB) * diag[1][j], NULL);
		
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

	return idx;
}