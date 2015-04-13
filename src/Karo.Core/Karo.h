// Karo.Core.h

#pragma once

#include "Tile.h"

using namespace System;

namespace Karo {
    namespace Core {

        public ref class Karo
        {
        public:
            Karo() {
                _tiles = gcnew array<Tile^>(20);
                for (int i = 0; i < 20; i++) {
                    _tiles[i] = gcnew Tile(i % 6, i / 6);
                }
            }
        private:
            array<Tile^> ^ _tiles;

        };
    }
}