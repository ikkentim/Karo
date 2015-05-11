#pragma once

#include "BoardTile.h"
#include "BoardPiece.h"

struct BoardMove {
    // When moving a tile, contains data of where the tile is coming from.
    BoardTile tile;
    // Contains data when the piece is moving to.
    BoardTile target;
    // When moving a piece, contains where the piece is coming from
    BoardPiece piece;

    // Constructor
    BoardMove() { }

    // Constructor for placing a piece
    BoardMove(BoardTile t)
        : target(t) { }

    // Constructor for moving a piece
    BoardMove(BoardTile t, BoardPiece p)
        : target(t), piece(p) { }

    // Constructor for moving a piece and tile
    BoardMove(BoardTile t, BoardPiece p, BoardTile o)
        : target(t), piece(p), tile(o) { }
};