// BoardState.h

#pragma once

#include "BoardTile.h"
#include "BoardPiece.h"
#include "BoardMove.h"

#define TILE_COUNT      (20)
#define PIECE_COUNT     (12)
#define MOVE_COUNT     (PIECE_COUNT * (8 + TILE_COUNT / 4))

class BoardState {
public:
    BoardState();
    BoardState(BoardTile * tiles, BoardPiece * pieces);
    ~BoardState();

    BoardTile * tiles() const;
    BoardPiece * pieces() const;
    bool is_finished();
    int piece_count();
    int available_moves(BoardPlayer player, BoardMove * moves, int count);
    bool is_valid_move(BoardMove move);
    BoardState apply_move(BoardMove move, BoardPlayer player);
    BoardState undo_move(BoardMove move, BoardPlayer player);

    bool tile(int x, int y, BoardTile *result);
    bool piece(int x, int y, BoardPiece *result);
    int corner_tiles(BoardTile * tiles, int count);

    BoardPlayer winner();
    int row_length(int x, int y, int ox, int oy, BoardPlayer player);

private:
    BoardTile * tiles_;
    BoardPiece * pieces_;

    bool is_corner_tile(int x, int y);
    bool is_valid_tile_placement(int x, int y);
    bool is_row_for_player(int x, int y, BoardPlayer player);
};