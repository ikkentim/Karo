#include "BoardState.h"
#include <cstring>
#include <assert.h>
#include <math.h>

#define BOARD_INITIAL_WIDTH     (5);

BoardState::BoardState() {
    tiles_ = new BoardTile[TILE_COUNT];
    pieces_ = new BoardPiece[PIECE_COUNT];

    for (int i = 0; i < TILE_COUNT; i++) {
        tiles_[i].x = i % BOARD_INITIAL_WIDTH;
        tiles_[i].y = i / BOARD_INITIAL_WIDTH;
    }
}

BoardState::BoardState(const BoardState& other) {
    tiles_ = new BoardTile[TILE_COUNT];
    pieces_ = new BoardPiece[PIECE_COUNT];

    memcpy(tiles_, other.tiles_, TILE_COUNT * sizeof(BoardTile));
    memcpy(pieces_, other.pieces_, PIECE_COUNT * sizeof(BoardPiece));
}

BoardState::BoardState(BoardTile * tiles, BoardPiece * pieces) {
    assert(tiles);
    assert(pieces);

    tiles_ = new BoardTile[TILE_COUNT];
    pieces_ = new BoardPiece[PIECE_COUNT];

    memcpy(tiles_, tiles, TILE_COUNT * sizeof(BoardTile));
    memcpy(pieces_, pieces, PIECE_COUNT * sizeof(BoardPiece));
}

BoardState:: ~BoardState() {
    delete[] tiles_;
    delete[] pieces_;
}

BoardTile * BoardState::tiles() const {
    return tiles_;
}

BoardPiece * BoardState::pieces() const {
    return pieces_;
}

bool BoardState::tile(int x, int y, BoardTile * result) {
    assert(tiles_);

    for (int i = 0; i < TILE_COUNT;i++)
        if (tiles_[i].x == x && tiles_[i].y == y)
        {
            
            if (result)
                *result = tiles_[i];
            return true;
        }

    return false;
}

bool BoardState::piece(int x, int y, BoardPiece * result) {
    assert(pieces_);

    for (int i = 0; i < PIECE_COUNT; i++)
        if (pieces_[i].tile.x == x && pieces_[i].tile.y == y && 
            pieces_[i].player != BoardPlayer::None)
        {
            if (result)
                *result = pieces_[i];
            return true;
        }

    return false;
}

bool BoardState::is_finished() {
    assert(pieces_);

    for (int i = 0; i < PIECE_COUNT;i++){

        // If the piece has no set player yet, this indicates that the game is
        // still in the first phase and cannot have finished.
        if (pieces_[i].player == BoardPlayer::None) {
            return false;
        }

        // Look for a row of 4 or more face up pieces of the same player.
        if (!pieces_[i].is_face_up) {
            continue;
        }

        int x = pieces_[i].tile.x;
        int y = pieces_[i].tile.y;

        // Check for rows in every direction ( |, -, /, \ )
        return 1 + row_length(x, y, -1, -1, pieces_[i].player) +
            row_length(x, y, 1, 1, pieces_[i].player) >= 4 ||
            1 + row_length(x, y, 1, -1, pieces_[i].player) +
            row_length(x, y, -1, 1, pieces_[i].player) >= 4 ||
            1 + row_length(x, y, -1, 0, pieces_[i].player) +
            row_length(x, y, 1, 0, pieces_[i].player) >= 4 ||
            1 + row_length(x, y, 0, -1, pieces_[i].player) +
            row_length(x, y, 0, 1, pieces_[i].player) >= 4;
    }

    return false;
}

int BoardState::piece_count() {
    int c = 0;

    assert(pieces_);

    // Count the number of pieces with a set player
    for (int i = 0; i < PIECE_COUNT; i++)
        if (pieces_[i].player != BoardPlayer::None)
            c++;

    return c;
}

int BoardState::available_moves(BoardPlayer player, BoardMove * moves, int count) {
    int idx = 0, corner_count;
    BoardTile * corners = NULL;

    assert(moves);

    // If there are less than PIECE_COUNT placed pieces, we're still in the 
    // first phase. In this phase we can only place pieces in any free spot.
    if (piece_count() < PIECE_COUNT)
    {
        for (int tile_idx = 0; tile_idx < TILE_COUNT; tile_idx++) {
            if (!piece(tiles_[tile_idx].x, tiles_[tile_idx].y, NULL)) {

                // Add the move to the moves array.
                if (idx < count)
                    moves[idx] = BoardMove(tiles_[tile_idx]);
                idx++;
            }
        }

        return idx;
    }

    // Loop trough every piece of the specified player and calculate the moves
    // it is able to perform.
    for (int piece_idx = 0; piece_idx < PIECE_COUNT; piece_idx++) {
        if (pieces_[piece_idx].player != player)
            continue;

        // Iterate every move in every direction ( |, -, /, \ )
        for (int ox = -1; ox <= 1; ox++)
            for (int oy = -1; oy <= 1; oy++) {
                // If both offsets are 0, it's not a move.
                if (ox == 0 && oy == 0) {
                    continue;
                }

                int newx = pieces_[piece_idx].tile.x + ox,
                    newy = pieces_[piece_idx].tile.y + oy;

                // If the target position is blocked, attempt jumping over it.
                // If this position is also blocked, skip.
                if (piece(newx, newy, NULL)) {
                    newx += ox;
                    newy += oy;

                    if (piece(newx, newy, NULL))
                        continue;
                }

                BoardTile target;

                // If there is a tile at the target, add the move.
                if (tile(newx, newy, &target)) {
                    if (idx < count)
                        moves[idx] = BoardMove(target, pieces_[piece_idx]);
                    idx++;

                    continue;
                }

                // To place a tile here, we need at least one connecting edge.
                if (!tile(newx - 1, newy, NULL) &&
                    !tile(newx + 1, newy, NULL) &&
                    !tile(newx, newy - 1, NULL) &&
                    !tile(newx, newy + 1, NULL))
                    continue;

                // We need to move a tile to this position, fill the corners
                // array if empty.
                if (!corners) {
                    corners = new BoardTile[20];
                    corner_count = corner_tiles(corners, 20);
                }

                // For every corner add a move of moving this corner to the
                // target position.
                for (int corner_idx = 0; corner_idx < corner_count;
                    corner_idx++) {
                    if (idx < count)
                        moves[idx] = BoardMove(BoardTile(newx, newy),
                        pieces_[piece_idx], corners[corner_idx]);
                    idx++;
                }
            }
    }

    if (corners) {
        delete[] corners;
        corners = NULL;
    }

    return idx;
}

bool BoardState::is_valid_move(BoardMove move) {
    //Step0: Initphase //Place piece on current location, just check if there is a piece already
    if (piece_count() < PIECE_COUNT)
    {
        return !piece(move.target.x, move.target.y, NULL);
    }

    //Move Phase
    //Step1: Check if new location is different from the current one
    if (move.target.x == move.piece.tile.x && move.target.y == move.piece.tile.y)
        return false;

    //Step2: Check if there already is a piece on the tile your trying to move to
    for (int i = 0; i < TILE_COUNT; i++) { //for each (RefTile^ tile in Tiles){
        if (piece(move.target.x, move.target.y, NULL))
            return false;

        //Step3: Check if there is a piece between your current location, and the new one
        int pieceLocationX, pieceLocationY, distancex, distancey;
        //If the X location won't change, keep the X location. otherwise add distance
        if (move.target.x == move.piece.tile.x)
            pieceLocationX = move.target.x;
        else {
            distancex = move.target.x - move.piece.tile.x;
            pieceLocationX = move.piece.tile.x + distancex / 2;
            distancex = abs(distancex);
        }
        if (move.target.y == move.piece.tile.y)
            pieceLocationY = move.target.y;
        else {
            distancey = move.target.y - move.piece.tile.y;
            pieceLocationY = move.piece.tile.y + distancey / 2;
            distancey = abs(distancey);
        }

        //Can't jump further then 2 spaces | can't jump (2,1) (like the chess horse :P) | can only jump over enemy
        BoardPiece jumped_piece;
        if (piece(pieceLocationX, pieceLocationY, &jumped_piece) &&
            jumped_piece != move.piece &&
            distancex < 3 && distancey < 3 && distancex + distancey != 3)
            return true;

        if (distancex > 1 || distancey > 1)
            return false;
    }
    return true;
}

BoardState BoardState::with_move_applied(BoardMove move, BoardPlayer player) {
    BoardState state = *this;

    // If in initial phase, simply add the piece to the pieces array.
    int count = piece_count();
    if (count < PIECE_COUNT) {
        state.pieces_[count] = BoardPiece(move.target, player);
        return state;
    }

    // If there is no tile present, we are moving an existing tile.
    if(!tile(move.target.x, move.target.y, NULL)) {
        for (int i = 0; i < TILE_COUNT; i++)
            if (state.tiles_[i] == move.tile) {
                state.tiles_[i] = move.target;

                for (int j = 0; j < PIECE_COUNT; j++)
                    if (state.pieces_[j] == move.piece) {
                        state.pieces_[j].tile = move.target;

                        return state;
                    }

                // No piece was found, should never happen.
                assert(0 && "piece was moved but does not exist!(1)");
                return state; // return error state.
            }

        // No tile was found, should never happen.
        assert(0 && "tile was moved but does not exist!");
        return state; // return error state.
    }

    // Simply move the piece. 
    for (int j = 0; j < PIECE_COUNT; j++)
        if (state.pieces_[j].tile == move.piece.tile) {
            state.pieces_[j].tile = move.target;

            return state;
        }

    // No piece was found, should never happen.
    assert(0 && "piece was moved but does not exist!(2)");
    return state; // return error state.
}

int BoardState::corner_tiles(BoardTile * tiles, int count) {
    int idx = 0;

    for (int i = 0; i < TILE_COUNT;i++) {
        // If the tile has at least 2 adjacent disconnected edges it is a corner.
        bool top = tile(tiles_[i].x, tiles_[i].y - 1, NULL);
        bool bottom = tile(tiles_[i].x, tiles_[i].y + 1, NULL);
        bool left = tile(tiles_[i].x - 1, tiles_[i].y, NULL);
        bool right = tile(tiles_[i].x + 1, tiles_[i].y, NULL);

        if (((top == false && left == false) ||
            (top == false && right == false) ||
            (bottom == false && left == false) ||
            (bottom == false && right == false)) && 
            !piece(tiles_[i].x, tiles_[i].y, NULL)) {
            if (idx < count)
                tiles[idx] = tiles_[i];
            idx++;
        }
    }

    return idx;
}

BoardPlayer BoardState::winner() {
    if (piece_count() < PIECE_COUNT) return BoardPlayer::None;

    for (int i = 0; i < PIECE_COUNT; i++)
    {
        if (pieces_[i].is_face_up && is_row_for_player(pieces_[i].tile.x, pieces_[i].tile.y, BoardPlayer::Player1))
            return BoardPlayer::Player1;
        if (pieces_[i].is_face_up && is_row_for_player(pieces_[i].tile.x, pieces_[i].tile.y, BoardPlayer::Player2))
            return BoardPlayer::Player2;
    }
}

int BoardState::row_length(int x, int y, int ox, int oy, BoardPlayer player) {
    x += ox;
    y += oy;

    BoardPiece p;
    if (!piece(x, y, &p)) return 0;

    if (!p.is_face_up || p.player != player) return 0;

    return 1 + row_length(x, y, ox, oy, player);
}

bool BoardState::is_row_for_player(int x, int y, BoardPlayer player) {
    int a1 = row_length(x, y, -1, -1, player) + 1 + row_length(x, y, 1, 1, player);
    int a2 = row_length(x, y, -1, 1, player) + 1 + row_length(x, y, 1, -1, player);
    int a3 = row_length(x, y, 0, -1, player) + 1 + row_length(x, y, 0, 1, player);
    int a4 = row_length(x, y, -1, 0, player) + 1 + row_length(x, y, 1, 0, player);

    return a1 >= 4 || a2 >= 4 || a3 >= 4 || a4 >= 4;
}

BoardState& BoardState::operator=(const BoardState& other) {
    memcpy(tiles_, other.tiles_, TILE_COUNT * sizeof(BoardTile));
    memcpy(pieces_, other.pieces_, PIECE_COUNT * sizeof(BoardPiece));
    return *this;
}