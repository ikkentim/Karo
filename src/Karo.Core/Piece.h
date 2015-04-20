// Piece.h

#pragma once

#include "Tile.h"
#include "Player.h"

using namespace System;

namespace Karo {
    namespace Core {

        public ref class Piece
        {
        public:
            Piece(Tile^ tile, Player player, bool isFaceUp) 
            {
                Tile = tile;
                Player = player;
                IsFaceUp = isFaceUp;
            }
            property bool IsFaceUp;
            property Tile^ Tile;
            property Player Player;
        };
    }
}
