#include "stdafx.h"

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;
using namespace Karo;

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
			auto karo = gcnew Core::Karo();

			auto moves = karo->GetAvailableMoves(Core::Player::Player1);

			int i = 0;

			for each (Common::Move^ move in moves)
			{
				if (move != nullptr)
					i++;
			}

			Assert::AreEqual(20, i);
		};

        [TestMethod]
        void GetCornerTilesTest0CornersAvailable()
        {
            auto tiles = gcnew array<Karo::Core::Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Karo::Core::Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Karo::Core::Piece^>(12);

            pieces[0] = gcnew Karo::Core::Piece(tiles[0 * 5 + 0], Karo::Core::Player::Player1, false);
            pieces[1] = gcnew Karo::Core::Piece(tiles[3 * 5 + 0], Karo::Core::Player::Player1, false);
            pieces[2] = gcnew Karo::Core::Piece(tiles[0 * 5 + 4], Karo::Core::Player::Player1, false);
            pieces[3] = gcnew Karo::Core::Piece(tiles[3 * 5 + 4], Karo::Core::Player::Player1, false);
            auto karo = gcnew Core::Karo(tiles, pieces);

            auto corners = karo->GetCornerTiles();

            Assert::AreEqual(0, System::Linq::Enumerable::Count(corners));
        };

        [TestMethod]
        void GetCornerTilesTest2CornersAvailable()
        {
            auto tiles = gcnew array<Karo::Core::Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Karo::Core::Tile(i % 5, i / 5);
            }

            tiles[2]->X = 5;
            tiles[2]->Y = 0;

            auto pieces = gcnew array<Karo::Core::Piece^>(12);

            pieces[0] = gcnew Karo::Core::Piece(tiles[0 * 5 + 0], Karo::Core::Player::Player1, false);
            pieces[1] = gcnew Karo::Core::Piece(tiles[3 * 5 + 0], Karo::Core::Player::Player1, false);
            pieces[2] = gcnew Karo::Core::Piece(tiles[2], Karo::Core::Player::Player1, false);
            pieces[3] = gcnew Karo::Core::Piece(tiles[3 * 5 + 4], Karo::Core::Player::Player1, false);
            auto karo = gcnew Core::Karo(tiles, pieces);

            auto corners = karo->GetCornerTiles();

            for each(auto c in corners) {
                Console::WriteLine(c);
            }
            Assert::AreEqual(2, System::Linq::Enumerable::Count(corners));
        };

        [TestMethod]
        void GetCornerTilesTest1CornerAvailable()
        {
            auto tiles = gcnew array<Karo::Core::Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Karo::Core::Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Karo::Core::Piece^>(12);

            pieces[0] = gcnew Karo::Core::Piece(tiles[0 * 5 + 0], Karo::Core::Player::Player1, false);
            pieces[1] = gcnew Karo::Core::Piece(tiles[3 * 5 + 0], Karo::Core::Player::Player1, false);
            pieces[2] = gcnew Karo::Core::Piece(tiles[0 * 5 + 4], Karo::Core::Player::Player1, false);
            auto karo = gcnew Core::Karo(tiles, pieces);

            auto corners = karo->GetCornerTiles();

            Assert::AreEqual(1, System::Linq::Enumerable::Count(corners));
        };

        [TestMethod]
        void GetAvailableMovesTestCornersUnavailable()
        {
            auto tiles = gcnew array<Karo::Core::Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Karo::Core::Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Karo::Core::Piece^>(12);

            // Player1 fills the corners
            pieces[0] = gcnew Karo::Core::Piece(tiles[0 * 5 + 0], Karo::Core::Player::Player1, false);
            pieces[1] = gcnew Karo::Core::Piece(tiles[0 * 5 + 4], Karo::Core::Player::Player1, false);
            pieces[2] = gcnew Karo::Core::Piece(tiles[3 * 5 + 0], Karo::Core::Player::Player1, false);
            pieces[3] = gcnew Karo::Core::Piece(tiles[3 * 5 + 4], Karo::Core::Player::Player1, false);
            pieces[4] = gcnew Karo::Core::Piece(tiles[0 * 5 + 1], Karo::Core::Player::Player1, false);
            pieces[5] = gcnew Karo::Core::Piece(tiles[0 * 5 + 2], Karo::Core::Player::Player1, false);

            // It's Player2's turn
            pieces[6] = gcnew Karo::Core::Piece(tiles[1 * 5 + 0], Karo::Core::Player::Player2, false);
            pieces[7] = gcnew Karo::Core::Piece(tiles[1 * 5 + 1], Karo::Core::Player::Player2, false);
            pieces[8] = gcnew Karo::Core::Piece(tiles[1 * 5 + 2], Karo::Core::Player::Player2, false);
            pieces[9] = gcnew Karo::Core::Piece(tiles[1 * 5 + 3], Karo::Core::Player::Player2, false);
            pieces[10] = gcnew Karo::Core::Piece(tiles[1 * 5 + 4], Karo::Core::Player::Player2, false);
            pieces[11] = gcnew Karo::Core::Piece(tiles[2 * 5 + 4], Karo::Core::Player::Player2, false);
            auto karo = gcnew Core::Karo(tiles, pieces);

            auto moves = karo->GetAvailableMoves(Karo::Core::Player::Player2);

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
            auto tiles = gcnew array<Karo::Core::Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Karo::Core::Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Karo::Core::Piece^>(12);

            // Player1 fills the corners
            pieces[0] = gcnew Karo::Core::Piece(tiles[0 * 5 + 0], Karo::Core::Player::Player1, false);
            pieces[1] = gcnew Karo::Core::Piece(tiles[0 * 5 + 4], Karo::Core::Player::Player1, false);
            pieces[2] = gcnew Karo::Core::Piece(tiles[0 * 5 + 3], Karo::Core::Player::Player1, false);
            pieces[3] = gcnew Karo::Core::Piece(tiles[3 * 5 + 4], Karo::Core::Player::Player1, false);
            pieces[4] = gcnew Karo::Core::Piece(tiles[0 * 5 + 1], Karo::Core::Player::Player1, false);
            pieces[5] = gcnew Karo::Core::Piece(tiles[0 * 5 + 2], Karo::Core::Player::Player1, false);

            // It's Player2's turn
            pieces[6] = gcnew Karo::Core::Piece(tiles[1 * 5 + 0], Karo::Core::Player::Player2, false);
            pieces[7] = gcnew Karo::Core::Piece(tiles[1 * 5 + 1], Karo::Core::Player::Player2, false);
            pieces[8] = gcnew Karo::Core::Piece(tiles[1 * 5 + 2], Karo::Core::Player::Player2, false);
            pieces[9] = gcnew Karo::Core::Piece(tiles[1 * 5 + 3], Karo::Core::Player::Player2, false);
            pieces[10] = gcnew Karo::Core::Piece(tiles[1 * 5 + 4], Karo::Core::Player::Player2, false);
            pieces[11] = gcnew Karo::Core::Piece(tiles[2 * 5 + 4], Karo::Core::Player::Player2, false);
            auto karo = gcnew Core::Karo(tiles, pieces);

            auto moves = karo->GetAvailableMoves(Karo::Core::Player::Player2);

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
            auto tiles = gcnew array<Karo::Core::Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Karo::Core::Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Karo::Core::Piece^>(12);

            auto karo = gcnew Karo::Core::Karo(tiles, pieces);

            Assert::AreEqual(false, karo->IsFinished);
        };

        [TestMethod]
        void IsFinishedTestHorizontalLine4()
        {
            auto tiles = gcnew array<Karo::Core::Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Karo::Core::Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Karo::Core::Piece^>(12);
            pieces[0] = gcnew Karo::Core::Piece(tiles[0 * 5 + 1], Karo::Core::Player::Player1, true);
            pieces[1] = gcnew Karo::Core::Piece(tiles[1 * 5 + 1], Karo::Core::Player::Player1, true);
            pieces[2] = gcnew Karo::Core::Piece(tiles[2 * 5 + 1], Karo::Core::Player::Player1, true);
            pieces[3] = gcnew Karo::Core::Piece(tiles[3 * 5 + 1], Karo::Core::Player::Player1, true);

            auto karo = gcnew Karo::Core::Karo(tiles, pieces);

            Assert::AreEqual(true, karo->IsFinished);
        };

        [TestMethod]
        void IsFinishedTestHorizontalLine3()
        {
            auto tiles = gcnew array<Karo::Core::Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Karo::Core::Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Karo::Core::Piece^>(12);
            pieces[0] = gcnew Karo::Core::Piece(tiles[1 * 5 + 1], Karo::Core::Player::Player1, true);
            pieces[1] = gcnew Karo::Core::Piece(tiles[2 * 5 + 1], Karo::Core::Player::Player1, true);
            pieces[2] = gcnew Karo::Core::Piece(tiles[3 * 5 + 1], Karo::Core::Player::Player1, true);
            //pieces[3] = gcnew Karo::Core::Piece(tiles[5 * 5 + 1], Karo::Core::Player::Player1, true);

            auto karo = gcnew Karo::Core::Karo(tiles, pieces);

            Assert::AreEqual(false, karo->IsFinished);
        };

        [TestMethod]
        void IsFinishedTestHorizontalLine3WithOppenent()
        {
            auto tiles = gcnew array<Karo::Core::Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Karo::Core::Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Karo::Core::Piece^>(12);
            pieces[0] = gcnew Karo::Core::Piece(tiles[0 * 5 + 1], Karo::Core::Player::Player1, true);
            pieces[1] = gcnew Karo::Core::Piece(tiles[1 * 5 + 1], Karo::Core::Player::Player1, true);
            pieces[2] = gcnew Karo::Core::Piece(tiles[2 * 5 + 1], Karo::Core::Player::Player1, true);
            pieces[3] = gcnew Karo::Core::Piece(tiles[3 * 5 + 1], Karo::Core::Player::Player2, true);

            auto karo = gcnew Karo::Core::Karo(tiles, pieces);

            Assert::AreEqual(false, karo->IsFinished);
        };

        [TestMethod]
        void IsFinishedTestHorizontalLine3WithUpsideDown()
        {
            auto tiles = gcnew array<Karo::Core::Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Karo::Core::Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Karo::Core::Piece^>(12);
            pieces[0] = gcnew Karo::Core::Piece(tiles[0 * 5 + 1], Karo::Core::Player::Player1, true);
            pieces[1] = gcnew Karo::Core::Piece(tiles[1 * 5 + 1], Karo::Core::Player::Player1, true);
            pieces[2] = gcnew Karo::Core::Piece(tiles[2 * 5 + 1], Karo::Core::Player::Player1, true);
            pieces[3] = gcnew Karo::Core::Piece(tiles[3 * 5 + 1], Karo::Core::Player::Player1, false);

            auto karo = gcnew Karo::Core::Karo(tiles, pieces);

            Assert::AreEqual(false, karo->IsFinished);
        };
        [TestMethod]
        void IsFinishedTestDiagonalRightLine4()
        {
            auto tiles = gcnew array<Karo::Core::Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Karo::Core::Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Karo::Core::Piece^>(12);
            pieces[0] = gcnew Karo::Core::Piece(tiles[0 * 5 + 1], Karo::Core::Player::Player1, true);
            pieces[1] = gcnew Karo::Core::Piece(tiles[1 * 5 + 2], Karo::Core::Player::Player1, true);
            pieces[2] = gcnew Karo::Core::Piece(tiles[2 * 5 + 3], Karo::Core::Player::Player1, true);
            pieces[3] = gcnew Karo::Core::Piece(tiles[3 * 5 + 4], Karo::Core::Player::Player1, true);

            auto karo = gcnew Karo::Core::Karo(tiles, pieces);

            Assert::AreEqual(true, karo->IsFinished);
        };

        [TestMethod]
        void IsFinishedTestDiagonalLeftLine4()
        {
            auto tiles = gcnew array<Karo::Core::Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Karo::Core::Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Karo::Core::Piece^>(12);
            pieces[0] = gcnew Karo::Core::Piece(tiles[3 * 5 + 0], Karo::Core::Player::Player1, true);
            pieces[1] = gcnew Karo::Core::Piece(tiles[2 * 5 + 1], Karo::Core::Player::Player1, true);
            pieces[2] = gcnew Karo::Core::Piece(tiles[1 * 5 + 2], Karo::Core::Player::Player1, true);
            pieces[3] = gcnew Karo::Core::Piece(tiles[0 * 5 + 3], Karo::Core::Player::Player1, true);

            auto karo = gcnew Karo::Core::Karo(tiles, pieces);

            Assert::AreEqual(true, karo->IsFinished);
        };

		[TestMethod]
		void IsValidMoveTestSameLocation()
		{
			//Should not be possible to move to the current location
			auto tiles = gcnew array<Karo::Core::Tile^>(20);
			for (int i = 0; i < 20; i++) {
				tiles[i] = gcnew Karo::Core::Tile(i % 5, i / 5);
			}

			auto pieces = gcnew array<Karo::Core::Piece^>(12);
			pieces[0] = gcnew Karo::Core::Piece(tiles[0], Karo::Core::Player::Player1, true);
			pieces[1] = gcnew Karo::Core::Piece(tiles[1], Karo::Core::Player::Player2, true);
			pieces[2] = gcnew Karo::Core::Piece(tiles[2], Karo::Core::Player::Player1, true);
			pieces[3] = gcnew Karo::Core::Piece(tiles[3], Karo::Core::Player::Player2, true);

			auto karo = gcnew Karo::Core::Karo(tiles, pieces);
			//Move from (1,1) to (1,1) [false]
			auto move = gcnew Karo::Common::Move(1, 1, 1, 1, 1, 1);
			Assert::AreEqual(false, karo->IsValidMove(move));

			//Move from (1,1) to (1,2) [true]
			move = gcnew Karo::Common::Move(1, 2, 1, 1, 1, 1);
			Assert::AreEqual(true, karo->IsValidMove(move));
		}

		[TestMethod]
		void IsValidMoveTestFreeTile()
		{
			auto tiles = gcnew array<Karo::Core::Tile^>(20);
			for (int i = 0; i < 20; i++) {
				tiles[i] = gcnew Karo::Core::Tile(i % 5, i / 5);
			}

			auto pieces = gcnew array<Karo::Core::Piece^>(12);
			pieces[0] = gcnew Karo::Core::Piece(tiles[0], Karo::Core::Player::Player1, true);
			pieces[1] = gcnew Karo::Core::Piece(tiles[1], Karo::Core::Player::Player2, true);
			pieces[2] = gcnew Karo::Core::Piece(tiles[2], Karo::Core::Player::Player1, true);
			pieces[3] = gcnew Karo::Core::Piece(tiles[3], Karo::Core::Player::Player2, true);
			pieces[4] = gcnew Karo::Core::Piece(tiles[6], Karo::Core::Player::Player2, true);

			auto karo = gcnew Karo::Core::Karo(tiles, pieces);
			//Move to location without piece [true]
			auto move = gcnew Karo::Common::Move(0, 1, 0, 0, 0, 0);
			Assert::AreEqual(true, karo->IsValidMove(move));
			//Move to location with piece [false]
			move = gcnew Karo::Common::Move(1, 1, 0, 0, 0, 0);
			Assert::AreEqual(false, karo->IsValidMove(move));
		}

		[TestMethod]
		void IsValidMoveTestValidMoveDistance()
		{
			//A piece can only move 1 square, unless it 'jumps' over another piece.
			//Todo:: Implement tests and code
			auto tiles = gcnew array<Karo::Core::Tile^>(20);
			for (int i = 0; i < 20; i++) {
				tiles[i] = gcnew Karo::Core::Tile(i % 5, i / 5);
			}

			auto pieces = gcnew array<Karo::Core::Piece^>(12);
			pieces[0] = gcnew Karo::Core::Piece(tiles[0], Karo::Core::Player::Player1, true);
			pieces[1] = gcnew Karo::Core::Piece(tiles[1], Karo::Core::Player::Player2, true);
			pieces[2] = gcnew Karo::Core::Piece(tiles[2], Karo::Core::Player::Player1, true);
			pieces[3] = gcnew Karo::Core::Piece(tiles[3], Karo::Core::Player::Player2, true);
			pieces[4] = gcnew Karo::Core::Piece(tiles[6], Karo::Core::Player::Player2, true);

			auto karo = gcnew Karo::Core::Karo(tiles, pieces);
			//Test moving 5squares [false]
			auto move = gcnew Karo::Common::Move(3, 2, 0, 0, 0, 0);
			Assert::AreEqual(false, karo->IsValidMove(move));

			//Test moving 2squares [true]
			move = gcnew Karo::Common::Move(1, 1, 0, 0, 0, 0);
			Assert::AreEqual(true, karo->IsValidMove(move));

			//Test moving to negative y [true]
			move = gcnew Karo::Common::Move(0, -1, 0, 0, 0, 0);
			Assert::AreEqual(true, karo->IsValidMove(move));

			//Test moving 4 squares [false] :: Should be [true] with jump!!
			move = gcnew Karo::Common::Move(2, 2, 0, 0, 0, 0);
			Assert::AreEqual(false, karo->IsValidMove(move));
		}

        [TestMethod]
        void WithMoveAppliedFirstPhase()
        {
            Karo::Core::Karo^ karo = gcnew Karo::Core::Karo();

            Karo::Core::Karo^ newKaro = karo->WithMoveApplied(gcnew Karo::Common::Move(2, 3, 0, 0, 0, 0), Karo::Core::Player::Player1);

            Karo::Core::Piece^ p = System::Linq::Enumerable::ElementAt(newKaro->Pieces, 0);

            Assert::AreEqual(2, p->Tile->X);
            Assert::AreEqual(3, p->Tile->Y);

        }
        [TestMethod]
        void WithMoveAppliedSecondPhase()
        {
            Karo::Core::Karo^ karo = gcnew Karo::Core::Karo();

            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(0, 0, 0, 0, 0, 0), Karo::Core::Player::Player1);
            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(1, 0, 0, 0, 0, 0), Karo::Core::Player::Player2);
            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(2, 0, 0, 0, 0, 0), Karo::Core::Player::Player1);
            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(3, 0, 0, 0, 0, 0), Karo::Core::Player::Player2);
            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(4, 0, 0, 0, 0, 0), Karo::Core::Player::Player1);
            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(0, 2, 0, 0, 0, 0), Karo::Core::Player::Player2);
            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(0, 1, 0, 0, 0, 0), Karo::Core::Player::Player1);
            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(1, 1, 0, 0, 0, 0), Karo::Core::Player::Player2);
            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(2, 1, 0, 0, 0, 0), Karo::Core::Player::Player1);
            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(3, 1, 0, 0, 0, 0), Karo::Core::Player::Player2);
            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(4, 1, 0, 0, 0, 0), Karo::Core::Player::Player1);
            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(1, 2, 0, 0, 0, 0), Karo::Core::Player::Player2);
            karo = karo->WithMoveApplied(gcnew Karo::Common::Move(4, 2, 4, 0, 0, 0), Karo::Core::Player::Player1);
            
            Karo::Core::Piece^ p = System::Linq::Enumerable::ElementAt(karo->Pieces, 4);

            Assert::AreEqual(4, p->Tile->X);
            Assert::AreEqual(2, p->Tile->Y);

        }

        [TestMethod]
        void GetWinnerHorizontal()
        {
            auto tiles = gcnew array<Karo::Core::Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Karo::Core::Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Karo::Core::Piece^>(12);
            pieces[0] = gcnew Karo::Core::Piece(tiles[0], Karo::Core::Player::Player1, true);
            pieces[1] = gcnew Karo::Core::Piece(tiles[1], Karo::Core::Player::Player1, true);
            pieces[2] = gcnew Karo::Core::Piece(tiles[2], Karo::Core::Player::Player1, true);
            pieces[3] = gcnew Karo::Core::Piece(tiles[3], Karo::Core::Player::Player1, true);
            pieces[4] = gcnew Karo::Core::Piece(tiles[4], Karo::Core::Player::Player1, false);
            pieces[5] = gcnew Karo::Core::Piece(tiles[5], Karo::Core::Player::Player1, false);
            pieces[6] = gcnew Karo::Core::Piece(tiles[6], Karo::Core::Player::Player2, false);
            pieces[7] = gcnew Karo::Core::Piece(tiles[7], Karo::Core::Player::Player2, false);
            pieces[8] = gcnew Karo::Core::Piece(tiles[8], Karo::Core::Player::Player2, false);
            pieces[9] = gcnew Karo::Core::Piece(tiles[9], Karo::Core::Player::Player2, false);
            pieces[10] = gcnew Karo::Core::Piece(tiles[10], Karo::Core::Player::Player2, false);
            pieces[11] = gcnew Karo::Core::Piece(tiles[11], Karo::Core::Player::Player2, false);

            auto karo = gcnew Karo::Core::Karo(tiles, pieces);

            Assert::AreEqual(Karo::Core::Player::Player1, karo->GetWinner());
        }
        [TestMethod]
        void GetWinnerVertical()
        {
            auto tiles = gcnew array<Karo::Core::Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Karo::Core::Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Karo::Core::Piece^>(12);
            pieces[0] = gcnew Karo::Core::Piece(tiles[0], Karo::Core::Player::Player1, true);
            pieces[1] = gcnew Karo::Core::Piece(tiles[5], Karo::Core::Player::Player1, true);
            pieces[2] = gcnew Karo::Core::Piece(tiles[10], Karo::Core::Player::Player1, true);
            pieces[3] = gcnew Karo::Core::Piece(tiles[15], Karo::Core::Player::Player1, true);
            pieces[4] = gcnew Karo::Core::Piece(tiles[4], Karo::Core::Player::Player1, false);
            pieces[5] = gcnew Karo::Core::Piece(tiles[12], Karo::Core::Player::Player1, false);
            pieces[6] = gcnew Karo::Core::Piece(tiles[6], Karo::Core::Player::Player2, false);
            pieces[7] = gcnew Karo::Core::Piece(tiles[7], Karo::Core::Player::Player2, false);
            pieces[8] = gcnew Karo::Core::Piece(tiles[8], Karo::Core::Player::Player2, false);
            pieces[9] = gcnew Karo::Core::Piece(tiles[9], Karo::Core::Player::Player2, false);
            pieces[10] = gcnew Karo::Core::Piece(tiles[13], Karo::Core::Player::Player2, false);
            pieces[11] = gcnew Karo::Core::Piece(tiles[11], Karo::Core::Player::Player2, false);

            auto karo = gcnew Karo::Core::Karo(tiles, pieces);

            Assert::AreEqual(Karo::Core::Player::Player1, karo->GetWinner());
        }
        [TestMethod]
        void GetWinnerDiagonal1()
        {
            auto tiles = gcnew array<Karo::Core::Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Karo::Core::Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Karo::Core::Piece^>(12);
            pieces[0] = gcnew Karo::Core::Piece(tiles[0], Karo::Core::Player::Player1, true);
            pieces[1] = gcnew Karo::Core::Piece(tiles[6], Karo::Core::Player::Player1, true);
            pieces[2] = gcnew Karo::Core::Piece(tiles[12], Karo::Core::Player::Player1, true);
            pieces[3] = gcnew Karo::Core::Piece(tiles[18], Karo::Core::Player::Player1, true);
            pieces[4] = gcnew Karo::Core::Piece(tiles[4], Karo::Core::Player::Player1, false);
            pieces[5] = gcnew Karo::Core::Piece(tiles[5], Karo::Core::Player::Player1, false);
            pieces[6] = gcnew Karo::Core::Piece(tiles[13], Karo::Core::Player::Player2, false);
            pieces[7] = gcnew Karo::Core::Piece(tiles[7], Karo::Core::Player::Player2, false);
            pieces[8] = gcnew Karo::Core::Piece(tiles[8], Karo::Core::Player::Player2, false);
            pieces[9] = gcnew Karo::Core::Piece(tiles[9], Karo::Core::Player::Player2, false);
            pieces[10] = gcnew Karo::Core::Piece(tiles[10], Karo::Core::Player::Player2, false);
            pieces[11] = gcnew Karo::Core::Piece(tiles[11], Karo::Core::Player::Player2, false);

            auto karo = gcnew Karo::Core::Karo(tiles, pieces);

            Assert::AreEqual(Karo::Core::Player::Player1, karo->GetWinner());
        }
        [TestMethod]
        void GetWinnerDiagonal2()
        {
            auto tiles = gcnew array<Karo::Core::Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Karo::Core::Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Karo::Core::Piece^>(12);
            pieces[0] = gcnew Karo::Core::Piece(tiles[4], Karo::Core::Player::Player2, true);
            pieces[1] = gcnew Karo::Core::Piece(tiles[8], Karo::Core::Player::Player2, true);
            pieces[2] = gcnew Karo::Core::Piece(tiles[12], Karo::Core::Player::Player2, true);
            pieces[3] = gcnew Karo::Core::Piece(tiles[16], Karo::Core::Player::Player2, true);
            pieces[4] = gcnew Karo::Core::Piece(tiles[4], Karo::Core::Player::Player1, false);
            pieces[5] = gcnew Karo::Core::Piece(tiles[12], Karo::Core::Player::Player1, false);
            pieces[6] = gcnew Karo::Core::Piece(tiles[6], Karo::Core::Player::Player2, false);
            pieces[7] = gcnew Karo::Core::Piece(tiles[7], Karo::Core::Player::Player2, false);
            pieces[8] = gcnew Karo::Core::Piece(tiles[8], Karo::Core::Player::Player2, false);
            pieces[9] = gcnew Karo::Core::Piece(tiles[14], Karo::Core::Player::Player2, false);
            pieces[10] = gcnew Karo::Core::Piece(tiles[10], Karo::Core::Player::Player2, false);
            pieces[11] = gcnew Karo::Core::Piece(tiles[11], Karo::Core::Player::Player2, false);

            auto karo = gcnew Karo::Core::Karo(tiles, pieces);

            Assert::AreEqual(Karo::Core::Player::Player2, karo->GetWinner());
        }
        [TestMethod]
        void GetWinnerDiagonal2NoWinner()
        {
            auto tiles = gcnew array<Karo::Core::Tile^>(20);
            for (int i = 0; i < 20; i++) {
                tiles[i] = gcnew Karo::Core::Tile(i % 5, i / 5);
            }

            auto pieces = gcnew array<Karo::Core::Piece^>(12);
            pieces[0] = gcnew Karo::Core::Piece(tiles[4], Karo::Core::Player::Player2, true);
            pieces[1] = gcnew Karo::Core::Piece(tiles[8], Karo::Core::Player::Player2, true);
            pieces[2] = gcnew Karo::Core::Piece(tiles[12], Karo::Core::Player::Player2, false);
            pieces[3] = gcnew Karo::Core::Piece(tiles[16], Karo::Core::Player::Player2, true);
            pieces[4] = gcnew Karo::Core::Piece(tiles[4], Karo::Core::Player::Player1, false);
            pieces[5] = gcnew Karo::Core::Piece(tiles[12], Karo::Core::Player::Player1, false);
            pieces[6] = gcnew Karo::Core::Piece(tiles[6], Karo::Core::Player::Player2, false);
            pieces[7] = gcnew Karo::Core::Piece(tiles[7], Karo::Core::Player::Player2, false);
            pieces[8] = gcnew Karo::Core::Piece(tiles[8], Karo::Core::Player::Player2, false);
            pieces[9] = gcnew Karo::Core::Piece(tiles[14], Karo::Core::Player::Player2, false);
            pieces[10] = gcnew Karo::Core::Piece(tiles[10], Karo::Core::Player::Player2, false);
            pieces[11] = gcnew Karo::Core::Piece(tiles[11], Karo::Core::Player::Player2, false);

            auto karo = gcnew Karo::Core::Karo(tiles, pieces);

            Assert::AreEqual(Karo::Core::Player::None, karo->GetWinner());
        }
	};
}
