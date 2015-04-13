// Tile.h
#pragma once

namespace Karo {
    namespace Core {
        public ref class Tile {
        public:
            Tile(int x, int y)  {
                X = x;
                Y = y;
            }
            property int X;
            property int Y;
        };
    }
}