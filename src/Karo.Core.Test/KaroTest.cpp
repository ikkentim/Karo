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

            for each(auto move in moves) {
                Console::WriteLine(move);
            }
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
	};
}
