#include "Intelligence.h"
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
    // TODO: Dismiss time for now, just go on for a few rounds.
	return choose_best_move(state_, 5, player);
}

BoardMove Intelligence::choose_best_move(BoardState * state, int time, BoardPlayer player) {
	BoardMove * moves = new BoardMove[MOVE_COUNT];
	int move_count = state->available_moves(player, moves, MOVE_COUNT);

	BoardMove bestMove;
    int bestScore = MIN_SCORE;

	for (int i = 0; i < move_count; i++) {
		BoardState * innerState;
		BoardMove innerMove;

		if (time > 0 && !state->with_move_applied(moves[i], player).is_finished()) {
			innerState = new BoardState(state->with_move_applied(moves[i], player));
			innerMove = choose_best_move(innerState, --time, OPPONENT(player));
		}
		else {
			innerState = new BoardState(*state);
			innerMove = moves[i];
		}

		int score = evaluate(new BoardState(innerState->with_move_applied(innerMove, player)), player);

        if (score > bestScore)
		{
			bestMove = moves[i];
			bestScore = score;
		}
		delete innerState;
	}

	return bestMove;
}
int Intelligence::evaluate(BoardState * state, BoardPlayer player) {


//neighbourx = new int[]{ 1, 1, 0, -1 };
//neighboury = new int[]{ 1, 0, 1, 1 }; 

    BoardPiece* allPieces = state->pieces();

    int score = 0;

    if (state->piece_count() < PIECE_COUNT)
        return 0;

	for (int i = 0; i < PIECE_COUNT; i++) {
		//for each piece
		//is piece flipped
		if (!allPieces[i].is_face_up)
			continue;

	//	bool mypiece = (allPieces[i].player == player);

	//	BoardTile tile = allPieces[i].tile;

		score += best_score(state, player, allPieces, i);
	}
    return score;
}

int Intelligence::best_score(BoardState * state, BoardPlayer player, BoardPiece* allPieces, int i){


	int idx = 0;
	bool mypiece = (allPieces[i].player == player);

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
			return mypiece ? MAX_SCORE : MIN_SCORE;
		}
		
		
		//Check if the edge of the line is blocked by an enemy piece or not
		bool blockedA = state->piece(tile.x + diag[0][j] + lenA * diag[0][j], tile.y + diag[1][j] + lenA * diag[1][j], NULL);
		bool blockedB = state->piece(tile.x  -diag[0][j] - lenB * diag[0][j], tile.y - diag[1][j] - lenB * diag[1][j], NULL);
		
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