#include "Intelligence.h"

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
	return choose_best_move(state_, 12, player);
}

BoardMove Intelligence::choose_best_move(BoardState * state, int time, BoardPlayer player) {
	BoardMove * moves = new BoardMove[MOVE_COUNT];
	int move_count = state->available_moves(player, moves, MOVE_COUNT);

	BoardMove bestMove;
    int bestScore = MIN_SCORE;

	for (int i = 0; i < move_count; i++) {
		BoardState * innerState;
		BoardMove innerMove;

		if (time > 0) {
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

        bool mypiece = (allPieces[i].player == player);

        BoardTile tile = allPieces[i].tile;
        for (int j = 0; j <= NEIGHBOUR_COUNT; j++) {

            // for each neighbour
            int neighbourscore =1;//
            //state->row_length(tile.x, tile.y, neighbourx[j], neighboury[j], player) + 
            //    1 + 
            //    state->row_length(tile.x, tile.y, -neighbourx[j], -neighboury[j], player);

            //check for winstate
            if (neighbourscore >= 4)
                    return mypiece ? MAX_SCORE : MIN_SCORE;
       
            score += mypiece ? neighbourscore : -neighbourscore;
        }
    }

    return score;
}