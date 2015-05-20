#include "Stdafx.h"
#include "KaroBoardState.h"
#include <cstring>
#include <assert.h>

namespace Karo {
    namespace Core {
        KaroBoardState::KaroBoardState() {
            Console::WriteLine("KaroBoardState::KaroBoardState");
            state_ = new BoardState();
        }
        KaroBoardState::KaroBoardState(array<Tile^>^ tiles, 
            array<Piece^>^ pieces) {
            assert(tiles != nullptr);
            assert(pieces != nullptr);
            assert(tiles->Length == TILE_COUNT);
            assert(pieces->Length == PIECE_COUNT);

            BoardTile * pTiles = new BoardTile[TILE_COUNT];
            BoardPiece * pPieces = new BoardPiece[PIECE_COUNT];

            for (int i = 0; i < TILE_COUNT; i++)
                if (tiles[i] != nullptr)
                    pTiles[i] = BoardTile(tiles[i]->X, tiles[i]->Y);

            for (int i = 0; i < PIECE_COUNT; i++)
                if (pieces[i] != nullptr)
                {
                    for (int j = 0; j < TILE_COUNT; j++) 
                        if (pTiles[j].position.x == pieces[i]->X && pTiles[j].position.y == pieces[i]->Y)
                            pPieces[i].tile = &pTiles[j];

                    pPieces[i].is_face_up = pieces[i]->IsFaceUp;
                    pPieces[i].player = (int)pieces[i]->Player;
                }

            state_ = new BoardState(pTiles, pPieces);
        }

        KaroBoardState::~KaroBoardState() {
            delete state_;
            state_ = 0;
        }

        IEnumerable<Tile^>^ KaroBoardState::Tiles::get() {
            auto result = gcnew List<Tile^>();
            auto tiles = state_->tiles();

            for (int i = 0; i < TILE_COUNT; i++)
                result->Add(gcnew Tile(tiles[i].position.x, tiles[i].position.y));

            return result;
        }

        IEnumerable<Piece^>^ KaroBoardState::Pieces::get() {
            auto result = gcnew List<Piece^>();
            auto pieces = state_->pieces();

            for (int i = 0; i < state_->piece_count(); i++)
                result->Add(gcnew Piece(pieces[i].tile->position.x, pieces[i].tile->position.y, 
                (KaroPlayer)pieces[i].player, pieces[i].is_face_up));
            
            return result;
        }

        IEnumerable<Tile^>^ KaroBoardState::CornerTiles::get() {
            auto result = gcnew List<Tile^>();
            BoardTile ** tiles = new BoardTile *[TILE_COUNT];
            int count = state_->corner_tiles(tiles, TILE_COUNT);

            for (int i = 0; i < count; i++)
                result->Add(gcnew Tile(tiles[i]->position.x, tiles[i]->position.y));

            delete[] tiles;
            return result;
        }
        bool KaroBoardState::IsFinished::get() {
            return state_->is_finished();
        }

        int KaroBoardState::PieceCount::get() {
            return state_->piece_count();
        }

        IEnumerable<Move^>^ KaroBoardState::GetAvailableMoves(KaroPlayer player) {
            auto result = gcnew List<Move^>();
            BoardMove * moves = new BoardMove[MOVE_COUNT];
            int count = state_->available_moves((BoardPlayer)player, moves, MOVE_COUNT);

            for (int i = 0; i < count; i++)
                result->Add(gcnew Move(moves[i].target.x, moves[i].target.y,
                moves[i].piece->tile->position.x, moves[i].piece->tile->position.y,
                moves[i].tile->position.x, moves[i].tile->position.y));

            delete[] moves;
            return result;
        }

        bool KaroBoardState::IsValidMove(Move^ move) {
            BoardMove m;
            state_->tile(move->OldTileX, move->OldTileY, &m.tile);
            state_->piece(move->OldPieceX, move->OldPieceY, &m.piece);
            m.target.x = move->NewPieceX;
            m.target.y = move->NewPieceY;

            return state_->is_valid_move(m);
        }

        void KaroBoardState::ApplyMove(Move^ move, KaroPlayer player) {
            state_->apply_move(state_->create_move(
                BoardPosition(move->NewPieceX, move->NewPieceY), 
                BoardPosition(move->OldPieceX, move->OldPieceY), 
                BoardPosition(move->OldTileX, move->OldTileY)), (BoardPlayer)player);
        }

        Tile^ KaroBoardState::GetTile(int x, int y) {
            BoardTile * result;
            int tmp = state_->piece_count();
            if (state_->tile(x, y, &result))
            {
                auto t = gcnew Tile(result->position.x, result->position.y);
                return t;
            }
            else
            {
                return nullptr;
            }
        }
        Piece^ KaroBoardState::GetPiece(int x, int y) {
            BoardPiece * result;
            return state_->piece(x, y, &result) 
                ? gcnew Piece(result->tile->position.x, result->tile->position.y,
                (KaroPlayer)result->player, result->is_face_up) 
                : nullptr;
         
        }
        KaroPlayer KaroBoardState::GetWinner() {
            int w = static_cast<int> (state_->winner());
            assert(w <= 2 && w >= 0);
            return (KaroPlayer)w;
        }

        bool KaroBoardState::IsCornerTile(int x, int y){
			if (GetTile(x - 1, y) == nullptr && GetTile(x, y - 1) == nullptr && (
				(GetTile(x - 1, y - 1)) != nullptr ||
				(GetTile(x - 1, y + 1)) != nullptr ||
				(GetTile(x + 1, y - 1)) != nullptr ||
				(GetTile(x + 1, y + 1)) != nullptr))
				return true;
			if (GetTile(x - 1, y) == nullptr && GetTile(x, y + 1) == nullptr && (
				(GetTile(x - 1, y - 1)) != nullptr ||
				(GetTile(x - 1, y + 1)) != nullptr ||
				(GetTile(x + 1, y - 1)) != nullptr ||
				(GetTile(x + 1, y + 1)) != nullptr))
				return true;
			if (GetTile(x + 1, y) == nullptr && GetTile(x, y - 1) == nullptr && (
				(GetTile(x - 1, y - 1)) != nullptr ||
				(GetTile(x - 1, y + 1)) != nullptr ||
				(GetTile(x + 1, y - 1)) != nullptr ||
				(GetTile(x + 1, y + 1)) != nullptr))
				return true;
			if (GetTile(x + 1, y) == nullptr && GetTile(x, y + 1) == nullptr && (
				(GetTile(x - 1, y - 1)) != nullptr ||
				(GetTile(x - 1, y + 1)) != nullptr ||
				(GetTile(x + 1, y - 1)) != nullptr ||
				(GetTile(x + 1, y + 1)) != nullptr))
				return true;

			return false;
		}
    }
}