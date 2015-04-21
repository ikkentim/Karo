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
		void FirstGetAvailableMovesReturns20Options()
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
        void IsFinishedTestHorizontalLine3AndEnemy()
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
        void IsFinishedTestHorizontalLine3AndUpsideDown()
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

			//These tiles have pieces
			tiles[0]->HasPiece = true;
			tiles[1]->HasPiece = true;
			tiles[2]->HasPiece = true;
			tiles[3]->HasPiece = true;
			tiles[6]->HasPiece = true;

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
	};
}
