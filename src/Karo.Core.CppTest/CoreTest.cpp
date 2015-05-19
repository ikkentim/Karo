#include "stdafx.h"
#include "CppUnitTest.h"
#include "BoardState.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace KaroCoreCppTest
{
	TEST_CLASS(CoreTest)
	{
	public:

		TEST_METHOD(GetAvailableMovesTestFreshBoard)
		{

			BoardState* board = new BoardState();
			BoardMove* moves = new BoardMove[MOVE_COUNT];

			int moveCount = board->available_moves(PLAYER_PLAYER2, moves, MOVE_COUNT);

			int count = 0;

			for (int i = 0; i < moveCount; i++) {
				if (&moves[i] != nullptr)
					count++;
			}
			Assert::AreEqual(20, count);

		}

		TEST_METHOD(GetCornerTilesTest0CornersAvailable) {
			BoardTile* tiles = new BoardTile[20];
			for (int i = 0; i < 20; i++) {
				tiles[i] = *new BoardTile(i % 5, i / 5);
			}

			BoardPiece* pieces = new BoardPiece[12];
			pieces[0] = *new BoardPiece(tiles[0 * 5 + 0], PLAYER_PLAYER1, false);
			pieces[1] = *new BoardPiece(tiles[3 * 5 + 0], PLAYER_PLAYER1, false);
			pieces[2] = *new BoardPiece(tiles[0 * 5 + 4], PLAYER_PLAYER1, false);
			pieces[3] = *new BoardPiece(tiles[3 * 5 + 4], PLAYER_PLAYER1, false);


			BoardState* karo = new BoardState(tiles, pieces);

			BoardTile* corners = new BoardTile[8];
			int i = 8;
			i = karo->corner_tiles(corners, i);

			Assert::AreEqual(0, i);
		}

		TEST_METHOD(GetCornerTilesTest2CornersAvailable) {
			BoardTile* tiles = new BoardTile[20];
			for (int i = 0; i < 20; i++) {
				tiles[i] = *new BoardTile(i % 5, i / 5);
			}

			BoardPiece* pieces = new BoardPiece[12];
			pieces[0] = *new BoardPiece(tiles[0 * 5 + 0], PLAYER_PLAYER1, false);
			pieces[1] = *new BoardPiece(tiles[2 * 5 + 0], PLAYER_PLAYER1, false);
			pieces[2] = *new BoardPiece(tiles[0 * 5 + 3], PLAYER_PLAYER1, false);
			pieces[3] = *new BoardPiece(tiles[3 * 5 + 4], PLAYER_PLAYER1, false);


			BoardState* karo = new BoardState(tiles, pieces);

			BoardTile* corners = new BoardTile[8];
			int i = 8;
			i = karo->corner_tiles(corners, i);

			Assert::AreEqual(2, i);
		}
		TEST_METHOD(GetCornerTilesTest1CornersAvailable) {
			BoardTile* tiles = new BoardTile[20];
			for (int i = 0; i < 20; i++) {
				tiles[i] = *new BoardTile(i % 5, i / 5);
			}

			BoardPiece* pieces = new BoardPiece[12];
			pieces[0] = *new BoardPiece(tiles[0 * 5 + 0], PLAYER_PLAYER1, false);
			pieces[1] = *new BoardPiece(tiles[2 * 5 + 0], PLAYER_PLAYER1, false);
			pieces[2] = *new BoardPiece(tiles[0 * 5 + 4], PLAYER_PLAYER1, false);
			pieces[3] = *new BoardPiece(tiles[3 * 5 + 4], PLAYER_PLAYER1, false);


			BoardState* karo = new BoardState(tiles, pieces);

			BoardTile* corners = new BoardTile[8];
			int i = 8;
			i = karo->corner_tiles(corners, i);

			Assert::AreEqual(1, i);
		}

		TEST_METHOD(GetAvailableMovesTest1CornerAvailable) {
			BoardTile* tiles = new BoardTile[20];
			for (int i = 0; i < 20; i++) {
				tiles[i] = *new BoardTile(i % 5, i / 5);
			}

			BoardPiece* pieces = new BoardPiece[12];

			pieces[0] = *new BoardPiece(tiles[0 * 5 + 0], PLAYER_PLAYER1, false);
			pieces[1] = *new BoardPiece(tiles[0 * 5 + 4], PLAYER_PLAYER1, false);
			pieces[2] = *new BoardPiece(tiles[0 * 5 + 3], PLAYER_PLAYER1, false);
			pieces[3] = *new BoardPiece(tiles[3 * 5 + 4], PLAYER_PLAYER1, false);
			pieces[4] = *new BoardPiece(tiles[0 * 5 + 1], PLAYER_PLAYER1, false);
			pieces[5] = *new BoardPiece(tiles[0 * 5 + 2], PLAYER_PLAYER1, false);

			pieces[6] = *new BoardPiece(tiles[1 * 5 + 0], PLAYER_PLAYER2, false);
			pieces[7] = *new BoardPiece(tiles[1 * 5 + 1], PLAYER_PLAYER2, false);
			pieces[8] = *new BoardPiece(tiles[1 * 5 + 2], PLAYER_PLAYER2, false);
			pieces[9] = *new BoardPiece(tiles[1 * 5 + 3], PLAYER_PLAYER2, false);
			pieces[10] = *new BoardPiece(tiles[1 * 5 + 4], PLAYER_PLAYER2, false);
			pieces[11] = *new BoardPiece(tiles[2 * 5 + 4], PLAYER_PLAYER2, false);

			BoardState* karo = new BoardState(tiles, pieces);

			BoardMove* moves = new BoardMove[MOVE_COUNT];
			int moveCount = karo->available_moves(PLAYER_PLAYER2, moves, MOVE_COUNT);

			int count = 0;

			for (int i = 0; i < moveCount; i++) {
				if (&moves[i] != nullptr)
					count++;
			}
			Assert::AreEqual(37, count);
		}
		TEST_METHOD(IsFinishedTestFreshBoard) {
			BoardTile* tiles = new BoardTile[20];
			for (int i = 0; i < 20; i++) {
				tiles[i] = *new BoardTile(i % 5, i / 5);
			}

			BoardPiece* pieces = new BoardPiece[12];

			BoardState* karo = new BoardState(tiles, pieces);

			Assert::AreEqual(false, karo->is_finished());
		}

		TEST_METHOD(IsFinishedTestHorizontalLine4) {
			BoardTile* tiles = new BoardTile[20];
			for (int i = 0; i < 20; i++) {
				tiles[i] = *new BoardTile(i % 5, i / 5);
			}

			BoardPiece* pieces = new BoardPiece[12];

			pieces[0] = *new BoardPiece(tiles[0 * 5 + 1], PLAYER_PLAYER1, true);
			pieces[1] = *new BoardPiece(tiles[1 * 5 + 1], PLAYER_PLAYER1, true);
			pieces[2] = *new BoardPiece(tiles[2 * 5 + 1], PLAYER_PLAYER1, true);
			pieces[3] = *new BoardPiece(tiles[3 * 5 + 1], PLAYER_PLAYER1, true);

			BoardState* karo = new BoardState(tiles, pieces);

			Assert::AreEqual(true, karo->is_finished());
		}
		TEST_METHOD(IsFinishedTestHorizontalLine3) {
			BoardTile* tiles = new BoardTile[20];
			for (int i = 0; i < 20; i++) {
				tiles[i] = *new BoardTile(i % 5, i / 5);
			}

			BoardPiece* pieces = new BoardPiece[12];

			pieces[0] = *new BoardPiece(tiles[0 * 5 + 1], PLAYER_PLAYER1, true);
			pieces[1] = *new BoardPiece(tiles[1 * 5 + 1], PLAYER_PLAYER1, true);
			pieces[2] = *new BoardPiece(tiles[2 * 5 + 1], PLAYER_PLAYER1, true);
			//pieces[3] = *new BoardPiece(tiles[3 * 5 + 1], PLAYER_PLAYER1, true);

			BoardState* karo = new BoardState(tiles, pieces);

			Assert::AreEqual(false, karo->is_finished());
		}

		TEST_METHOD(IsFinishedTestHorizontalLine3WithOpponent) {
			BoardTile* tiles = new BoardTile[20];
			for (int i = 0; i < 20; i++) {
				tiles[i] = *new BoardTile(i % 5, i / 5);
			}

			BoardPiece* pieces = new BoardPiece[12];

			pieces[0] = *new BoardPiece(tiles[0 * 5 + 1], PLAYER_PLAYER1, true);
			pieces[1] = *new BoardPiece(tiles[1 * 5 + 1], PLAYER_PLAYER1, true);
			pieces[2] = *new BoardPiece(tiles[2 * 5 + 1], PLAYER_PLAYER1, true);
			pieces[3] = *new BoardPiece(tiles[3 * 5 + 1], PLAYER_PLAYER2, true);

			BoardState* karo = new BoardState(tiles, pieces);

			Assert::AreEqual(false, karo->is_finished());
		}

		TEST_METHOD(IsFinishedTestHorizontalLine3WithUnmarked) {
			BoardTile* tiles = new BoardTile[20];
			for (int i = 0; i < 20; i++) {
				tiles[i] = *new BoardTile(i % 5, i / 5);
			}

			BoardPiece* pieces = new BoardPiece[12];

			pieces[0] = *new BoardPiece(tiles[0 * 5 + 1], PLAYER_PLAYER1, true);
			pieces[1] = *new BoardPiece(tiles[1 * 5 + 1], PLAYER_PLAYER1, true);
			pieces[2] = *new BoardPiece(tiles[2 * 5 + 1], PLAYER_PLAYER1, true);
			pieces[3] = *new BoardPiece(tiles[3 * 5 + 1], PLAYER_PLAYER2, false);

			BoardState* karo = new BoardState(tiles, pieces);

			Assert::AreEqual(false, karo->is_finished());
		}

		TEST_METHOD(IsFinishedTestDiagonalLine4) {
			BoardTile* tiles = new BoardTile[20];
			for (int i = 0; i < 20; i++) {
				tiles[i] = *new BoardTile(i % 5, i / 5);
			}

			BoardPiece* pieces = new BoardPiece[12];

			pieces[0] = *new BoardPiece(tiles[0 * 5 + 1], PLAYER_PLAYER1, true);
			pieces[1] = *new BoardPiece(tiles[1 * 5 + 2], PLAYER_PLAYER1, true);
			pieces[2] = *new BoardPiece(tiles[2 * 5 + 3], PLAYER_PLAYER1, true);
			pieces[3] = *new BoardPiece(tiles[3 * 5 + 4], PLAYER_PLAYER1, true);

			BoardState* karo = new BoardState(tiles, pieces);

			Assert::AreEqual(true, karo->is_finished());
		}

		TEST_METHOD(IsFinishedTestDiagonalLeftLine4) {
			BoardTile* tiles = new BoardTile[20];
			for (int i = 0; i < 20; i++) {
				tiles[i] = *new BoardTile(i % 5, i / 5);
			}

			BoardPiece* pieces = new BoardPiece[12];

			pieces[0] = *new BoardPiece(tiles[3 * 5 + 0], PLAYER_PLAYER1, true);
			pieces[1] = *new BoardPiece(tiles[2 * 5 + 1], PLAYER_PLAYER1, true);
			pieces[2] = *new BoardPiece(tiles[1 * 5 + 2], PLAYER_PLAYER1, true);
			pieces[3] = *new BoardPiece(tiles[0 * 5 + 3], PLAYER_PLAYER1, true);

			BoardState* karo = new BoardState(tiles, pieces);

			Assert::AreEqual(true, karo->is_finished());
		}

		TEST_METHOD(IsValidMoveTestFreeTile) {
			BoardTile* tiles = new BoardTile[20];
			for (int i = 0; i < 20; i++) {
				tiles[i] = *new BoardTile(i % 5, i / 5);
			}

			BoardPiece* pieces = new BoardPiece[12];
			pieces[0] = *new BoardPiece(tiles[2], PLAYER_PLAYER1, true);
			pieces[1] = *new BoardPiece(tiles[4], PLAYER_PLAYER2, true);
			pieces[2] = *new BoardPiece(tiles[5], PLAYER_PLAYER1, true);
			pieces[3] = *new BoardPiece(tiles[6], PLAYER_PLAYER2, true);
			pieces[4] = *new BoardPiece(tiles[8], PLAYER_PLAYER1, true);
			pieces[5] = *new BoardPiece(tiles[10], PLAYER_PLAYER2, true);
			pieces[6] = *new BoardPiece(tiles[11], PLAYER_PLAYER1, true);
			pieces[7] = *new BoardPiece(tiles[12], PLAYER_PLAYER2, true);
			pieces[8] = *new BoardPiece(tiles[14], PLAYER_PLAYER1, true);
			pieces[9] = *new BoardPiece(tiles[16], PLAYER_PLAYER2, true);
			pieces[10] = *new BoardPiece(tiles[17], PLAYER_PLAYER1, true);
			pieces[11] = *new BoardPiece(tiles[19], PLAYER_PLAYER2, true);

			BoardState* karo = new BoardState(tiles, pieces);

			BoardMove* move = new BoardMove(tiles[3], pieces[0]);
			Assert::AreEqual(true, karo->is_valid_move(*move));


		}
		TEST_METHOD(IsValidMoveJumping) {
			BoardTile* tiles = new BoardTile[20];
			for (int i = 0; i < 20; i++) {
				tiles[i] = *new BoardTile(i % 5, i / 5);
			}

			BoardPiece* pieces = new BoardPiece[12];
			pieces[0] = *new BoardPiece(tiles[2], PLAYER_PLAYER1, true);
			pieces[1] = *new BoardPiece(tiles[4], PLAYER_PLAYER2, true);
			pieces[2] = *new BoardPiece(tiles[5], PLAYER_PLAYER1, true);
			pieces[3] = *new BoardPiece(tiles[6], PLAYER_PLAYER2, true);
			pieces[4] = *new BoardPiece(tiles[8], PLAYER_PLAYER1, true);
			pieces[5] = *new BoardPiece(tiles[10], PLAYER_PLAYER2, true);
			pieces[6] = *new BoardPiece(tiles[11], PLAYER_PLAYER1, true);
			pieces[7] = *new BoardPiece(tiles[12], PLAYER_PLAYER2, true);
			pieces[8] = *new BoardPiece(tiles[14], PLAYER_PLAYER1, true);
			pieces[9] = *new BoardPiece(tiles[16], PLAYER_PLAYER2, true);
			pieces[10] = *new BoardPiece(tiles[17], PLAYER_PLAYER1, true);
			pieces[11] = *new BoardPiece(tiles[19], PLAYER_PLAYER2, true);

			BoardState* karo = new BoardState(tiles, pieces);

			// horizontal right
			BoardMove* move = new BoardMove(tiles[7], pieces[2]);
			Assert::AreEqual(true, karo->is_valid_move(*move));

			//horizontal left
			move = new BoardMove(tiles[15], pieces[10]);
			Assert::AreEqual(true, karo->is_valid_move(*move));

			//vertical down
			move = new BoardMove(tiles[15], pieces[2]);
			Assert::AreEqual(true, karo->is_valid_move(*move));

			//vertical up
			move = new BoardMove(tiles[1], pieces[6]);
			Assert::AreEqual(true, karo->is_valid_move(*move));

			//diag up-left
			//move = new BoardMove(*new BoardTile(4,-1), pieces[3], tiles[20]);
			//Assert::AreEqual(true, karo->is_valid_move(*move));

		}

		TEST_METHOD(GetWinnerHorizontal) {
			BoardTile* tiles = new BoardTile[20];
			for (int i = 0; i < 20; i++) {
				tiles[i] = *new BoardTile(i % 5, i / 5);
			}

			BoardPiece* pieces = new BoardPiece[12];
			pieces[0] = *new BoardPiece(tiles[2], PLAYER_PLAYER1, true);
			pieces[1] = *new BoardPiece(tiles[4], PLAYER_PLAYER2, true);
			pieces[2] = *new BoardPiece(tiles[5], PLAYER_PLAYER1, true);
			pieces[3] = *new BoardPiece(tiles[6], PLAYER_PLAYER1, true);
			pieces[4] = *new BoardPiece(tiles[7], PLAYER_PLAYER1, true);
			pieces[5] = *new BoardPiece(tiles[8], PLAYER_PLAYER1, true);
			pieces[6] = *new BoardPiece(tiles[9], PLAYER_PLAYER1, true);
			pieces[7] = *new BoardPiece(tiles[12], PLAYER_PLAYER2, true);
			pieces[8] = *new BoardPiece(tiles[14], PLAYER_PLAYER1, true);
			pieces[9] = *new BoardPiece(tiles[16], PLAYER_PLAYER2, true);
			pieces[10] = *new BoardPiece(tiles[17], PLAYER_PLAYER1, true);
			pieces[11] = *new BoardPiece(tiles[19], PLAYER_PLAYER2, true);

			BoardState* karo = new BoardState(tiles, pieces);

			Assert::AreEqual(PLAYER_PLAYER1, karo->winner());
		}

		TEST_METHOD(GetWinnerVertical) {
			BoardTile* tiles = new BoardTile[20];
			for (int i = 0; i < 20; i++) {
				tiles[i] = *new BoardTile(i % 5, i / 5);
			}

			BoardPiece* pieces = new BoardPiece[12];
			pieces[0] = *new BoardPiece(tiles[0], PLAYER_PLAYER1, true);
			pieces[1] = *new BoardPiece(tiles[5], PLAYER_PLAYER1, true);
			pieces[2] = *new BoardPiece(tiles[10], PLAYER_PLAYER1, true);
			pieces[3] = *new BoardPiece(tiles[15], PLAYER_PLAYER1, true);
			pieces[4] = *new BoardPiece(tiles[4], PLAYER_PLAYER1, true);
			pieces[5] = *new BoardPiece(tiles[12], PLAYER_PLAYER1, true);
			pieces[6] = *new BoardPiece(tiles[6], PLAYER_PLAYER2, true);
			pieces[7] = *new BoardPiece(tiles[7], PLAYER_PLAYER2, true);
			pieces[8] = *new BoardPiece(tiles[8], PLAYER_PLAYER2, true);
			pieces[9] = *new BoardPiece(tiles[9], PLAYER_PLAYER2, true);
			pieces[10] = *new BoardPiece(tiles[13], PLAYER_PLAYER2, true);
			pieces[11] = *new BoardPiece(tiles[11], PLAYER_PLAYER2, true);

			BoardState* karo = new BoardState(tiles, pieces);

			Assert::AreEqual(PLAYER_PLAYER1, karo->winner());

		}
		TEST_METHOD(GetWinnerDiagonal) {
			BoardTile* tiles = new BoardTile[20];
			for (int i = 0; i < 20; i++) {
				tiles[i] = *new BoardTile(i % 5, i / 5);
			}

			BoardPiece* pieces = new BoardPiece[12];
			pieces[0] = *new BoardPiece(tiles[0], PLAYER_PLAYER1, true);
			pieces[1] = *new BoardPiece(tiles[6], PLAYER_PLAYER1, true);
			pieces[2] = *new BoardPiece(tiles[12], PLAYER_PLAYER1, true);
			pieces[3] = *new BoardPiece(tiles[18], PLAYER_PLAYER1, true);
			pieces[4] = *new BoardPiece(tiles[4], PLAYER_PLAYER1, true);
			pieces[5] = *new BoardPiece(tiles[5], PLAYER_PLAYER1, true);
			pieces[6] = *new BoardPiece(tiles[13], PLAYER_PLAYER2, true);
			pieces[7] = *new BoardPiece(tiles[7], PLAYER_PLAYER2, true);
			pieces[8] = *new BoardPiece(tiles[8], PLAYER_PLAYER2, true);
			pieces[9] = *new BoardPiece(tiles[14], PLAYER_PLAYER2, true);
			pieces[10] = *new BoardPiece(tiles[10], PLAYER_PLAYER2, true);
			pieces[11] = *new BoardPiece(tiles[11], PLAYER_PLAYER2, true);

			BoardState* karo = new BoardState(tiles, pieces);

			Assert::AreEqual(PLAYER_PLAYER1, karo->winner());
		}

		TEST_METHOD(GetWinnerDiagonalNoWinner) {
			BoardTile* tiles = new BoardTile[20];
			for (int i = 0; i < 20; i++) {
				tiles[i] = *new BoardTile(i % 5, i / 5);
			}

			BoardPiece* pieces = new BoardPiece[12];
			pieces[0] = *new BoardPiece(tiles[0], PLAYER_PLAYER1, true);
			pieces[1] = *new BoardPiece(tiles[6], PLAYER_PLAYER2, true);
			pieces[2] = *new BoardPiece(tiles[12], PLAYER_PLAYER1, true);
			pieces[3] = *new BoardPiece(tiles[18], PLAYER_PLAYER1, true);
			pieces[4] = *new BoardPiece(tiles[4], PLAYER_PLAYER1, true);
			pieces[5] = *new BoardPiece(tiles[5], PLAYER_PLAYER1, true);
			pieces[6] = *new BoardPiece(tiles[13], PLAYER_PLAYER2, true);
			pieces[7] = *new BoardPiece(tiles[7], PLAYER_PLAYER2, true);
			pieces[8] = *new BoardPiece(tiles[8], PLAYER_PLAYER2, true);
			pieces[9] = *new BoardPiece(tiles[9], PLAYER_PLAYER1, true);
			pieces[10] = *new BoardPiece(tiles[10], PLAYER_PLAYER2, true);
			pieces[11] = *new BoardPiece(tiles[11], PLAYER_PLAYER2, true);

			BoardState* karo = new BoardState(tiles, pieces);

			Assert::AreEqual(PLAYER_NONE, karo->winner());
		}
	};
}