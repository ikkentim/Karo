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
                BoardMove m = intelligence_->state()->create_move(
                    BoardPosition(previousMove->NewPieceX, previousMove->NewPieceY),
                    BoardPosition(previousMove->OldPieceX, previousMove->OldPieceY),
                    BoardPosition(previousMove->OldTileX, previousMove->OldTileY)
                    );

                intelligence_->apply_move(m, PLAYER_PLAYER2);
            }

            // Choose the best move.
            BoardMove newMove = intelligence_->choose_best_move(timeLimit, PLAYER_PLAYER1);
            
            // Convert unmanaged move to GC'd move.
            Karo::Common::Move^ gcMove = gcnew Move(newMove.target.x, newMove.target.y,
                newMove.piece ? newMove.piece->tile->position.x : 0, newMove.piece ? newMove.piece->tile->position.y : 0,
                newMove.tile ? newMove.tile->position.x : 0, newMove.tile ? newMove.tile->position.y : 0);

            // Apply our move to our internal board state.
            intelligence_->apply_move(newMove, PLAYER_PLAYER1);

            done->Invoke(gcMove);
        }

        int Player::Evaluate() {
            return intelligence_->evaluate(PLAYER_PLAYER1);
        }
    }
}