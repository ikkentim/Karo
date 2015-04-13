#include "Stdafx.h"
#include "Karo.h"

namespace Karo {
    namespace Core {
        Karo::Karo() {
            _tiles = gcnew array<Tile^>(20);
            for (int i = 0; i < 20; i++) {
                _tiles[i] = gcnew Tile(i % 6, i / 6);
            }
        }
    }
}