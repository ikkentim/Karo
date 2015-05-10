#include "Stdafx.h"
#include "KaroBoardState.h"
#include <cstring>

namespace Karo {
    namespace Core {
        KaroBoardState::KaroBoardState() {
            state_ = new BoardState();
        }

        KaroBoardState::KaroBoardState(BoardState * state) {
            state_ = state;
        }

        KaroBoardState::~KaroBoardState() {
            delete state_;
            state_ = 0;
        }

        IEnumerable<Tile^>^ KaroBoardState::Tiles::get() {
            auto result = gcnew List<Tile^>();
            auto tiles = state_->tiles();

            for (int i = 0; i < TILE_COUNT; i++)
                result->Add(gcnew Tile(tiles[i].x, tiles[i].y));

            return result;
        }

        IEnumerable<Piece^>^ KaroBoardState::Pieces::get() {
            auto result = gcnew List<Piece^>();
            auto pieces = state_->pieces();

            for (int i = 0; i < state_->piece_count(); i++)
                result->Add(gcnew Piece(pieces[i].tile.x, pieces[i].tile.y, 
                pieces[i].is_face_up, (KaroPlayer)pieces[i].player));

            return result;
        }

        IEnumerable<Tile^>^ KaroBoardState::CornerTiles::get() {
            auto result = gcnew List<Tile^>();
            BoardTile * tiles = new BoardTile[TILE_COUNT];
            int count = state_->corner_tiles(tiles, TILE_COUNT);

            for (int i = 0; i < count; i++)
                result->Add(gcnew Tile(tiles[i].x, tiles[i].y));

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
                moves[i].piece.tile.x, moves[i].piece.tile.y,
                moves[i].tile.x, moves[i].tile.y));

            delete[] moves;
            return result;
        }

        bool KaroBoardState::IsValidMove(Move^ move) {
            BoardMove m;
            m.tile.x = move->OldTileX;
            m.tile.y = move->OldTileY;
            m.piece.tile.x = move->OldPieceX;
            m.piece.tile.y = move->OldPieceY;
            m.target.x = move->NewPieceX;
            m.target.y = move->NewPieceY;

            return state_->is_valid_move(m);
        }

        KaroBoardState^ KaroBoardState::WithMoveApplied(Move^ move, KaroPlayer player) {
            BoardMove m;
            m.tile.x = move->OldTileX;
            m.tile.y = move->OldTileY;
            m.piece.tile.x = move->OldPieceX;
            m.piece.tile.y = move->OldPieceY;
            m.target.x = move->NewPieceX;
            m.target.y = move->NewPieceY;

            return gcnew KaroBoardState(new BoardState(
                state_->with_move_applied(m, (BoardPlayer)player)));
        }

        Tile^ KaroBoardState::GetTile(int x, int y) {
            BoardTile result;
            int tmp = state_->piece_count();
            if (state_->tile(x, y, &result))
            {
                auto t = gcnew Tile(result.x, result.y);
                return t;
            }
            else
            {
                return nullptr;
            }
        }
        Piece^ KaroBoardState::GetPiece(int x, int y) {
            BoardPiece result;
            if (state_->piece(x, y, &result))
            {
                return gcnew Piece(result.tile.x, result.tile.y, result.is_face_up, (KaroPlayer)result.player);
            }
            else
            {
                return nullptr;
            }
        }
        KaroPlayer KaroBoardState::GetWinner() {
            return (KaroPlayer)state_->winner();
        }
    }
}