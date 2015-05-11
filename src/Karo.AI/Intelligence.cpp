#include "stdafx.h"
#include "Intelligence.h"

Intelligence::Intelligence() {
    state_ = new BoardState();
}

Intelligence::~Intelligence() {
    delete state_;
}

void Intelligence::apply_move(BoardMove move, BoardPlayer player) {
    auto state = state_->with_move_applied(move, player);

    delete state_;
    state_ = new BoardState(state);
}
BoardMove Intelligence::choose_best_move(int time, BoardPlayer player) {
    // TODO: Dismiss time for now, just go on for a few rounds.
    return choose_best_move(state_, 10, player);
}

BoardMove Intelligence::choose_best_move(BoardState * state, int depth, BoardPlayer player) {
    BoardMove * moves = new BoardMove[MOVE_COUNT];
    int move_count = state->available_moves(player, moves, MOVE_COUNT);

    BoardMove bestMove;
    int bestScore = -9001; // ITS OVER 9000!!!!!!!!!!!11111one1111

    for (int i = 0; i < move_count; i++) {
        BoardState * innerState;
        BoardMove innerMove;

        if (depth > 0) {
            innerState = new BoardState(state->with_move_applied(moves[i], player));
            innerMove = choose_best_move(innerState, --depth, OPPONENT(player));
        }
        else {
            innerState = state;
            innerMove = moves[i];
        }

        int score = evaluate(new BoardState(innerState->with_move_applied(innerMove, player)), player);

        if (score > bestScore)
        {
            bestMove = innerMove;
            bestScore = score;
        }

        delete innerState;
    }

    return bestMove;
}

int Intelligence::evaluate(BoardState * state, BoardPlayer player)
{
    return 1;
}