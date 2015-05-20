#pragma once

#include <stddef.h>
#include "BoardPosition.h"
#include "BoardPiece.h"

#define DIRECTION_NORTH         (0)
#define DIRECTION_NORTHEAST     (1)
#define DIRECTION_EAST          (2)
#define DIRECTION_SOUTHEAST     (3)
#define DIRECTION_SOUTH         (4)
#define DIRECTION_SOUTHWEST     (5)
#define DIRECTION_WEST          (6)
#define DIRECTION_NORTHWEST     (7)
#define DIRECTION_COUNT         (8)
#define DIRECTION_FLIP(d)       ((d + 4) % DIRECTION_COUNT)

struct BoardTile {
    BoardPosition position;
    BoardPiece * piece;
    BoardTile * neighbors[DIRECTION_COUNT];
    BoardTile() { }

    BoardTile(int xx, int yy)
        : position(xx, yy) { }

    inline bool operator==(BoardTile o) {
        return position == o.position;
    }

    inline bool operator!=(BoardTile o) {
        return !(*this == o);
    }
};