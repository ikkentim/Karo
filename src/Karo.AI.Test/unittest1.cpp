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

		TEST_METHOD(Test)
		{
			// TODO: Your test code here
		}

	};
}