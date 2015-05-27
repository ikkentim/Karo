#include "stdafx.h"
#include "CppUnitTest.h"
#include "BoardState.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define GENERATE_TILES(a) BoardTile a[20]; \
    for (int i = 0; i < 20; i++) { \
        a[i] = BoardTile(i % 5, i / 5); }

#define PICK_PIECE(pa, ta, _i, _x, _y, _p) for(int i=0;i<20;i++){ \
    if(ta[i].position.x == _x && ta[i].position.y == _y){ \
    pa[_i] =BoardPiece(&ta[i], _p, false); } }

#define PICK_PIECE_UP(pa, ta, _i, _x, _y, _p) for(int i=0;i<20;i++){ \
    if(ta[i].position.x == _x && ta[i].position.y == _y){ \
    pa[_i] =BoardPiece(&ta[i], _p, true); } }

namespace KaroCoreCppTest
{
	TEST_CLASS(CoreTest)
	{
	public:

		TEST_METHOD(GetAvailableMovesTestFreshBoard)
		{
			BoardState board = BoardState();
			BoardMove moves[MOVE_COUNT];

            Assert::AreEqual(20, board.available_moves(PLAYER_PLAYER2, moves, MOVE_COUNT));
		}

		TEST_METHOD(GetCornerTilesTest0CornersAvailable) {
            GENERATE_TILES(tiles);

            BoardPiece pieces[12];
            PICK_PIECE(pieces, tiles, 0, 0, 0, PLAYER_PLAYER1);
            PICK_PIECE(pieces, tiles, 1, 0, 3, PLAYER_PLAYER1);
            PICK_PIECE(pieces, tiles, 2, 4, 0, PLAYER_PLAYER1);
            PICK_PIECE(pieces, tiles, 3, 4, 3, PLAYER_PLAYER1);

			BoardState karo = BoardState(tiles, pieces);

            Assert::AreEqual(0, karo.corner_tiles(NULL, 0));
		}

		TEST_METHOD(GetCornerTilesTest2CornersAvailable) {
            GENERATE_TILES(tiles);

            BoardPiece pieces[12];
            PICK_PIECE(pieces, tiles, 0, 0, 0, PLAYER_PLAYER1);
            PICK_PIECE(pieces, tiles, 1, 0, 2, PLAYER_PLAYER1);
            PICK_PIECE(pieces, tiles, 2, 3, 0, PLAYER_PLAYER1);
            PICK_PIECE(pieces, tiles, 3, 4, 3, PLAYER_PLAYER1);

            BoardState karo = BoardState(tiles, pieces);

            Assert::AreEqual(2, karo.corner_tiles(NULL, 0));
		}

		TEST_METHOD(GetCornerTilesTest1CornersAvailable) {
            GENERATE_TILES(tiles);

            BoardPiece pieces[12];
            PICK_PIECE(pieces, tiles, 0, 0, 0, PLAYER_PLAYER1);
            PICK_PIECE(pieces, tiles, 1, 0, 2, PLAYER_PLAYER1);
            PICK_PIECE(pieces, tiles, 2, 4, 0, PLAYER_PLAYER1);
            PICK_PIECE(pieces, tiles, 3, 4, 3, PLAYER_PLAYER1);

            BoardState karo = BoardState(tiles, pieces);

            Assert::AreEqual(1, karo.corner_tiles(NULL, 0));
		}

		TEST_METHOD(GetAvailableMovesTest1CornerAvailable) {
            GENERATE_TILES(tiles);

            BoardPiece pieces[12];
            PICK_PIECE(pieces, tiles, 0, 0, 0, PLAYER_PLAYER1);
            PICK_PIECE(pieces, tiles, 1, 4, 0, PLAYER_PLAYER1);
            PICK_PIECE(pieces, tiles, 2, 3, 0, PLAYER_PLAYER1);
            PICK_PIECE(pieces, tiles, 3, 4, 3, PLAYER_PLAYER1);
            PICK_PIECE(pieces, tiles, 4, 1, 0, PLAYER_PLAYER1);
            PICK_PIECE(pieces, tiles, 5, 2, 0, PLAYER_PLAYER1);

            PICK_PIECE(pieces, tiles, 6, 0, 1, PLAYER_PLAYER1);
            PICK_PIECE(pieces, tiles, 7, 2, 1, PLAYER_PLAYER1);
            PICK_PIECE(pieces, tiles, 8, 3, 1, PLAYER_PLAYER1);
            PICK_PIECE(pieces, tiles, 9, 4, 1, PLAYER_PLAYER1);
            PICK_PIECE(pieces, tiles, 10, 5, 1, PLAYER_PLAYER1);
            PICK_PIECE(pieces, tiles, 11, 5, 2, PLAYER_PLAYER1);

            BoardState karo = BoardState(tiles, pieces);

			BoardMove* moves = new BoardMove[MOVE_COUNT];
			int count = karo.available_moves(PLAYER_PLAYER2, moves, MOVE_COUNT);

			Assert::AreEqual(37, count);
		}
		TEST_METHOD(IsFinishedTestFreshBoard) {
            GENERATE_TILES(tiles);

            BoardPiece pieces[12];
            BoardState karo = BoardState(tiles, pieces);

			Assert::AreEqual(false, karo.is_finished());
		}

		TEST_METHOD(IsFinishedTestHorizontalLine4) {
            GENERATE_TILES(tiles);

            BoardPiece pieces[12];
            PICK_PIECE_UP(pieces, tiles, 0, 1, 0, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 1, 1, 1, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 2, 1, 2, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 3, 1, 3, PLAYER_PLAYER1);

            BoardState karo = BoardState(tiles, pieces);

			Assert::AreEqual(true, karo.is_finished());
		}
		TEST_METHOD(IsFinishedTestHorizontalLine3) {
            GENERATE_TILES(tiles);

            BoardPiece pieces[12];
            PICK_PIECE_UP(pieces, tiles, 0, 1, 0, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 1, 1, 1, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 2, 1, 2, PLAYER_PLAYER1);

            BoardState karo = BoardState(tiles, pieces);

			Assert::AreEqual(false, karo.is_finished());
		}

		TEST_METHOD(IsFinishedTestHorizontalLine3WithOpponent) {
            GENERATE_TILES(tiles);

            BoardPiece pieces[12];
            PICK_PIECE_UP(pieces, tiles, 0, 1, 0, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 1, 1, 1, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 2, 1, 2, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 3, 1, 3, PLAYER_PLAYER1);

            BoardState karo = BoardState(tiles, pieces);
 
			Assert::AreEqual(false, karo.is_finished());
		}

		TEST_METHOD(IsFinishedTestHorizontalLine3WithUnmarked) {
            GENERATE_TILES(tiles);

            BoardPiece pieces[12];
            PICK_PIECE_UP(pieces, tiles, 0, 1, 0, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 1, 1, 1, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 2, 1, 2, PLAYER_PLAYER1);
            PICK_PIECE(   pieces, tiles, 3, 1, 3, PLAYER_PLAYER1);

            BoardState karo = BoardState(tiles, pieces);

			Assert::AreEqual(false, karo.is_finished());
		}

		TEST_METHOD(IsFinishedTestDiagonalLine4) {
            GENERATE_TILES(tiles);

            BoardPiece pieces[12];
            PICK_PIECE_UP(pieces, tiles, 0, 0, 0, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 1, 1, 1, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 2, 2, 2, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 3, 3, 3, PLAYER_PLAYER1);

            BoardState karo = BoardState(tiles, pieces);

			Assert::AreEqual(true, karo.is_finished());
		}

		TEST_METHOD(IsFinishedTestDiagonalLeftLine4) {
            GENERATE_TILES(tiles);

            BoardPiece pieces[12];
            PICK_PIECE_UP(pieces, tiles, 0, 0, 3, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 1, 1, 2, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 2, 2, 1, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 3, 3, 0, PLAYER_PLAYER1);

            BoardState karo = BoardState(tiles, pieces);

			Assert::AreEqual(true, karo.is_finished());
		}

		TEST_METHOD(IsValidMoveTestFreeTile) {
            GENERATE_TILES(tiles);

            BoardPiece pieces[12];
            PICK_PIECE_UP(pieces, tiles, 0, 0, 0, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 1, 2, 0, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 2, 4, 0, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 3, 0, 1, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 4, 2, 1, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 5, 4, 1, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 6, 0, 2, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 7, 2, 2, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 8, 4, 2, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 9, 0, 3, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 10, 2, 3, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 11, 4, 3, PLAYER_PLAYER2);

            BoardState karo = BoardState(tiles, pieces);

            BoardMove move = karo.create_move(BoardPosition(1,0), BoardPosition(0,0), BoardPosition());
			Assert::AreEqual(true, karo.is_valid_move(move));
		}

		TEST_METHOD(IsValidMoveJumping) {
            GENERATE_TILES(tiles);

            BoardPiece pieces[12];
            PICK_PIECE_UP(pieces, tiles, 0, 0, 0, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 1, 2, 0, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 2, 4, 0, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 3, 0, 1, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 4, 2, 1, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 5, 4, 1, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 6, 0, 2, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 7, 2, 2, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 8, 4, 2, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 9, 0, 3, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 10, 2, 3, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 11, 4, 3, PLAYER_PLAYER2);

            BoardState karo = BoardState(tiles, pieces);

			// horizontal right
            BoardMove move = karo.create_move(tiles[7].position, pieces[2].tile->position, BoardPosition());
			Assert::AreEqual(true, karo.is_valid_move(move));

			//horizontal left
            move = karo.create_move(tiles[15].position, pieces[10].tile->position, BoardPosition());
			Assert::AreEqual(true, karo.is_valid_move(move));

			//vertical down
            move = karo.create_move(tiles[15].position, pieces[2].tile->position, BoardPosition());
			Assert::AreEqual(true, karo.is_valid_move(move));

			//vertical up
            move = karo.create_move(tiles[1].position, pieces[6].tile->position, BoardPosition());
			Assert::AreEqual(true, karo.is_valid_move(move));
		}

		TEST_METHOD(GetWinnerHorizontal) {
            GENERATE_TILES(tiles);

            BoardPiece pieces[12];
            PICK_PIECE_UP(pieces, tiles, 0, 0, 0, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 1, 2, 0, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 2, 0, 1, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 3, 1, 1, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 4, 2, 1, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 5, 3, 1, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 6, 0, 2, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 7, 2, 2, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 8, 4, 2, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 9, 0, 3, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 10, 2, 3, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 11, 4, 3, PLAYER_PLAYER2);

            BoardState karo = BoardState(tiles, pieces);

			Assert::AreEqual(PLAYER_PLAYER1, karo.winner());
		}

		TEST_METHOD(GetWinnerVertical) {
            GENERATE_TILES(tiles);

            BoardPiece pieces[12];
            PICK_PIECE_UP(pieces, tiles, 0, 0, 0, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 1, 2, 0, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 2, 1, 0, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 3, 1, 1, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 4, 1, 2, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 5, 1, 3, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 6, 0, 2, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 7, 2, 2, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 8, 4, 2, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 9, 0, 3, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 10, 2, 3, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 11, 4, 3, PLAYER_PLAYER2);

            BoardState karo = BoardState(tiles, pieces);

			Assert::AreEqual(PLAYER_PLAYER1, karo.winner());

		}
		TEST_METHOD(GetWinnerDiagonal) {
            GENERATE_TILES(tiles);

            BoardPiece pieces[12];
            PICK_PIECE_UP(pieces, tiles, 0, 0, 0, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 1, 1, 1, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 2, 2, 2, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 3, 3, 3, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 4, 2, 1, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 5, 4, 1, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 6, 0, 2, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 7, 3, 2, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 8, 4, 2, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 9, 0, 3, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 10, 2, 3, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 11, 4, 3, PLAYER_PLAYER2);

            BoardState karo = BoardState(tiles, pieces);

			Assert::AreEqual(PLAYER_PLAYER1, karo.winner());
		}

		TEST_METHOD(GetWinnerDiagonalNoWinner) {
            GENERATE_TILES(tiles);

            BoardPiece pieces[12];
            PICK_PIECE_UP(pieces, tiles, 0, 0, 0, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 1, 2, 0, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 2, 4, 0, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 3, 0, 1, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 4, 2, 1, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 5, 4, 1, PLAYER_PLAYER1);
            PICK_PIECE_UP(pieces, tiles, 6, 0, 2, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 7, 2, 2, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 8, 4, 2, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 9, 0, 3, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 10, 2, 3, PLAYER_PLAYER2);
            PICK_PIECE_UP(pieces, tiles, 11, 4, 3, PLAYER_PLAYER2);

            BoardState karo = BoardState(tiles, pieces);

			Assert::AreEqual(PLAYER_NONE, karo.winner());
		}
	};
}