#include "AI.h"

namespace Karo {
	namespace AI {
		AI::AI()
		{
			BoardState = gcnew Karo::Core::Karo();
		}

		Karo::Common::Move^ AI::ChooseBestMove(Karo::Core::Karo^, int, Karo::Core::Player)
		{
			return gcnew Karo::Common::Move(1, 1, 0, 0, 0, 0);
		}

		void AI::DoMove(Karo::Common::Move^ previousMove, int timeLimit, Action<Karo::Common::Move^>^ done)
		{
			// add previous move
			BoardState = BoardState->WithMoveApplied(previousMove);

			Karo::Common::Move^ newMove = ChooseBestMove(BoardState, timeLimit, Karo::Core::Player::Player1);

			// apply our move
			BoardState = BoardState->WithMoveApplied(newMove);

			array<Karo::Common::Move^>^ moves = gcnew array< Karo::Common::Move^ > {newMove };



			done->Invoke(newMove);
		}
	}
}