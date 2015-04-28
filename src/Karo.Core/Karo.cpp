#include "Stdafx.h"
#include "Karo.h"

namespace Karo {
    namespace Core {
        Karo::Karo() {
            _tiles = gcnew array<Tile^>(20);
            for (int i = 0; i < 20; i++) {
                _tiles[i] = gcnew Tile(i % 5, i / 5);
            }
            _pieces = gcnew array<Piece^>(12);
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

			//Step0: Initphase //Place piece on current location, just check if there is a piece already
			if (PieceCount() <= 11)
			{
				if (GetPiece(move->NewPieceX, move->NewPieceY) != nullptr)
					return false;
				else
					return true;
			}
			//Move Phase
			else{
				//Step1: Check if new location is different from the current one
				if (move->NewPieceX == move->OldPieceX && move->NewPieceY == move->OldPieceY)
					return false;

				//Step2: Check if there already is a piece on the tile your trying to move to
				for each (Tile^ tile in Tiles){
					if (GetPiece(move->NewPieceX,move->NewPieceY)!= nullptr)
						return false;

					//Step3: Check if there is a piece between your current location, and the new one
					int pieceLocationX, pieceLocationY, distancex, distancey;
					//If the X location won't change, keep the X location. otherwise add distance
					if (move->NewPieceX == move->OldPieceX)
						pieceLocationX = move->NewPieceX;
					else{
						distancex = (move->NewPieceX - move->OldPieceX);
						if (distancex < 0)
							distancex = Math::Abs(-distancex);
						if (move->NewPieceX > move->OldPieceX)
							pieceLocationX = (move->OldPieceX + distancex / 2);
						else
							pieceLocationX = (move->OldPieceX - distancex / 2);

					}
					if (move->NewPieceY == move->OldPieceY)
						pieceLocationY = move->OldPieceY;
					else{
						distancey = (move->NewPieceY - move->OldPieceY);
						if (distancey < 0)
							distancey = Math::Abs(-distancey); 
						if (move->NewPieceY > move->OldPieceY)
							pieceLocationY = (move->OldPieceY + distancey / 2);
						else
							pieceLocationY = (move->OldPieceY - distancey / 2);
					}

					if (distancex > 2 || distancey > 2)
						return false;
					//If there is a piece on the calculated X and Y, Then you can jump over it (if enemy)
					if (GetPiece(pieceLocationX, pieceLocationY) != nullptr &&
						GetPiece(pieceLocationX, pieceLocationY)->Player != GetPiece(move->OldPieceX,move->OldPieceY)->Player)
						return true;
					//Check if the new location is into 'move-range'.
					//Distance of X and Y should be max 1. Unless there is a enemy piece next to it (jumping)
					//Keep in mind the possibilty of jumping over other pieces!!
					if (distancex > 1 || distancey > 1)
						return false;
				}
				return true;
			}
		}


        Karo^ Karo::WithMoveApplied(Move^ move, Player player) {
            if (move == nullptr) {
                throw gcnew ArgumentNullException("move");
            }

            array<Tile^>^ tiles = gcnew array<Tile^> (20);
            array<Piece^>^ pieces = gcnew array<Piece^>(12);
            int tidx = 0, pidx = 0;

            Tile^ movingTile = nullptr, ^targetTile = nullptr;
            Piece^ movingPiece = nullptr;
            
            for each(Tile^ tile in Tiles) {
                Tile^ newTile = tiles[tidx++] = gcnew Tile(tile->X, tile->Y);

                if (newTile->X == move->OldTileX && newTile->Y == move->OldTileY) {
                    movingTile = newTile;
                }
                if (newTile->X == move->NewPieceX && newTile->Y == move->NewPieceY) {
                    targetTile = newTile;
                }
            }

                for each(Piece^ piece in Pieces) {
                if (piece == nullptr) continue;
                Tile^ tile = nullptr;

                for each(Tile^ t in tiles) {
                    if (t->X == piece->Tile->X && t->Y == piece->Tile->Y) {
                        tile = t;
                        continue;
                    }
                }

                if (tile == nullptr) {
                    throw gcnew Exception("Invalid board state");
                }

                pieces[pidx++] = gcnew Piece(tile, piece->Player, piece->IsFaceUp);

                if (pieces[pidx - 1]->Tile->X == move->OldPieceX && pieces[pidx - 1]->Tile->Y == move->OldPieceY)
                    movingPiece = pieces[pidx - 1];
            }

            int pieceCount = PieceCount();
            if (pieceCount < 12) {
                if (targetTile == nullptr) {
                    throw gcnew Exception("Invalid board state");
                }
                pieces[pieceCount] = gcnew Piece(targetTile, player, false);
                return gcnew Karo(tiles, pieces);;
            }
            if (movingPiece == nullptr) {
                throw gcnew Exception("Invalid board state");
            }

            if (targetTile == nullptr) {
                if (movingTile == nullptr) {
                    throw gcnew Exception("Invalid board state");
                }

                targetTile = movingTile;
                targetTile->X = move->NewPieceX;
                targetTile->Y = move->NewPieceY;
            }

            movingPiece->Tile = targetTile;

            if (Math::Abs(move->NewPieceX - move->OldPieceX) > 1 || Math::Abs(move->NewPieceY - move->OldPieceY) > 1) {
                movingPiece->IsFaceUp = !movingPiece->IsFaceUp;
            }

            return gcnew Karo(tiles, pieces);
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