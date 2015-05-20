#include "stdafx.h"
#include "CppUnitTest.h"
#include "Intelligence.h"
#include "BoardMove.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace KaroAITest
{		
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(ChooseBestMove)
		{
			Intelligence * intelligence = new Intelligence();

			BoardMove move = intelligence->choose_best_move(10, PLAYER_PLAYER1);

			Assert::AreEqual(move.target.x, 0);
		}

        TEST_METHOD(TempTest)
        {
            BoardState state;
            BoardMove * moves = new BoardMove[MOVE_COUNT];
            state.available_moves(PLAYER_PLAYER1, moves, MOVE_COUNT);
            state.apply_move(moves[0], PLAYER_PLAYER1);

            Assert::AreEqual(state.pieces()[0].player, PLAYER_PLAYER1);
            state.undo_move(moves[0], PLAYER_PLAYER1);
            Assert::AreEqual(state.pieces()[0].player, PLAYER_NONE);

        }

		TEST_METHOD(Test)
		{
			// TODO: Your test code here
		}

	};
}