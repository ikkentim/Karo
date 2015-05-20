#pragma once

#include <stddef.h>
#include "BoardPosition.h"
#include <assert.h>

struct BoardMove {
    // When moving a tile, contains data of where the tile is coming from.
    BoardTile * tile = NULL;
    // Contains data when the piece is moving to.
    BoardPosition target;
    // When moving a piece, contains where the piece is coming from
    BoardPiece * piece = NULL;

    BoardPosition tile_position;
    BoardPosition piece_position;

    // Constructor
    BoardMove() { }

    // Constructor for placing a piece
    BoardMove(BoardPosition t)
        : target(t), piece(NULL), tile(NULL) { }

    // Constructor for moving a piece
    BoardMove(BoardPosition t, BoardPiece * p)
        : target(t), piece(p), tile(NULL) {
        assert(p);
        piece_position = p->tile->position;
    }

    // Constructor for moving a piece and tile
    BoardMove(BoardPosition t, BoardPiece * p, BoardTile * o)
        : target(t), piece(p), tile(o) {
        assert(p);
        assert(o);
        piece_position = p->tile->position;
        tile_position = o->position;
    }
};