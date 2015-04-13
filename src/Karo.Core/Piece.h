// Piece.h

#pragma once

#include "Tile.h"

using namespace System;

namespace Karo {
    namespace Core {

        public ref class Piece
        {
        public:
            Piece()
            {
            }
            property bool IsFaceUp;
            property Tile^ Tile;
        };
    }
}
