#include "stdafx.h"
#include "Player.h"

using namespace Karo::Common;

namespace Karo {
    namespace AI {
        Player::Player() {
            intelligence_ = new Intelligence();
        }

        Player::~Player() {
            delete intelligence_;
        }

        void Player::DoMove(Karo::Common::Move^ previousMove, int timeLimit, Action<Karo::Common::Move^>^ done) {
            
            // If oppenent made a move, apply it to the internal board state.
            if (previousMove != nullptr) {
                // Convert GC'd move to unmanged move.
                BoardMove m;
                m.tile.x = previousMove->OldTileX;
                m.tile.y = previousMove->OldTileY;
                m.piece.tile.x = previousMove->OldPieceX;
                m.piece.tile.y = previousMove->OldPieceY;
                m.target.x = previousMove->NewPieceX;
                m.target.y = previousMove->NewPieceY;

                intelligence_->apply_move(m, PLAYER_PLAYER2);
            }

            // Choose the best move.
            BoardMove newMove = intelligence_->choose_best_move(timeLimit, PLAYER_PLAYER1);

            // Apply our move to our internal board state.
            intelligence_->apply_move(newMove, PLAYER_PLAYER1);

            // Convert unmanaged move to GC'd move.
            Karo::Common::Move^ gcMove = gcnew Move(newMove.target.x, newMove.target.y,
                newMove.piece.tile.x, newMove.piece.tile.y,
                newMove.tile.x, newMove.tile.y);

            done->Invoke(gcMove);
        }
    }
}