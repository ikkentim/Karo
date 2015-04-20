#include "Stdafx.h"
#include "Karo.h"

namespace Karo {
    namespace Core {
        Karo::Karo() {
            _tiles = gcnew array<Tile^>(20);
            for (int i = 0; i < 20; i++) {
                _tiles[i] = gcnew Tile(i % 5, i / 5);
            }
        }
        bool Karo::IsFinished::get()
        {
            //todo: Implement
            return false;
        }
        IEnumerable<Piece^>^ Karo::Pieces::get() {
            return _pieces;
        }
        IEnumerable<Tile^>^ Karo::Tiles::get() {
            return _tiles;
        }
        IEnumerable<Move^>^ Karo::GetAvailableMoves() {
            return nullptr;
        }

        bool Karo::IsValidMove(Move^ move) {
            //todo: Implement
            return true;
        }
        Karo^ Karo::WithMoveApplied(Move^ move) {
            //todo: Implement
            return nullptr;
        }
    }
}