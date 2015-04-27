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

        Karo::Karo(array<Tile^>^ tiles, array<Piece^>^ pieces) {
            if (tiles == nullptr)
                throw gcnew ArgumentNullException("tiles");
            if (pieces == nullptr)
                throw gcnew ArgumentNullException("pieces");

            if (tiles->Length != 20)
                throw gcnew ArgumentException("The length of tiles must be 20");

            if (pieces->Length != 12)
                throw gcnew ArgumentException("The length of pieces must be 12");

            _tiles = tiles;
            _pieces = pieces;
        }

        Piece^ Karo::GetPiece(int x, int y) {
            for each(auto piece in Pieces)
                if (piece != nullptr && piece->Tile->X == x && piece->Tile->Y == y)
                    return piece;
        }

        int Karo::GetRowLength(int x, int y, int offsetX, int offsetY, Player player) {
            x += offsetX;
            y += offsetY;

            auto piece = GetPiece(x, y);

            if (piece == nullptr || !piece->IsFaceUp || piece->Player != player) return 0;

            return 1 + GetRowLength(x, y, offsetX, offsetY, player);
        }

        bool Karo::IsFinished::get() {
            for each(auto piece1 in Pieces) {
                if (piece1 == nullptr || !piece1->IsFaceUp) {
                    return false;
                }

                int x = piece1->Tile->X;
                int y = piece1->Tile->Y;

                return 1 + GetRowLength(x, y, -1, -1, piece1->Player) +
                    GetRowLength(x, y, 1, 1, piece1->Player) >= 4 ||
                    1 + GetRowLength(x, y, 1, -1, piece1->Player) +
                    GetRowLength(x, y, -1, 1, piece1->Player) >= 4 ||
                    1 + GetRowLength(x, y, -1, 0, piece1->Player) +
                    GetRowLength(x, y, 1, 0, piece1->Player) >= 4 ||
                    1 + GetRowLength(x, y, 0, -1, piece1->Player) +
                    GetRowLength(x, y, 0, 1, piece1->Player) >= 4;
            }

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
			else // we're in the tile moving phase
			{
				for each (Piece^ piece in Pieces)
				{
					// loop surrounding tiles for positions
					for (int x = -1; x++; x <= 1)
					{
						for (int y = -1; y++; y <= 1)
						{
							Tile^ surroundingTile = GetTileAt(x, y);
						}
					}
				}
			}

			return moves;
        }

		Tile^ Karo::GetTileAt(int x, int y)
		{
			if (Tiles == nullptr)
				return nullptr;

			for each (Tile^ tile in Tiles)
			{
				if (tile != nullptr)
					if (tile->X == x && tile->Y == y)
						return tile;
			}

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