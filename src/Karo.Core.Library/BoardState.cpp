#include "BoardState.h"
#include <cstring>
#include <assert.h>
#include <math.h>
#include <iostream>

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

#define SET_CONNECTION(a,b,d) {tiles_[a].neighbors[d] = &tiles_[b]; \
    tiles_[b].neighbors[DIRECTION_FLIP(d)] = &tiles_[a]; }

    // Fill tiles array with initial data
    for (int i = 0; i < TILE_COUNT; i++) {
        tiles_[i].position.x = i % BOARD_INITIAL_WIDTH;
        tiles_[i].position.y = i / BOARD_INITIAL_WIDTH;

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

    for (int i = 0; i < TILE_COUNT;i++)
        if (tiles_[i].position.x == x && tiles_[i].position.y == y)
        {
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

bool BoardState::is_finished() {
    assert(pieces_);
    /*
    for (int i = 0; i < PIECE_COUNT;i++){

        // If the piece has no set player yet, this indicates that the game is
        // still in the first phase and cannot have finished.
        if (pieces_[i].player == PLAYER_NONE) {
            return false;
        }

        // Look for a row of 4 or more face up pieces of the same player.
        if (!pieces_[i].is_face_up) {
            continue;
        }

        int x = pieces_[i].tile->position.x;
        int y = pieces_[i].tile->position.y;

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
    */
    return false;
}

int BoardState::piece_count() {
    int c = 0;

    assert(pieces_);
    
    // Count the number of pieces with a set player
    for (int i = 0; i < PIECE_COUNT; i++)
        if (pieces_[i].player != PLAYER_NONE)
            c++;
    
    return c;
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

    if (idx > count) {
        cout << idx << " > " << count << endl;
    }
    assert(idx <= count);
#if defined _DEBUG

    for (int i = 0; i < idx; i++) {
        if (!moves[i].piece) {
            cout << idx << "moves found. move " << i << ". p/t 0x" << moves[i].piece << "/0x" << moves[i].tile << endl;
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
    if (!is_valid_tile_placement(move.tile->position.x, move.tile->position.y))
         return false;
  
    //Move Phase
    //Step1: Check if new location is different from the current one
    if (move.target.x == move.piece->tile->position.x && move.target.y == move.piece->tile->position.y)
        return false;

	//Check if new location has any neighbors (otherwise you get invalid boardstate)
	if (!(tile(move.target.x+1, move.target.y, NULL) || tile(move.target.x-1, move.target.y, NULL) ||
		tile(move.target.x, move.target.y+1, NULL) || tile(move.target.x, move.target.y-1, NULL)))
		return false;

    //Step2: Check if there already is a piece on the tile your trying to move to
    for (int i = 0; i < TILE_COUNT; i++) { //for each (RefTile^ tile in Tiles){
        if (piece(move.target.x, move.target.y, NULL))
            return false;

        //Step3: Check if there is a piece between your current location, and the new one
        int pieceLocationX, pieceLocationY, distancex=0, distancey=0;
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

bool BoardState::is_valid_tile_placement(int x, int y) {
    
    //Check for tileplacement | left | right | up | down
    if (!tile(x - 1, y, NULL) &&
        !tile(x + 1, y, NULL) &&
        !tile(x, y - 1, NULL) &&
        !tile(x, y + 1, NULL))
        return false;
    
    return true;
}

bool BoardState::is_corner_tile(int x, int y) {
    return false;
    /*
    int numberFreeSides = 0;

    if (!tile(x - 1, y, NULL))
        numberFreeSides++;
    if (!tile(x + 1, y, NULL))
        numberFreeSides++;
    if (!tile(x, y - 1, NULL))
        numberFreeSides++;
    if (!tile(x, y + 1, NULL))
        numberFreeSides++;

    if (numberFreeSides > 1)
        return true;

    return false;*/
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

void BoardState::update_neighbors(BoardPosition oldPos, BoardPosition newPos,
    BoardTile * t) {
#define UNSET_NEIGHBOR(xp, yp, d) { BoardTile * tmp; if(tile(xp, yp, &tmp)) \
                                { tmp->neighbors[DIRECTION_FLIP(d)] = NULL; } }

#define SET_NEIGHBOR(xp, yp, d, n) { BoardTile * tmp; if(tile(xp, yp, &tmp)) \
                                { tmp->neighbors[DIRECTION_FLIP(d)] = n; } }

    // Unset neighbors of old tile position.
    UNSET_NEIGHBOR(oldPos.x + 0, oldPos.y - 1, DIRECTION_NORTH);
    UNSET_NEIGHBOR(oldPos.x + 1, oldPos.y - 1, DIRECTION_NORTHEAST);
    UNSET_NEIGHBOR(oldPos.x + 1, oldPos.y + 0, DIRECTION_EAST);
    UNSET_NEIGHBOR(oldPos.x + 1, oldPos.y + 1, DIRECTION_SOUTHEAST);
    UNSET_NEIGHBOR(oldPos.x + 0, oldPos.y + 1, DIRECTION_SOUTH);
    UNSET_NEIGHBOR(oldPos.x - 1, oldPos.y + 1, DIRECTION_SOUTHWEST);
    UNSET_NEIGHBOR(oldPos.x - 1, oldPos.y + 0, DIRECTION_WEST);
    UNSET_NEIGHBOR(oldPos.x - 1, oldPos.y - 1, DIRECTION_NORTHWEST);

    // Set neighbors of new tile position.
    SET_NEIGHBOR(newPos.x + 0, newPos.y - 1, DIRECTION_NORTH, t);
    SET_NEIGHBOR(newPos.x + 1, newPos.y - 1, DIRECTION_NORTHEAST, t);
    SET_NEIGHBOR(newPos.x + 1, newPos.y + 0, DIRECTION_EAST, t);
    SET_NEIGHBOR(newPos.x + 1, newPos.y + 1, DIRECTION_SOUTHEAST, t);
    SET_NEIGHBOR(newPos.x + 0, newPos.y + 1, DIRECTION_SOUTH, t);
    SET_NEIGHBOR(newPos.x - 1, newPos.y + 1, DIRECTION_SOUTHWEST, t);
    SET_NEIGHBOR(newPos.x - 1, newPos.y + 0, DIRECTION_WEST, t);
    SET_NEIGHBOR(newPos.x - 1, newPos.y - 1, DIRECTION_NORTHWEST, t);

#undef UNSET_NEIGHBOR
#undef SET_NEIGHBOR
}

void BoardState::apply_move(BoardMove move, BoardPlayer player) {
    int count = piece_count();
    BoardTile * newTile = NULL;

    assert(player);
    assert(!piece(move.target.x, move.target.y, NULL));

    // If in initial phase, place the piece at the target position.
    if (count < PIECE_COUNT)
    {
        assert(!move.piece);
        assert(!move.tile);

        // Set the occupied tile to the piece.
        bool ok_place_pos = 
            tile(move.target.x, move.target.y, &pieces_[count].tile);

        assert(ok_place_pos);
        
        // Set the owner of this piece.
        pieces_[count].player = player;

        // Set the piece occupying this tile.
        pieces_[count].tile->piece = &pieces_[count];

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

        if (move.tile->piece) {
            cout << "ERRRR 0x" << move.tile << "/0x" << move.tile->piece->tile << endl;
        }
        assert(!move.tile->piece || move.tile->piece->tile == move.tile);
        assert(!move.tile->piece && "moving tile is occupied");

        // Update the tile position, update the neighbors
        BoardPosition oldTilePosition = move.tile->position;
        move.tile->position = move.target;
        update_neighbors(oldTilePosition, move.target, move.tile);

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
    tile(move.target.x, move.target.y, &move.piece->tile);
    move.piece->tile->piece = move.piece;
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

        return;
    }

    // If a tile was moved, move it back.
    if (move.tile) {
        assert(!tile(move.tile_position.x, move.tile_position.y, NULL));

        update_neighbors(move.tile->position, move.tile_position, move.tile);
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
}

int BoardState::corner_tiles(BoardTile ** tiles, int count) {
    int idx = 0;

    for (int i = 0; i < TILE_COUNT;i++) {
        // If the tile has at least 2 adjacent disconnected edges it is a corner.
        bool top = tile(tiles_[i].position.x, tiles_[i].position.y - 1, NULL);
        bool bottom = tile(tiles_[i].position.x, tiles_[i].position.y + 1, NULL);
        bool left = tile(tiles_[i].position.x - 1, tiles_[i].position.y, NULL);
        bool right = tile(tiles_[i].position.x + 1, tiles_[i].position.y, NULL);

        if (((top == false && left == false) ||
            (top == false && right == false) ||
            (bottom == false && left == false) ||
            (bottom == false && right == false)) &&
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
            is_row_for_player(pieces_[i].tile->position.x, 
            pieces_[i].tile->position.y, pieces_[i].player))

			return pieces_[i].player;
    }
    return PLAYER_NONE;
}

int BoardState::row_length(int x, int y, int ox, int oy, BoardPlayer player) {
    return 1;
    x += ox;
    y += oy;

    BoardPiece * p;
    if (!piece(x, y, &p)) return 0;

    if (!p->is_face_up || p->player != player) return 0;

    return 1 + row_length(x, y, ox, oy, player);

}

bool BoardState::is_row_for_player(int x, int y, BoardPlayer player) {
    int a1 = row_length(x, y, -1, -1, player) + 1 + row_length(x, y, 1, 1, player);
    int a2 = row_length(x, y, -1, 1, player) + 1 + row_length(x, y, 1, -1, player);
    int a3 = row_length(x, y, 0, -1, player) + 1 + row_length(x, y, 0, 1, player);
    int a4 = row_length(x, y, -1, 0, player) + 1 + row_length(x, y, 1, 0, player);

	return a1 >= 4 || a2 >= 4 || a3 >= 4 || a4 >= 4;
}