#pragma once

#include "BoardPlayer.h"
#include "BoardTile.h"

struct BoardTile;

struct BoardPiece {
    BoardTile * tile;
    BoardPlayer player;
    bool is_face_up;

    BoardPiece() 
        : tile(), player(PLAYER_NONE), is_face_up(false) { }

    inline bool operator==(BoardPiece o) {
        return tile == o.tile && player == o.player && is_face_up == o.is_face_up;
    }

    inline bool operator!=(BoardPiece o) {
        return !(*this == o);
    }
};