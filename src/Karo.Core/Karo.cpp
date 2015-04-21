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

        IEnumerable<Tile^>^ Karo::GetCornerTiles() {
            List<Tile ^>^ tiles = gcnew List<Tile^>();

            for each(Tile^ tile in Tiles) {
                Tile^ top = GetTileAt(tile->X, tile->Y - 1);
                Tile^ bottom = GetTileAt(tile->X, tile->Y + 1);
                Tile^ left = GetTileAt(tile->X - 1, tile->Y);
                Tile^ right = GetTileAt(tile->X + 1, tile->Y);

                if (((top == nullptr && left == nullptr) ||
                    (top == nullptr && right == nullptr) ||
                    (bottom == nullptr && left == nullptr) ||
                    (bottom == nullptr && right == nullptr)) &&
                    GetPiece(tile->X, tile->Y) == nullptr)
                    tiles->Add(tile);
            }

            return tiles;
        }
        IEnumerable<Move^>^ Karo::GetAvailableMoves(Player player) {
			List<Move^>^ moves = gcnew List<Move^>(48); // 6 pieces * 8 directions

			if (PieceCount() < 12) // we're still in the first phase
			{
				for each (Tile^ tile in Tiles)
				{
					if (!tile->HasPiece)
					{
						moves->Add(gcnew Move(tile->X, tile->Y, 0, 0, 0, 0));
					}
				}
			}
			else // we're in the tile moving phase
			{
                IEnumerable<Tile^>^cornerTiles;

				for each (Piece^ piece in Pieces)
				{
                    if (piece->Player != player) continue;

					// loop surrounding tiles for positions
                    for (int x = -1; x <= 1; x++)
					{
                        for (int y = -1; y <= 1; y++)
                        {
                            if (x == 0 && y == 0) continue;

                            int newx = piece->Tile->X + x;
                            int newy = piece->Tile->Y + y;

                            // todo: check is filled, jump over
                            Piece^ pieceOnTile = GetPiece(newx, newy);

                            if (pieceOnTile != nullptr) {
                                newx += x;
                                newy += y;

                                pieceOnTile = GetPiece(newx, newy);

                                if (pieceOnTile != nullptr) {
                                    continue;
                                }
                            }

                            Tile^ surroundingTile = GetTileAt(newx, newy);

                            if (surroundingTile == nullptr)
                            {
                                if (cornerTiles == nullptr) {
                                    cornerTiles = GetCornerTiles();
                                }

                                for each(Tile^ cornerTile in cornerTiles)
                                {
                                    Tile^ top = GetTileAt(newx, newy - 1);
                                    Tile^ bottom = GetTileAt(newx, newy + 1);
                                    Tile^ left = GetTileAt(newx - 1, newy);
                                    Tile^ right = GetTileAt(newx + 1, newy);

                                    if (top == nullptr && bottom == nullptr && left == nullptr && right == nullptr) continue;

                                    moves->Add(gcnew Move(newx, newy, piece->Tile->X, piece->Tile->Y, cornerTile->X, cornerTile->Y));
                                }
                            }
                            else
                            {
                                if (pieceOnTile == nullptr) {
                                    moves->Add(gcnew Move(newx, newy, piece->Tile->X, piece->Tile->Y, 0, 0));
                                }
                            }

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
			//Check if new location != old location
			if (move->NewPieceX == move->OldPieceX && move->NewPieceY == move->OldPieceY)
				return false;

			//Check if there is a piece on the tile you want to move to
			for each (Tile^ tile in Tiles){
				if (tile->X == move->NewPieceX && tile->Y == move->NewPieceY && tile->HasPiece)
					return false;

				//Check if the new location is into 'move-range'.
				//Don't let pieces move 5 tiles..
				//Keep in mind the possibilty of jumping over other pieces!!
				int distanceX, distanceY,totalDistance;
				distanceX = move->NewPieceX - move->OldPieceX;
				distanceY = move->NewPieceY - move->OldPieceY;
				if (distanceX < 0)
					distanceX = Math::Abs(-distanceX);
				if (distanceY < 0)
					distanceY = Math::Abs(-distanceY);

				totalDistance = distanceX + distanceY;
				if (totalDistance < 0)
					totalDistance = Math::Abs(-totalDistance);

				//No does not allow jumping yet!
				if (totalDistance > 3)
					return false;

				//If there is not, check if there is a tile on that position
				//There is a tile, without piece
				else if (tile->X == move->NewPieceX && tile->Y == move->NewPieceY)
					return true;
			}
			//If there is no tile, check if you can move a tile to this position
			//todo:: tile moving

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