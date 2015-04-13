
#include "Tile.h"

using namespace System;

namespace Karo {
    namespace Core {

        public ref class Karo
        {
        public:
            Karo();
            array<Tile^>^ GetTiles() {
                return _tiles;
            }
        private:
            array<Tile^> ^ _tiles;
        };
    }
}