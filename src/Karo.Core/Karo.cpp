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
        IEnumerable<Move^>^ Karo::GetAvailableMoves(Player player) {
			array<Move^>^ moves = gcnew array<Move^>(48); // 6 pieces * 8 directions

			int i = 0;

			if (PieceCount() < 12) // we're still in the first phase
			{
				for each (Tile^ tile in Tiles)
				{
					if (!tile->HasPiece)
					{
						moves[i++] = gcnew Move(tile->X, tile->Y, 0, 0, 0, 0);
					}
				}
			}

			return moves;
        }

        bool Karo::IsValidMove(Move^ move) {
            //todo: Implement
            return true;
        }
        Karo^ Karo::WithMoveApplied(Move^ move) {
            //todo: Implement
            return nullptr;
        }
		int Karo::PieceCount()
		{
			if (Pieces == nullptr)
				return 0;

			int i = 0;

			for each (Piece^ piece in Pieces)
				if (piece != nullptr)
					i++;

			return i;
		}
    }
}