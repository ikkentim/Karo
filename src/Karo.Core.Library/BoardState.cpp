#include "BoardState.h"
#include <cstring>
#include <assert.h>
#include <math.h>
#include <iostream>
#include <string>

#define BOARD_INITIAL_WIDTH     (5)

using namespace std;

BoardState::BoardState() {
    tiles_ = new BoardTile[TILE_COUNT];
    pieces_ = new BoardPiece[PIECE_COUNT];

    for (int i = 0; i < TILE_COUNT; i++)
        tiles_[i].piece = NULL;
    for (int i = 0; i < PIECE_COUNT; i++) {
        pieces_[i].tile = NULL;
        pieces_[i].player = PLAYER_NONE;
    }

#define SET_CONNECTION(a,b,d); {tiles_[a].neighbors[d] = &tiles_[b]; \
    tiles_[b].neighbors[DIRECTION_FLIP(d)] = &tiles_[a]; }

    // Fill tiles array with initial data
    for (int i = 0; i < TILE_COUNT; i++) {
        tiles_[i].position.x = i % BOARD_INITIAL_WIDTH;
        tiles_[i].position.y = i / BOARD_INITIAL_WIDTH;

        for (int d = 0; d < DIRECTION_COUNT; d++)
            tiles_[i].neighbors[d] = NULL;

        if (tiles_[i].position.x > 0) {
            SET_CONNECTION(i, i - 1, DIRECTION_WEST);
            if (tiles_[i].position.y > 0) {
                SET_CONNECTION(i, i - BOARD_INITIAL_WIDTH - 1, DIRECTION_NORTHWEST);
            }
        }
        if (tiles_[i].position.y > 0) {
            SET_CONNECTION(i, i - BOARD_INITIAL_WIDTH, DIRECTION_NORTH);
            if (tiles_[i].position.x < BOARD_INITIAL_WIDTH - 1) {
                SET_CONNECTION(i, i - BOARD_INITIAL_WIDTH + 1, DIRECTION_NORTHEAST);
            }
        }
    }

#undef SET_CONNECTION

    assert_ok();
}

BoardState::BoardState(const BoardState& other) {
    tiles_ = new BoardTile[TILE_COUNT];
    pieces_ = new BoardPiece[PIECE_COUNT];

    *this = other;
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

bool BoardState::tile(int x, int y, BoardTile ** result) {
    assert(tiles_);

    for (int i = 0; i < TILE_COUNT; i++)
        if (tiles_[i].position.x == x && tiles_[i].position.y == y) {
            if (result)
                *result = &tiles_[i];
            return true;
        }

    return false;
}

bool BoardState::piece(int x, int y, BoardPiece ** result) {
    assert(pieces_);

    for (int i = 0; i < PIECE_COUNT; i++)
        if (pieces_[i].tile != NULL &&
            pieces_[i].tile->position.x == x &&
            pieces_[i].tile->position.y == y &&
            pieces_[i].player != PLAYER_NONE)
        {
            if (result)
                *result = &pieces_[i];
            return true;
        }

    return false;
}

bool BoardState::piece_in_direction(BoardPiece * piece, int direction, int offset, BoardPiece **result) {
    if (!piece) {
        return false;
    }
    if (offset < 0) {
        offset = -offset;
        direction = DIRECTION_FLIP(direction);
    }

    BoardTile * tile = piece->tile;

    while (tile && offset > 0) {
        tile = tile->neighbors[direction];
        offset--;
    }

    if (!tile || !tile->piece) {
        return false;
    }

    if (result)
        *result = tile->piece;
    return true;
}

bool BoardState::is_finished() {
    return is_finished_;
}

int BoardState::piece_count() {
    return piece_count_;
}

int BoardState::available_moves(BoardPlayer player, BoardMove * moves, int count) {
    int idx = 0, corner_count;
    BoardTile ** corners = NULL;

    assert(moves);

    // If there are less than PIECE_COUNT placed pieces, we're still in the 
    // first phase. In this phase we can only place pieces in any free spot.
    if (piece_count() < PIECE_COUNT)
    {
        for (int tile_idx = 0; tile_idx < TILE_COUNT; tile_idx++) {
            if (!piece(tiles_[tile_idx].position.x, tiles_[tile_idx].position.y, NULL)) {

                // Add the move to the moves array.
                if (idx < count)
                    moves[idx] = BoardMove(tiles_[tile_idx].position);
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

        // TODO: Use tile neighbors
        // Iterate every move in every direction ( |, -, /, \ )
        for (int ox = -1; ox <= 1; ox++)
            for (int oy = -1; oy <= 1; oy++) {
                // If both offsets are 0, it's not a move.
                if (ox == 0 && oy == 0) {
                    continue;
                }

                int newx = pieces_[piece_idx].tile->position.x + ox,
                    newy = pieces_[piece_idx].tile->position.y + oy;

                // If the target position is blocked, attempt jumping over it.
                // If this position is also blocked, skip.
                if (piece(newx, newy, NULL)) {
                    newx += ox;
                    newy += oy;

                    if (piece(newx, newy, NULL))
                        continue;
                }

                BoardTile * target;

                // If there is a tile at the target, add the move.
                if (tile(newx, newy, &target)) {
                    if (idx < count)
                        moves[idx] = BoardMove(target->position, &pieces_[piece_idx]);
                    idx++;

                    continue;
                }

                // TODO: Use tile neighbors
                // To place a tile here, we need at least one connecting tile.
                if (!tile(newx - 1, newy, NULL) &&
                    !tile(newx + 1, newy, NULL) &&
                    !tile(newx, newy - 1, NULL) &&
                    !tile(newx, newy + 1, NULL))
                    continue;

                // We need to move a tile to this position, fill the corners
                // array if empty.
                if (!corners) {
                    corners = new BoardTile *[TILE_COUNT];
                    corner_count = corner_tiles(corners, TILE_COUNT);
                }

                // For every corner add a move of moving this corner to the
                // target position.
                for (int corner_idx = 0; corner_idx < corner_count;
                    corner_idx++) {
                    assert(corners[corner_idx]);

                    if (corners[corner_idx]->is_neighbor(BoardPosition(newx, newy))) {
                        continue;
                    }

                    if (idx < count)
                        moves[idx] = BoardMove(BoardPosition(newx, newy),
                        &pieces_[piece_idx], corners[corner_idx]);
                    idx++;
                }
            }
    }

    if (corners) {
        delete[] corners;
        corners = NULL;
    }

    assert(idx <= count);

#if defined _DEBUG

    for (int i = 0; i < idx; i++) {
        if (!moves[i].piece) {
            assert(moves[i].piece && "Move generated without origin");
        }
    }
#endif
    return idx;
}

bool BoardState::is_valid_move(BoardMove move) {
    //Step0: Initphase //Place piece on current location, just check if there is a piece already
    if (piece_count() < PIECE_COUNT)
        return !piece(move.target.x, move.target.y, NULL);

    //Todo:: Check for tiles, clean code (make more functions oid)
    //Make testcases for it

    //Check for tileplacement
    if (!is_valid_tile_placement(move.target.x, move.target.y, move.tile->position.x, move.tile->position.y))
        return false;

    //Move Phase
    //Step1: Check if new location is different from the current one
    if (move.target.x == move.piece->tile->position.x && move.target.y == move.piece->tile->position.y)
        return false;

    // TODO: Use tile neighbors
    //Check if new location has any neighbors (otherwise you get invalid boardstate)
    if (!(tile(move.target.x + 1, move.target.y, NULL) || tile(move.target.x - 1, move.target.y, NULL) ||
        tile(move.target.x, move.target.y + 1, NULL) || tile(move.target.x, move.target.y - 1, NULL)))
        return false;

    //Step2: Check if there already is a piece on the tile your trying to move to
    for (int i = 0; i < TILE_COUNT; i++) { //for each (RefTile^ tile in Tiles){
        if (piece(move.target.x, move.target.y, NULL))
            return false;

        //Step3: Check if there is a piece between your current location, and the new one
        int pieceLocationX, pieceLocationY, distancex = 0, distancey = 0;
        //If the X location won't change, keep the X location. otherwise add distance
        if (move.target.x == move.piece->tile->position.x)
            pieceLocationX = move.target.x;
        else {
            distancex = move.target.x - move.piece->tile->position.x;
            pieceLocationX = move.piece->tile->position.x + distancex / 2;
            distancex = abs(distancex);
        }
        if (move.target.y == move.piece->tile->position.y)
            pieceLocationY = move.target.y;
        else {
            distancey = move.target.y - move.piece->tile->position.y;
            pieceLocationY = move.piece->tile->position.y + distancey / 2;
            distancey = abs(distancey);
        }

        //I have no information about the tile you want to move here -_-;
        //Cant take certain tiles when jumping 
        //int tileX = move.target.x;
        //int tileY = move.target.y;

        //if (tile(tileX + 1, tileY,NULL))
        //	tileX + 1;
        //if (tile(tileX - 1, tileY, NULL))
        //	tileX - 1;
        //if (tile(tileX, tileY + 1, NULL))
        //	tileY + 1;
        //if (tile(tileX, tileY -1, NULL))
        //	tileY - 1;

        //Can't jump further then 2 spaces | can't jump (2,1) (like the chess horse :P) | can only jump over enemy
        BoardPiece * jumped_piece;
        if (piece(pieceLocationX, pieceLocationY, &jumped_piece) &&
            jumped_piece != move.piece &&
            distancex < 3 && distancey < 3 && distancex + distancey != 3)
            return true;

        if (distancex > 1 || distancey > 1)
            return false;
    }

    return true;
}

bool BoardState::is_valid_tile_placement(int x, int y, int tx, int ty) {
    //Check if the tile used to move, doesnt make it invalid
    //first check if the move has only 1 neighbor
    int neightbor = 0;
    int tilex, tiley;
    // TODO: Use tile neighbors
    if (tile(x - 1, y, NULL)){
        neightbor++;
        tilex = x - 1;
        tiley = y;
    }
    if (tile(x + 1, y, NULL)){
        neightbor++;
        tilex = x + 1;
        tiley = y;
    }
    if (tile(x, y - 1, NULL)){
        neightbor++;
        tilex = x;
        tiley = y - 1;
    }
    if (tile(x, y + 1, NULL)){
        neightbor++;
        tilex = x;
        tiley = y + 1;
    }
    //Check if the tile used to move, is not the only neighbor
    if (neightbor == 1 && tilex == tx && tiley == ty || neightbor == 0)
        return false;
    
    return true;
}

BoardMove BoardState::create_move(BoardPosition target, BoardPosition piecePos,
    BoardPosition tilePos) {
    BoardMove m;
    m.target = target;
    m.piece_position = piecePos; // For undo
    m.tile_position = tilePos; // For undo
    m.piece = NULL;
    m.tile = NULL;

    if (piece_count() == PIECE_COUNT) {
        piece(piecePos.x, piecePos.y, &m.piece);
        assert(m.piece);
        if (!tile(target.x, target.y, NULL)) {
            tile(tilePos.x, tilePos.y, &m.tile);
            assert(m.tile);
        }
    }

    return m;
}

void BoardState::update_neighbors(BoardPosition newPos, BoardTile * t) {
   for (int d = 0; d < DIRECTION_COUNT; d++)
        t->neighbors[d] = NULL;

    for (int i = 0; i < TILE_COUNT; i++) {
        if (tiles_ + i == t)
            continue;

        for (int d = 0; d < DIRECTION_COUNT; d++)
        {
            if (tiles_[i].neighbors[d] == t)
                tiles_[i].neighbors[d] = NULL;

            if (tiles_[i].position.x + DIRECTION_OFFSET_X(d) == newPos.x &&
                tiles_[i].position.y + DIRECTION_OFFSET_Y(d) == newPos.y) {
                tiles_[i].neighbors[d] = t;
                t->neighbors[DIRECTION_FLIP(d)] = &tiles_[i];
            }
        }
    }
}

void BoardState::apply_move(BoardMove move, BoardPlayer player) {
    BoardTile * newTile = NULL;

    assert(player);
    assert(!piece(move.target.x, move.target.y, NULL));

    // If in initial phase, place the piece at the target position.
    if (piece_count_ < PIECE_COUNT)
    {
        if (piece_count_<0)
        cout << "place init pc at " << piece_count_ << endl;
        assert(!move.piece);
        assert(!move.tile);

        // Set the occupied tile to the piece.
        bool ok_place_pos =
            tile(move.target.x, move.target.y, &pieces_[piece_count_].tile);

        assert(ok_place_pos);

        // Set the owner of this piece.
        pieces_[piece_count_].player = player;

        // Set the piece occupying this tile.
        pieces_[piece_count_].tile->piece = &pieces_[piece_count_];

        piece_count_++;
        return;
    }

    assert(move.piece);
    assert(move.piece->player);
    assert(move.piece->tile);
    assert(move.piece == move.piece->tile->piece);

    // Move the tile if needed.
    if (!tile(move.target.x, move.target.y, &newTile)) {
        assert(move.tile &&
            "no tile specified while no tile at target position");

        assert(!move.tile->piece || move.tile->piece->tile == move.tile);
        assert(!move.tile->piece && "moving tile is occupied");

        // Update the tile position, update the neighbors
        BoardPosition oldTilePosition = move.tile->position;
        move.tile->position = move.target;
        update_neighbors(move.target, move.tile);

        // Store this tile as the target tile
        newTile = move.tile;
    }
    else {
        assert(!move.tile);
    }

    // Flip the tile if jumping.
    if (abs(move.piece->tile->position.x - move.target.x) > 1 ||
        abs(move.piece->tile->position.y - move.target.y) > 1)
        move.piece->is_face_up = !move.piece->is_face_up;

    // Move the piece to the specified target.
    move.piece->tile->piece = NULL;
    move.piece->tile = newTile;
    newTile->piece = move.piece;

    calc_is_finished();

    //Do some debugging checks
    assert_ok();
}

void BoardState::undo_move(BoardMove move, BoardPlayer player) {

    // If no piece was moves, this must be the first phase.
    if (!move.piece) {
        BoardPiece * removePiece = NULL;
        bool remove_ok = piece(move.target.x, move.target.y, &removePiece);

        assert(remove_ok);
        assert(removePiece->tile);
        assert(!removePiece->is_face_up);

        removePiece->tile->piece = NULL;
        removePiece->tile = NULL;
        removePiece->player = PLAYER_NONE;

        piece_count_--;
        return;
    }

    // If a tile was moved, move it back.
    if (move.tile) {
        assert(!tile(move.tile_position.x, move.tile_position.y, NULL));

        update_neighbors(move.tile_position, move.tile);
        move.tile->position = move.tile_position;
    }

    assert(!piece(move.piece_position.x, move.piece_position.y, NULL));

    // Flip the tile back if jumped.
    if (abs(move.piece_position.x - move.target.x) > 1 ||
        abs(move.piece_position.y - move.target.y) > 1)
        move.piece->is_face_up = !move.piece->is_face_up;

    assert(move.piece->tile);

    // Move the piece back to it's old tile.
    move.piece->tile->piece = NULL;
    bool undo_ok = tile(move.piece_position.x, move.piece_position.y,
        &move.piece->tile);
    move.piece->tile->piece = move.piece;

    assert(undo_ok);

    calc_is_finished();

    assert_ok();
}

int BoardState::corner_tiles(BoardTile ** tiles, int count) {
    int idx = 0;

    // TODO: Calculate every time after a tile has moved which tiles are corner-
    // tiles. Store this to the BoardTile structure. Should speed things by a lot.

    for (int i = 0; i < TILE_COUNT; i++) {
        // If the tile has at least 2 disconnected edges it is a corner. 
        // Must also be unoccupied.
        if (((!tiles_[i].neighbors[DIRECTION_NORTH] && 
            !tiles_[i].neighbors[DIRECTION_WEST]) ||
            (!tiles_[i].neighbors[DIRECTION_NORTH] && 
            !tiles_[i].neighbors[DIRECTION_EAST]) ||
            (!tiles_[i].neighbors[DIRECTION_SOUTH] && 
            !tiles_[i].neighbors[DIRECTION_WEST]) ||
            (!tiles_[i].neighbors[DIRECTION_SOUTH] && 
            !tiles_[i].neighbors[DIRECTION_EAST])) &&
            !tiles_[i].piece) {
            if (idx < count)
                tiles[idx] = &tiles_[i];
            idx++;
        }
    }
    return idx;
}

BoardPlayer BoardState::winner() {
    if (piece_count() < PIECE_COUNT) return PLAYER_NONE;

    for (int i = 0; i < PIECE_COUNT; i++)
    {
        if (pieces_[i].is_face_up &&
            is_row_for_player(&pieces_[i], pieces_[i].player))

            return pieces_[i].player;

    }
    return PLAYER_NONE;
}

int BoardState::row_length(BoardPiece * piece, int direction, BoardPlayer player) {
    if (!piece || !piece->is_face_up || piece->player != player)
        return 0;

    assert(piece->tile);

    BoardTile * next = piece->tile->neighbors[direction];

    return !next || !next->piece || !next->piece->is_face_up || 
        next->piece->player != player 
        ? 0 
        : 1 + row_length(next->piece, direction, player);

}

bool BoardState::is_row_for_player(BoardPiece * piece, BoardPlayer player) {
    if (!piece || !piece->is_face_up || piece->player != player)
        return false;

    for (int d = 0; d < DIRECTION_COUNT / 2; d++)
        if (row_length(piece, d, player) + 1 + 
            row_length(piece, DIRECTION_FLIP(d), player) >= 4) {
            return true;
        }

    return false;
}

void BoardState::calc_is_finished() {
    assert(pieces_);

    for (int i = 0; i < PIECE_COUNT; i++) {
        // If the piece has no set player yet, this indicates that the game is
        // still in the first phase and cannot have finished.
        if (pieces_[i].player == PLAYER_NONE) {
            is_finished_ = false;
            return;
        }

        assert(pieces_[i].tile);

        // Look for a row of 4 or more face up pieces of the same player.
        if (!pieces_[i].is_face_up) {
            continue;
        }

        if (is_row_for_player(&pieces_[i], pieces_[i].player))
            is_finished_ = true;
            return;
    }

    is_finished_ = false;
}

void BoardState::assert_ok() {
#if defined _DEBUG
#define DEBUG_TILE(xo, yo, dir) { if (tile(tiles_[i].position.x + xo, tiles_[i].position.y + yo, &dbgTile)) { \
            assert(dbgTile == tiles_[i].neighbors[dir]); } }

    BoardTile *dbgTile = NULL;
    for (int i = 0; i < TILE_COUNT; i++) {
        DEBUG_TILE(0, -1, DIRECTION_NORTH);
        DEBUG_TILE(1, -1, DIRECTION_NORTHEAST);
        DEBUG_TILE(1, 0, DIRECTION_EAST);
        DEBUG_TILE(1, 1, DIRECTION_SOUTHEAST);
        DEBUG_TILE(0, 1, DIRECTION_SOUTH);
        DEBUG_TILE(-1, 1, DIRECTION_SOUTHWEST);
        DEBUG_TILE(-1, 0, DIRECTION_WEST);
        DEBUG_TILE(-1, -1, DIRECTION_NORTHWEST);
    }
#undef DEBUG_TILE
#endif
}
