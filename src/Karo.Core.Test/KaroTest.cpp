#include "stdafx.h"

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;
using namespace Karo::Core;
using namespace Karo::Common;
namespace KaroCoreTest
{
	[TestClass]
	public ref class KaroTest
	{
	private:
		TestContext^ testContextInstance;

	public:
		/// <summary>
		///Gets or sets the test context which provides
		///information about and functionality for the current test run.
		///</summary>
		property Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ TestContext
		{
			Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ get()
			{
				return testContextInstance;
			}
			System::Void set(Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ value)
			{
				testContextInstance = value;
			}
		};

#pragma region Additional test attributes
		//
		//You can use the following additional attributes as you write your tests:
		//
		//Use ClassInitialize to run code before running the first test in the class
		//[ClassInitialize()]
		//static void MyClassInitialize(TestContext^ testContext) {};
		//
		//Use ClassCleanup to run code after all tests in a class have run
		//[ClassCleanup()]
		//static void MyClassCleanup() {};
		//
		//Use TestInitialize to run code before running each test
		//[TestInitialize()]
		//void MyTestInitialize() {};
		//
		//Use TestCleanup to run code after each test has run
		//[TestCleanup()]
		//void MyTestCleanup() {};
		//
#pragma endregion 

		[TestMethod]
		void GetAvailableMovesTestFreshBoard()
		{
			auto karo = gcnew KaroBoardState();

			auto moves = karo->GetAvailableMoves(KaroPlayer::Player1);

			int i = 0;

			for each (Move^ move in moves)
			{
				if (move != nullptr)
					i++;
			}

			Assert::AreEqual(20, i);
		};

        [TestMethod]
        void GetCornerTilesTest0CornersAvailable()
        {
            auto tiles = gcnew array<Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Piece^>(12);

            pieces[0] = gcnew Piece(tiles[0 * 5 + 0], KaroPlayer::Player1, false);
            pieces[1] = gcnew Piece(tiles[3 * 5 + 0], KaroPlayer::Player1, false);
            pieces[2] = gcnew Piece(tiles[0 * 5 + 4], KaroPlayer::Player1, false);
            pieces[3] = gcnew Piece(tiles[3 * 5 + 4], KaroPlayer::Player1, false);
            auto karo = gcnew KaroBoardState(tiles, pieces);

            auto corners = karo->CornerTiles;

            Assert::AreEqual(0, System::Linq::Enumerable::Count(corners));
        };

        [TestMethod]
        void GetCornerTilesTest2CornersAvailable()
        {
            auto tiles = gcnew array<Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Tile(i % 5, i / 5);
            }

            tiles[2]->X = 5;
            tiles[2]->Y = 0;

            auto pieces = gcnew array<Piece^>(12);

            pieces[0] = gcnew Piece(tiles[0 * 5 + 0], KaroPlayer::Player1, false);
            pieces[1] = gcnew Piece(tiles[3 * 5 + 0], KaroPlayer::Player1, false);
            pieces[2] = gcnew Piece(tiles[2], KaroPlayer::Player1, false);
            pieces[3] = gcnew Piece(tiles[3 * 5 + 4], KaroPlayer::Player1, false);
            auto karo = gcnew KaroBoardState(tiles, pieces);

            auto corners = karo->CornerTiles;

            for each(auto c in corners) {
                Console::WriteLine(c);
            }
            Assert::AreEqual(2, System::Linq::Enumerable::Count(corners));
        };

        [TestMethod]
        void GetCornerTilesTest1CornerAvailable()
        {
            auto tiles = gcnew array<Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Piece^>(12);

            pieces[0] = gcnew Piece(tiles[0 * 5 + 0], KaroPlayer::Player1, false);
            pieces[1] = gcnew Piece(tiles[3 * 5 + 0], KaroPlayer::Player1, false);
            pieces[2] = gcnew Piece(tiles[0 * 5 + 4], KaroPlayer::Player1, false);
            auto karo = gcnew KaroBoardState(tiles, pieces);

            auto corners = karo->CornerTiles;

            Assert::AreEqual(1, System::Linq::Enumerable::Count(corners));
        };

        [TestMethod]
        void GetAvailableMovesTestCornersUnavailable()
        {
            auto tiles = gcnew array<Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Piece^>(12);

            // Player1 fills the corners
            pieces[0] = gcnew Piece(tiles[0 * 5 + 0], KaroPlayer::Player1, false);
            pieces[1] = gcnew Piece(tiles[0 * 5 + 4], KaroPlayer::Player1, false);
            pieces[2] = gcnew Piece(tiles[3 * 5 + 0], KaroPlayer::Player1, false);
            pieces[3] = gcnew Piece(tiles[3 * 5 + 4], KaroPlayer::Player1, false);
            pieces[4] = gcnew Piece(tiles[0 * 5 + 1], KaroPlayer::Player1, false);
            pieces[5] = gcnew Piece(tiles[0 * 5 + 2], KaroPlayer::Player1, false);

            // It's Player2's turn
            pieces[6] = gcnew Piece(tiles[1 * 5 + 0], KaroPlayer::Player2, false);
            pieces[7] = gcnew Piece(tiles[1 * 5 + 1], KaroPlayer::Player2, false);
            pieces[8] = gcnew Piece(tiles[1 * 5 + 2], KaroPlayer::Player2, false);
            pieces[9] = gcnew Piece(tiles[1 * 5 + 3], KaroPlayer::Player2, false);
            pieces[10] = gcnew Piece(tiles[1 * 5 + 4], KaroPlayer::Player2, false);
            pieces[11] = gcnew Piece(tiles[2 * 5 + 4], KaroPlayer::Player2, false);
            auto karo = gcnew KaroBoardState(tiles, pieces);

            auto moves = karo->GetAvailableMoves(KaroPlayer::Player2);

            /* Board setup:
             * P1: X P2:I
             * X X X _ X
             * I I I I I
             * _ _ _ _ I
             * X _ _ _ X
             * 2 3 4 3 4 == 16
             */
            Assert::AreEqual(16, System::Linq::Enumerable::Count(moves));
        };

        [TestMethod]
        void GetAvailableMovesTest1CornerAvailable()
        {
            auto tiles = gcnew array<Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Piece^>(12);

            // Player1 fills the corners
            pieces[0] = gcnew Piece(tiles[0 * 5 + 0], KaroPlayer::Player1, false);
            pieces[1] = gcnew Piece(tiles[0 * 5 + 4], KaroPlayer::Player1, false);
            pieces[2] = gcnew Piece(tiles[0 * 5 + 3], KaroPlayer::Player1, false);
            pieces[3] = gcnew Piece(tiles[3 * 5 + 4], KaroPlayer::Player1, false);
            pieces[4] = gcnew Piece(tiles[0 * 5 + 1], KaroPlayer::Player1, false);
            pieces[5] = gcnew Piece(tiles[0 * 5 + 2], KaroPlayer::Player1, false);

            // It's Player2's turn
            pieces[6] = gcnew Piece(tiles[1 * 5 + 0], KaroPlayer::Player2, false);
            pieces[7] = gcnew Piece(tiles[1 * 5 + 1], KaroPlayer::Player2, false);
            pieces[8] = gcnew Piece(tiles[1 * 5 + 2], KaroPlayer::Player2, false);
            pieces[9] = gcnew Piece(tiles[1 * 5 + 3], KaroPlayer::Player2, false);
            pieces[10] = gcnew Piece(tiles[1 * 5 + 4], KaroPlayer::Player2, false);
            pieces[11] = gcnew Piece(tiles[2 * 5 + 4], KaroPlayer::Player2, false);
            auto karo = gcnew KaroBoardState(tiles, pieces);

            auto moves = karo->GetAvailableMoves(KaroPlayer::Player2);

            /* Board setup:
             * P1: X P2:I
             * X X X X X
             * I I I I I
             * _ _ _ _ I
             * _ _ _ _ X
             * 7 6 6 6 12 == 37
             */
            Assert::AreEqual(37, System::Linq::Enumerable::Count(moves));
        };

        [TestMethod]
        void IsFinishedTestFreshBoard()
        {
            auto tiles = gcnew array<Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Piece^>(12);

            auto karo = gcnew KaroBoardState(tiles, pieces);

            Assert::AreEqual(false, karo->IsFinished);
        };

        [TestMethod]
        void IsFinishedTestHorizontalLine4()
        {
            auto tiles = gcnew array<Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Piece^>(12);
            pieces[0] = gcnew Piece(tiles[0 * 5 + 1], KaroPlayer::Player1, true);
            pieces[1] = gcnew Piece(tiles[1 * 5 + 1], KaroPlayer::Player1, true);
            pieces[2] = gcnew Piece(tiles[2 * 5 + 1], KaroPlayer::Player1, true);
            pieces[3] = gcnew Piece(tiles[3 * 5 + 1], KaroPlayer::Player1, true);

            auto karo = gcnew KaroBoardState(tiles, pieces);

            Assert::AreEqual(true, karo->IsFinished);
        };

        [TestMethod]
        void IsFinishedTestHorizontalLine3()
        {
            auto tiles = gcnew array<Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Piece^>(12);
            pieces[0] = gcnew Piece(tiles[1 * 5 + 1], KaroPlayer::Player1, true);
            pieces[1] = gcnew Piece(tiles[2 * 5 + 1], KaroPlayer::Player1, true);
            pieces[2] = gcnew Piece(tiles[3 * 5 + 1], KaroPlayer::Player1, true);
            //pieces[3] = gcnew Piece(tiles[5 * 5 + 1], KaroPlayer::Player1, true);

            auto karo = gcnew KaroBoardState(tiles, pieces);

            Assert::AreEqual(false, karo->IsFinished);
        };

        [TestMethod]
        void IsFinishedTestHorizontalLine3WithOppenent()
        {
            auto tiles = gcnew array<Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Piece^>(12);
            pieces[0] = gcnew Piece(tiles[0 * 5 + 1], KaroPlayer::Player1, true);
            pieces[1] = gcnew Piece(tiles[1 * 5 + 1], KaroPlayer::Player1, true);
            pieces[2] = gcnew Piece(tiles[2 * 5 + 1], KaroPlayer::Player1, true);
            pieces[3] = gcnew Piece(tiles[3 * 5 + 1], KaroPlayer::Player2, true);

            auto karo = gcnew KaroBoardState(tiles, pieces);

            Assert::AreEqual(false, karo->IsFinished);
        };

        [TestMethod]
        void IsFinishedTestHorizontalLine3WithUpsideDown()
        {
            auto tiles = gcnew array<Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Piece^>(12);
            pieces[0] = gcnew Piece(tiles[0 * 5 + 1], KaroPlayer::Player1, true);
            pieces[1] = gcnew Piece(tiles[1 * 5 + 1], KaroPlayer::Player1, true);
            pieces[2] = gcnew Piece(tiles[2 * 5 + 1], KaroPlayer::Player1, true);
            pieces[3] = gcnew Piece(tiles[3 * 5 + 1], KaroPlayer::Player1, false);

            auto karo = gcnew KaroBoardState(tiles, pieces);

            Assert::AreEqual(false, karo->IsFinished);
        };
        [TestMethod]
        void IsFinishedTestDiagonalRightLine4()
        {
            auto tiles = gcnew array<Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Piece^>(12);
            pieces[0] = gcnew Piece(tiles[0 * 5 + 1], KaroPlayer::Player1, true);
            pieces[1] = gcnew Piece(tiles[1 * 5 + 2], KaroPlayer::Player1, true);
            pieces[2] = gcnew Piece(tiles[2 * 5 + 3], KaroPlayer::Player1, true);
            pieces[3] = gcnew Piece(tiles[3 * 5 + 4], KaroPlayer::Player1, true);

            auto karo = gcnew KaroBoardState(tiles, pieces);

            Assert::AreEqual(true, karo->IsFinished);
        };

        [TestMethod]
        void IsFinishedTestDiagonalLeftLine4()
        {
            auto tiles = gcnew array<Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Piece^>(12);
            pieces[0] = gcnew Piece(tiles[3 * 5 + 0], KaroPlayer::Player1, true);
            pieces[1] = gcnew Piece(tiles[2 * 5 + 1], KaroPlayer::Player1, true);
            pieces[2] = gcnew Piece(tiles[1 * 5 + 2], KaroPlayer::Player1, true);
            pieces[3] = gcnew Piece(tiles[0 * 5 + 3], KaroPlayer::Player1, true);

            auto karo = gcnew KaroBoardState(tiles, pieces);

            Assert::AreEqual(true, karo->IsFinished);
        };

		[TestMethod]
		void IsValidMoveTestSameLocation()
		{
			//Should not be possible to move to the current location
			auto tiles = gcnew array<Tile^>(20);
			for (int i = 0; i < 20; i++) {
				tiles[i] = gcnew Tile(i % 5, i / 5);
			}

			auto pieces = gcnew array<Piece^>(12);
			pieces[0] = gcnew Piece(tiles[1 * 5 + 1], KaroPlayer::Player1, true);

			auto karo = gcnew KaroBoardState(tiles, pieces);
			//Move from (1,1) to (1,1) [false]
			auto move = gcnew Karo::Common::Move(2, 0, 2, 0, 2, 0);
			Assert::AreEqual(false, karo->IsValidMove(move));

			//Move from (1,1) to (1,2) [true]
			move = gcnew Karo::Common::Move(1, 0, 2, 0, 2, 0);
			Assert::AreEqual(true, karo->IsValidMove(move));
		}

		[TestMethod]
		void IsValidMoveTestFreeTile()
		{
			auto tiles = gcnew array<Tile^>(20);
			for (int i = 0; i < 20; i++) {
				tiles[i] = gcnew Tile(i % 5, i / 5);
			}

			auto pieces = gcnew array<Piece^>(12);
			pieces[0] = gcnew Piece(tiles[0], KaroPlayer::Player1, true);
			pieces[1] = gcnew Piece(tiles[1], KaroPlayer::Player2, true);
			pieces[2] = gcnew Piece(tiles[2], KaroPlayer::Player1, true);
			pieces[3] = gcnew Piece(tiles[3], KaroPlayer::Player2, true);
			pieces[4] = gcnew Piece(tiles[6], KaroPlayer::Player2, true);

			auto karo = gcnew KaroBoardState(tiles, pieces);
			//Move to location without piece [true]
			auto move = gcnew Karo::Common::Move(0, 0, 0, 1, 0, 1);
			Assert::AreEqual(true, karo->IsValidMove(move));
			//Move to location with piece [false]
			move = gcnew Karo::Common::Move(3, 1, 2, 0, 2, 0);
			Assert::AreEqual(false, karo->IsValidMove(move));
		}

		[TestMethod]
		void IsValidMoveTestValidMoveJumping()
		{
			//A piece can only move 1 square, unless it 'jumps' over another piece.
			//Todo:: Implement tests and code
			auto tiles = gcnew array<Karo::Core::Tile^>(20);
			for (int i = 0; i < 20; i++) {
				tiles[i] = gcnew Karo::Core::Tile(i % 5, i / 5);
			}

			//Place pieces
			auto pieces = gcnew array<Karo::Core::Piece^>(12);
			pieces[0] = gcnew Karo::Core::Piece(tiles[2], KaroPlayer::Player1, true);
			pieces[1] = gcnew Karo::Core::Piece(tiles[4], KaroPlayer::Player2, true);
			pieces[2] = gcnew Karo::Core::Piece(tiles[5], KaroPlayer::Player1, true);
			pieces[3] = gcnew Karo::Core::Piece(tiles[6], KaroPlayer::Player2, true);
			pieces[4] = gcnew Karo::Core::Piece(tiles[8], KaroPlayer::Player1, true);
			pieces[5] = gcnew Karo::Core::Piece(tiles[10], KaroPlayer::Player2, true);
			pieces[6] = gcnew Karo::Core::Piece(tiles[11], KaroPlayer::Player1, true);
			pieces[7] = gcnew Karo::Core::Piece(tiles[12], KaroPlayer::Player2, true);
			pieces[8] = gcnew Karo::Core::Piece(tiles[14], KaroPlayer::Player1, true);
			pieces[9] = gcnew Karo::Core::Piece(tiles[16], KaroPlayer::Player2, true);
			pieces[10] = gcnew Karo::Core::Piece(tiles[17], KaroPlayer::Player1, true);
			pieces[11] = gcnew Karo::Core::Piece(tiles[19], KaroPlayer::Player2, true);

			auto karo = gcnew KaroBoardState(tiles, pieces);
			//Test Jumping horizontal right
			auto move = gcnew Karo::Common::Move(3, 2, 1, 2, 1, 2);
			Assert::AreEqual(true, karo->IsValidMove(move));

			//Test Jumping horizontal left
			move = gcnew Karo::Common::Move(0, 3, 2, 3, 2, 3);
			Assert::AreEqual(true, karo->IsValidMove(move));

			//Test Jumping vertical up
			move = gcnew Karo::Common::Move(1, 0, 1, 2, 1, 2);
			Assert::AreEqual(true, karo->IsValidMove(move));

			//Test Jumping vertical down
			move = gcnew Karo::Common::Move(0, 3, 0, 1, 0, 1);
			Assert::AreEqual(true, karo->IsValidMove(move));

			//Test Jumping diagnal up-right
			move = gcnew Karo::Common::Move(3, -1, 1, 1, 1, 1);
			Assert::AreEqual(true, karo->IsValidMove(move));

			//Meh my pieces are all in the wrong place for futher testing :p
			//Will fix later

			//Test Jumping diagnal up-left
			//move = gcnew Karo::Common::Move(0, 3, 2, 3, 2, 3);
			//Assert::AreEqual(true, karo->IsValidMove(move));

			//Test Jumping diagnal down-right
			//move = gcnew Karo::Common::Move(1, 0, 1, 2, 1, 2);
			//Assert::AreEqual(true, karo->IsValidMove(move));

			//Test Jumping diagnal down-left
			//move = gcnew Karo::Common::Move(0, 3, 2, 1, 2, 1);
			//Assert::AreEqual(true, karo->IsValidMove(move));
		}

		[TestMethod]
		void IsValidMoveTestValidMoveDistance()
		{
			//A piece can only move 1 square, unless it 'jumps' over another piece.
			//Todo:: Implement tests and code
			auto tiles = gcnew array<Tile^>(20);
			for (int i = 0; i < 20; i++) {
				tiles[i] = gcnew Tile(i % 5, i / 5);
			}

			auto pieces = gcnew array<Piece^>(12);
			pieces[0] = gcnew Piece(tiles[0 * 5 + 0], KaroPlayer::Player1, true);
            pieces[1] = gcnew Piece(tiles[0 * 5 + 1], KaroPlayer::Player1, true);
            pieces[2] = gcnew Piece(tiles[0 * 5 + 2], KaroPlayer::Player1, true);
            pieces[3] = gcnew Piece(tiles[0 * 5 + 3], KaroPlayer::Player1, true);
            pieces[4] = gcnew Piece(tiles[0 * 5 + 4], KaroPlayer::Player1, true);
            pieces[4] = gcnew Piece(tiles[1 * 5 + 0], KaroPlayer::Player1, true);
            pieces[0] = gcnew Piece(tiles[3 * 5 + 0], KaroPlayer::Player2, true);
            pieces[1] = gcnew Piece(tiles[3 * 5 + 1], KaroPlayer::Player2, true);
            pieces[2] = gcnew Piece(tiles[3 * 5 + 2], KaroPlayer::Player2, true);
            pieces[3] = gcnew Piece(tiles[3 * 5 + 3], KaroPlayer::Player2, true);
            pieces[4] = gcnew Piece(tiles[3 * 5 + 4], KaroPlayer::Player2, true);
            pieces[4] = gcnew Piece(tiles[2 * 5 + 0], KaroPlayer::Player2, true);
        
			auto karo = gcnew KaroBoardState(tiles, pieces);
			//Test moving 5squares [false]
			auto move = gcnew Karo::Common::Move(0, 3, 2, 0, 2, 0);
			Assert::AreEqual(false, karo->IsValidMove(move));

			//Test moving 2squares [true]
			move = gcnew Karo::Common::Move(1, 1, 0, 0, 0, 0);
            Assert::AreEqual(true, karo->IsValidMove(move));

			//Test moving to negative y [true]
			move = gcnew Karo::Common::Move(2, -1, 2, 0, 2, 0);
			Assert::AreEqual(true, karo->IsValidMove(move));

			//Test moving 4 squares [false] :: Should be [true] with jump!!
			move = gcnew Karo::Common::Move(2, 1, 0, 1, 0, 1);
			Assert::AreEqual(true, karo->IsValidMove(move));
		}

        [TestMethod]
        void WithMoveAppliedFirstPhase()
        {
            KaroBoardState^ karo = gcnew KaroBoardState();

            KaroBoardState^ newKaro = karo->WithMoveApplied(gcnew Karo::Common::Move(2, 3, 0, 0, 0, 0), KaroPlayer::Player1);

            Piece^ p = System::Linq::Enumerable::ElementAt(newKaro->Pieces, 0);

            Assert::AreEqual(2, p->X);
            Assert::AreEqual(3, p->Y);

        }
        [TestMethod]
        void WithMoveAppliedSecondPhase()
        {
            KaroBoardState^ karo = gcnew KaroBoardState();

            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(0, 0, 0, 0, 0, 0), KaroPlayer::Player1);
            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(1, 0, 0, 0, 0, 0), KaroPlayer::Player2);
            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(2, 0, 0, 0, 0, 0), KaroPlayer::Player1);
            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(3, 0, 0, 0, 0, 0), KaroPlayer::Player2);
            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(4, 0, 0, 0, 0, 0), KaroPlayer::Player1);
            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(0, 2, 0, 0, 0, 0), KaroPlayer::Player2);
            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(0, 1, 0, 0, 0, 0), KaroPlayer::Player1);
            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(1, 1, 0, 0, 0, 0), KaroPlayer::Player2);
            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(2, 1, 0, 0, 0, 0), KaroPlayer::Player1);
            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(3, 1, 0, 0, 0, 0), KaroPlayer::Player2);
            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(4, 1, 0, 0, 0, 0), KaroPlayer::Player1);
            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(1, 2, 0, 0, 0, 0), KaroPlayer::Player2);
            
            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(4, 2, 4, 0, 0, 0), KaroPlayer::Player1);
            
            Piece^ p = System::Linq::Enumerable::ElementAt(karo->Pieces, 4);

            Assert::AreEqual(4, p->X);
            Assert::AreEqual(2, p->Y);

        }

        [TestMethod]
        void GetWinnerHorizontal()
        {
            auto tiles = gcnew array<Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Piece^>(12);
            pieces[0] = gcnew Piece(tiles[0], KaroPlayer::Player1, true);
            pieces[1] = gcnew Piece(tiles[1], KaroPlayer::Player1, true);
            pieces[2] = gcnew Piece(tiles[2], KaroPlayer::Player1, true);
            pieces[3] = gcnew Piece(tiles[3], KaroPlayer::Player1, true);
            pieces[4] = gcnew Piece(tiles[4], KaroPlayer::Player1, false);
            pieces[5] = gcnew Piece(tiles[5], KaroPlayer::Player1, false);
            pieces[6] = gcnew Piece(tiles[6], KaroPlayer::Player2, false);
            pieces[7] = gcnew Piece(tiles[7], KaroPlayer::Player2, false);
            pieces[8] = gcnew Piece(tiles[8], KaroPlayer::Player2, false);
            pieces[9] = gcnew Piece(tiles[9], KaroPlayer::Player2, false);
            pieces[10] = gcnew Piece(tiles[10], KaroPlayer::Player2, false);
            pieces[11] = gcnew Piece(tiles[11], KaroPlayer::Player2, false);

            auto karo = gcnew KaroBoardState(tiles, pieces);

            Assert::AreEqual(KaroPlayer::Player1, karo->GetWinner());
        }
        [TestMethod]
        void GetWinnerVertical()
        {
            auto tiles = gcnew array<Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Piece^>(12);
            pieces[0] = gcnew Piece(tiles[0], KaroPlayer::Player1, true);
            pieces[1] = gcnew Piece(tiles[5], KaroPlayer::Player1, true);
            pieces[2] = gcnew Piece(tiles[10], KaroPlayer::Player1, true);
            pieces[3] = gcnew Piece(tiles[15], KaroPlayer::Player1, true);
            pieces[4] = gcnew Piece(tiles[4], KaroPlayer::Player1, false);
            pieces[5] = gcnew Piece(tiles[12], KaroPlayer::Player1, false);
            pieces[6] = gcnew Piece(tiles[6], KaroPlayer::Player2, false);
            pieces[7] = gcnew Piece(tiles[7], KaroPlayer::Player2, false);
            pieces[8] = gcnew Piece(tiles[8], KaroPlayer::Player2, false);
            pieces[9] = gcnew Piece(tiles[9], KaroPlayer::Player2, false);
            pieces[10] = gcnew Piece(tiles[13], KaroPlayer::Player2, false);
            pieces[11] = gcnew Piece(tiles[11], KaroPlayer::Player2, false);

            auto karo = gcnew KaroBoardState(tiles, pieces);

            Assert::AreEqual(KaroPlayer::Player1, karo->GetWinner());
        }
        [TestMethod]
        void GetWinnerDiagonal1()
        {
            auto tiles = gcnew array<Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Piece^>(12);
            pieces[0] = gcnew Piece(tiles[0], KaroPlayer::Player1, true);
            pieces[1] = gcnew Piece(tiles[6], KaroPlayer::Player1, true);
            pieces[2] = gcnew Piece(tiles[12], KaroPlayer::Player1, true);
            pieces[3] = gcnew Piece(tiles[18], KaroPlayer::Player1, true);
            pieces[4] = gcnew Piece(tiles[4], KaroPlayer::Player1, false);
            pieces[5] = gcnew Piece(tiles[5], KaroPlayer::Player1, false);
            pieces[6] = gcnew Piece(tiles[13], KaroPlayer::Player2, false);
            pieces[7] = gcnew Piece(tiles[7], KaroPlayer::Player2, false);
            pieces[8] = gcnew Piece(tiles[8], KaroPlayer::Player2, false);
            pieces[9] = gcnew Piece(tiles[9], KaroPlayer::Player2, false);
            pieces[10] = gcnew Piece(tiles[10], KaroPlayer::Player2, false);
            pieces[11] = gcnew Piece(tiles[11], KaroPlayer::Player2, false);

            auto karo = gcnew KaroBoardState(tiles, pieces);

            Assert::AreEqual(KaroPlayer::Player1, karo->GetWinner());
        }
        [TestMethod]
        void GetWinnerDiagonal2()
        {
            auto tiles = gcnew array<Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Piece^>(12);
            pieces[0] = gcnew Piece(tiles[4], KaroPlayer::Player2, true);
            pieces[1] = gcnew Piece(tiles[8], KaroPlayer::Player2, true);
            pieces[2] = gcnew Piece(tiles[12], KaroPlayer::Player2, true);
            pieces[3] = gcnew Piece(tiles[16], KaroPlayer::Player2, true);
            pieces[4] = gcnew Piece(tiles[4], KaroPlayer::Player1, false);
            pieces[5] = gcnew Piece(tiles[12], KaroPlayer::Player1, false);
            pieces[6] = gcnew Piece(tiles[6], KaroPlayer::Player2, false);
            pieces[7] = gcnew Piece(tiles[7], KaroPlayer::Player2, false);
            pieces[8] = gcnew Piece(tiles[8], KaroPlayer::Player2, false);
            pieces[9] = gcnew Piece(tiles[14], KaroPlayer::Player2, false);
            pieces[10] = gcnew Piece(tiles[10], KaroPlayer::Player2, false);
            pieces[11] = gcnew Piece(tiles[11], KaroPlayer::Player2, false);

            auto karo = gcnew KaroBoardState(tiles, pieces);

            Assert::AreEqual(KaroPlayer::Player2, karo->GetWinner());
        }
        [TestMethod]
        void GetWinnerDiagonal2NoWinner()
        {
            auto tiles = gcnew array<Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Piece^>(12);
            pieces[0] = gcnew Piece(tiles[4], KaroPlayer::Player2, true);
            pieces[1] = gcnew Piece(tiles[8], KaroPlayer::Player2, true);
            pieces[2] = gcnew Piece(tiles[12], KaroPlayer::Player2, false);
            pieces[3] = gcnew Piece(tiles[16], KaroPlayer::Player2, true);
            pieces[4] = gcnew Piece(tiles[4], KaroPlayer::Player1, false);
            pieces[5] = gcnew Piece(tiles[12], KaroPlayer::Player1, false);
            pieces[6] = gcnew Piece(tiles[6], KaroPlayer::Player2, false);
            pieces[7] = gcnew Piece(tiles[7], KaroPlayer::Player2, false);
            pieces[8] = gcnew Piece(tiles[8], KaroPlayer::Player2, false);
            pieces[9] = gcnew Piece(tiles[14], KaroPlayer::Player2, false);
            pieces[10] = gcnew Piece(tiles[10], KaroPlayer::Player2, false);
            pieces[11] = gcnew Piece(tiles[11], KaroPlayer::Player2, false);

            auto karo = gcnew KaroBoardState(tiles, pieces);

            Assert::AreEqual(KaroPlayer::None, karo->GetWinner());
        }
	};
}
