using namespace System;
using namespace System::Collections::Generic;
using namespace Karo::Common;

#include "KaroPlayer.h"
#include "BoardState.h"

namespace Karo {
    namespace Core {
        public ref class Tile
        {
        public:
            property int X;
            property int Y;
            Tile(int x, int y) {
                X = x;
                Y = y;
            }
        };
        public ref class Piece
        {
        public:
            property int X;
            property int Y;
            property bool IsFaceUp;
            property KaroPlayer Player;
            Piece(Tile^ tile, KaroPlayer player, bool isFaceUp) {
                X = tile->X;
                Y = tile->Y;
                Player = player;
                IsFaceUp = isFaceUp;
            }
            Piece(int x, int y, KaroPlayer player, bool isFaceUp) {
                X = x;
                Y = y;
                IsFaceUp = isFaceUp;
                Player = player;
            }
        };

        public ref class KaroBoardState
        {
        public:
            KaroBoardState();
            KaroBoardState(array<Tile^>^ tiles, array<Piece^>^ pieces);
            ~KaroBoardState();
            property IEnumerable<Tile^>^ Tiles {
                IEnumerable<Tile^>^ get();
            }
            property IEnumerable<Piece^>^ Pieces {
                IEnumerable<Piece^>^ get();
            }
            property IEnumerable<Tile^>^ CornerTiles {
                IEnumerable<Tile^>^ get();
            }
            property bool IsFinished {
                bool get();
            };
            property int PieceCount {
                int get();
            }
            IEnumerable<Move^>^ GetAvailableMoves(KaroPlayer player);
            bool IsValidMove(Move^ move);
            KaroBoardState^ WithMoveApplied(Move^ move, KaroPlayer player);
            Tile^ GetTile(int x, int y);
            Piece^ GetPiece(int x, int y);
            KaroPlayer GetWinner();
        private:
            KaroBoardState(BoardState * state);
            BoardState *state_ = 0;
        };
    }
}