
#include "Tile.h"
#include "Piece.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace Karo::Common;

namespace Karo {
    namespace Core {

        public ref class Karo
        {
        public:
            Karo();
            Karo(array<Tile^>^ tiles, array<Piece^>^ pieces);
            property IEnumerable<Tile^>^ Tiles {
                IEnumerable<Tile^>^ get();
            }
            property IEnumerable<Piece^>^ Pieces {
                IEnumerable<Piece^>^ get();
            }
            property bool IsFinished {
                bool get();
            };
            IEnumerable<Move^>^ GetAvailableMoves();
            bool IsValidMove(Move^ move);
            Karo^ WithMoveApplied(Move^ move);
        private:
            array<Tile^>^ _tiles;
            array<Piece^>^ _pieces;
            Piece^ GetPiece(int x, int y);
            int GetRowLength(int x, int y, int offsetX, int offsetY, Player player);
        };
    }
}