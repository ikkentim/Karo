
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
            IEnumerable<Move^>^ GetAvailableMoves(Player player);
            bool IsValidMove(Move^ move);
            Karo^ WithMoveApplied(Move^ move, Player player);
			int PieceCount();
            Tile^ GetTileAt(int, int);
            IEnumerable<Tile^>^ GetCornerTiles();
            Piece^ GetPiece(int x, int y);
			bool IsValidTilePlacement(int x, int y);
			bool IsCornerTile(int x, int y);
            Player GetWinner();
            int GetRowLength(int x, int y, int offsetX, int offsetY, Player player);
        private:
            bool IsRowForPlayerAt(int x, int y, Player player);
            array<Tile^>^ _tiles;
            array<Piece^>^ _pieces;
        };
    }
}