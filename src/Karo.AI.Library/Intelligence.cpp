#include "Intelligence.h"
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <limits>

// MiniMax / AlphaBeta
// -------------------
#define MAX_DEPTH                                       (3)

// Evaluation
// ----------
#define SCORE_WIN                                       (10000000) // Score given to a winning player.
#define ZOBRIST_TYPES									(5) //amount of different 
// Score based on distance between own pieces
#define SCORE_DISTANCE_SELF                             (4) // Max distance between own pieces to give addtional.
#define SCORE_DISTANCE_SELF_BASE                        (1) // Score given to pieces less than SCORE_DISTANCE_SELF tiles away.
#define SCORE_DISTANCE_SELF_MULTIPLIER                  (1) // Multiplier of (SCORE_DISTANCE_SELF-distance) to add to the score.
#define SCORE_DISTANCE_SELF_FACE_MULTIPLIER             (1) // Multiplier of score if the piece is face-up.
#define SCORE_DISTANCE_SELF_FACE_OTHER_MULTIPLIER       (1) // Multiplier of score if the other piece is face-up.

// Score based on distance between own and opponents pieces
#define SCORE_DISTANCE_OPPONENT                         (2) // Max distance between own and opponents pieces to give addtional
#define SCORE_DISTANCE_OPPONENT_BASE                    (1) // Score given to pieces less than SCORE_DISTANCE_SELF tiles away.
#define SCORE_DISTANCE_OPPONENT_FACE_MULTIPLIER         (-1) // Multiplier of (SCORE_DISTANCE_SELF-distance) to add to the score.
#define SCORE_DISTANCE_OPPONENT_FACE_OTHER_MULTIPLIER   (1) // Multiplier of score if the piece is face-up.
#define SCORE_DISTANCE_OPPONENT_MULTIPLIER              (1) // Multiplier of score if the other piece is face-up.

// Score based on the length of a line of face-up pieces
#define SCORE_LINE_MULTIPLIER                           (8) // Multiplier of score given to a line of face-up pieces.

using namespace std;

Intelligence::Intelligence() {
	state_ = new BoardState();
	//currently quite dirty, it can result in doubles, will add checks later
	for (int x = 0; x < (TILE_COUNT * TILE_COUNT); x++) {
		for (int y = 0; y < ZOBRIST_TYPES; y++) {
			zobrist_randoms[x][y] = rand() % 1000000;
		}
	}
}

Intelligence::~Intelligence() {

    delete state_;
}

void Intelligence::apply_move(BoardMove move, BoardPlayer player) {
    state_->apply_move(move, player);
}

BoardMove Intelligence::choose_best_move(int time, BoardPlayer player) {
	BoardMove moves[MOVE_COUNT];
	BoardMove bestMove;
    int depth = MAX_DEPTH;
    int alpha = numeric_limits<int>::min();
    int beta = numeric_limits<int>::max();

    // Reset debug counters.
    iteration_count = 0;
    prune_count = 0;

    // Calculate depth.
    int piece_count = state_->piece_count();

    // If in initual phase, do some extra moves.
    if (piece_count < PIECE_COUNT) {
        depth = max(PIECE_COUNT - piece_count + MAX_DEPTH / 2, depth);
    }
    
	int move_count = state_->available_moves(player, moves, MOVE_COUNT);
	for (int i = 0; i < move_count; i++)
    {
        state_->apply_move(moves[i], player);
        int newScore = alpha_beta(MAX_DEPTH, alpha, beta, OPPONENT(player));
        state_->undo_move(moves[i], player);

        if (newScore > alpha)
		{
			bestMove = moves[i];
            alpha = newScore;
		}
	}

    assert(alpha != numeric_limits<int>::min());

	cout << "iterations: " << iteration_count << endl;
    cout << "prunes: " << prune_count << endl;
    cout << "best score: " << alpha << endl;

	return bestMove;
}

int Intelligence::alpha_beta(int depth, int alpha, int beta, BoardPlayer player)
{
	iteration_count++;

	if (depth == 0 || state_->is_finished())
	{
#if defined _DEBUG
        if (evaluate(player) != -evaluate(OPPONENT(player))) {
            cout << "Evalulate " << player << ": " << evaluate(player) << ", evaluate " << OPPONENT(player) << ": " << evaluate(OPPONENT(player)) << endl;
            //assert(evaluate(player) == -evaluate(OPPONENT(player)));
        }
#endif

        // WAS: return evaluate(player);

		int boardhash = zobrist_hash(PLAYER_PLAYER1);

		if (&trans_table->at(boardhash) != nullptr) {
			return trans_table->at(boardhash);
		}

		int eval = evaluate(PLAYER_PLAYER1);
		trans_table->emplace(boardhash, eval);
		return eval;
	}
	
    // Maximizer node.
	if (player == PLAYER_PLAYER1)
	{
        int value = numeric_limits<int>::min();

		BoardMove moves[MOVE_COUNT];
		int move_count = state_->available_moves(player, moves, MOVE_COUNT);
		
		for (int i = 0; i < move_count; i++)
        {
            state_->apply_move(moves[i], player);
            value = max(value, alpha_beta(depth - 1, alpha, beta, OPPONENT(player)));
            state_->undo_move(moves[i], player);

            alpha = max(alpha, value);

            if (value > beta)
			{
				prune_count++;
				break;
			}
		}

		return value;
	}

    // Minimizer node.
	else
	{
        int value = numeric_limits<int>::max();

		BoardMove moves[MOVE_COUNT];
		int move_count = state_->available_moves(player, moves, MOVE_COUNT);

		for (int i = 0; i < move_count; i++)
        {
            state_->apply_move(moves[i], player);
            value = min(value, alpha_beta(depth - 1, alpha, beta, OPPONENT(player)));

            state_->undo_move(moves[i], player);

            beta = min(beta, value);

			if (value < alpha)
			{
				prune_count++;
				break;
			}
		}

		return value;
	}
}

int Intelligence::evaluate(BoardPlayer player) {
    BoardPiece* allPieces = state_->pieces();
    int score = 0;
    int pieceCount = state_->piece_count();

    for (int i = 0; i < pieceCount; i++) {
        // Rate distance to own and opponents pieces.
        for (int j = 0; j < pieceCount; j++) {
            // Calculate the distance between the two pieces.
            int distance = abs(allPieces[i].tile->position.x - allPieces[j].tile->position.x) +
                abs(allPieces[i].tile->position.y - allPieces[j].tile->position.y);

            // Other piece is own piece.
            if (allPieces[i].player == allPieces[j].player) {
                if (allPieces[i].player == allPieces[j].player &&
                    distance < SCORE_DISTANCE_SELF) {
                    // If the other piece is face up, double the score.
                    score += SCORE_DISTANCE_SELF_BASE +
                        SCORE_DISTANCE_SELF_MULTIPLIER *
                        (allPieces[i].is_face_up ? SCORE_DISTANCE_SELF_FACE_MULTIPLIER : 1) * 
                        (allPieces[j].is_face_up ? SCORE_DISTANCE_SELF_FACE_OTHER_MULTIPLIER : 1) *
                        (allPieces[i].player == player
                        ? SCORE_DISTANCE_SELF - distance
                        : -SCORE_DISTANCE_SELF + distance);
                }
            }
            // Other piece is opponents piece.
            else {
                if (allPieces[i].player == allPieces[j].player &&
                    distance < SCORE_DISTANCE_OPPONENT) {
                    // If the opponents piece is face up, double the score.
                    score += SCORE_DISTANCE_OPPONENT_BASE +
                        SCORE_DISTANCE_OPPONENT_MULTIPLIER *
                        (allPieces[i].is_face_up ? SCORE_DISTANCE_OPPONENT_FACE_MULTIPLIER : 1) *
                        (allPieces[j].is_face_up ? SCORE_DISTANCE_OPPONENT_FACE_OTHER_MULTIPLIER : 1) *
                        (allPieces[i].player == player
                        ? SCORE_DISTANCE_OPPONENT - distance
                        : -SCORE_DISTANCE_OPPONENT + distance);
                }
            }
        }

        // Score face-up pieces with additional points.
		if (!allPieces[i].is_face_up)
			continue;

		if (allPieces[i].player == player)
            score += piece_score(&allPieces[i]) * SCORE_LINE_MULTIPLIER;
		else
            score -= piece_score(&allPieces[i]) * SCORE_LINE_MULTIPLIER;
	}

    return score;
}

int Intelligence::zobrist_hash(BoardPlayer player) {
	int minxpos = 99;
	int minypos = 99;
	BoardTile* tiles = state_->tiles();
	BoardPiece* pieces = state_->pieces();

	BoardTile newtiles[TILE_COUNT];
	BoardPiece newpieces[PIECE_COUNT];

	for (int i = 0; i < TILE_COUNT; i++) {
		BoardPosition p = tiles[i].position;
		if (p.x < minxpos)
			minxpos = p.x;
		if (p.y < minypos)
			minypos = p.y;
	}
	
	for (int i = 0; i < TILE_COUNT; i++) {
		newtiles[i] = tiles[i];

		newtiles[i].position.x = newtiles[i].position.x + minxpos;
		newtiles[i].position.y = newtiles[i].position.y + minypos;
	}

	for (int i = 0; i < PIECE_COUNT; i++) {
		newpieces[i] = pieces[i];

		newpieces[i].tile->position.x = newpieces[i].tile->position.x + minxpos;
		newpieces[i].tile->position.y = newpieces[i].tile->position.y + minypos;
	}

	int hash = 0;

	for (int i = 0; i < TILE_COUNT; i++) {
		int randvalposition = (newtiles[i].position.x * TILE_COUNT) + newtiles[i].position.y;
		int randvaltype = 4;
		hash ^= zobrist_randoms[randvalposition][randvaltype];
	}
	for (int i = 0; i < PIECE_COUNT; i++) {
		int randvalposition = (newpieces[i].tile->position.x * TILE_COUNT) 
			+ newpieces[i].tile->position.y;
		int randvaltype = 0;
		if (newpieces[i].player == PLAYER_PLAYER1) {

			if (newpieces[1].is_face_up) { randvaltype = 0; }
			else { randvaltype = 1;}
		}
		else {

			if (newpieces[i].is_face_up) { randvaltype = 2;}
			else { randvaltype = 3; }
		}
		hash ^= zobrist_randoms[randvalposition][randvaltype];
	}
	return hash;
}

int Intelligence::piece_score(BoardPiece * piece){
	int score = 0;

    if (!piece->is_face_up) {
        return 0;
    }

    // Loop trough every direction combination
    for (int d = 0; d < DIRECTION_COUNT / 2; d++) {
        // Calculate the lenght of the line.
        int lenA = state_->row_length(piece, d, piece->player);
        int lenB = state_->row_length(piece, DIRECTION_FLIP(d), piece->player);

        // If row is over 4 long, this piece is worth max score.
        if ((score += lenA + 1 + lenB) >= 4) {
            return SCORE_WIN;
        }

        // Check if the edge of the line is blocked by an enemy piece or not.
        bool blockedA = state_->piece_in_direction(piece, d, lenA + 1, NULL);
        bool blockedB = state_->piece_in_direction(piece, DIRECTION_FLIP(d), lenA + 1, NULL);

        // If it's blocked we decrease the score.
        int hScore = score / 2;
        if (blockedA){
            score -= hScore;
        }
        if (blockedB){
            score -= hScore;
        }
    }

    return score;
}