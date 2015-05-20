// BoardState.h

#pragma once

#include "BoardTile.h"
#include "BoardPiece.h"
#include "BoardMove.h"

#define TILE_COUNT      (20)
#define PIECE_COUNT     (12)
#define MOVE_COUNT     (PIECE_COUNT * (8 + TILE_COUNT))

class BoardState {
public:
    BoardState();
    BoardState(const BoardState& other);
    BoardState(BoardTile * tiles, BoardPiece * pieces);
    ~BoardState();
    BoardTile * tiles() const;
    BoardPiece * pieces() const;
    bool tile(int x, int y, BoardTile **result);
    bool piece(int x, int y, BoardPiece **result);
    bool is_finished();
    int piece_count();   
    BoardMove create_move(BoardPosition target, BoardPosition piece,
        BoardPosition tile);
    int available_moves(BoardPlayer player, BoardMove * moves, int count);
    void apply_move(BoardMove move, BoardPlayer player);
    void undo_move(BoardMove move, BoardPlayer player);
    bool is_valid_move(BoardMove move);
    int corner_tiles(BoardTile ** tiles, int count);
    BoardPlayer winner();
    int row_length(int x, int y, int ox, int oy, BoardPlayer player);
private:
    BoardTile * tiles_;
    BoardPiece * pieces_;
    void update_neighbors(BoardPosition newPos, BoardTile * tile);
    bool is_valid_tile_placement(int x, int y, int tx, int ty);
    bool is_row_for_player(int x, int y, BoardPlayer player);
    void assert_ok();
};