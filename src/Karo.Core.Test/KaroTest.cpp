#include "stdafx.h"

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

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
		void TestMethod()
		{
			auto karo = gcnew Karo::Core::Karo();

			//karo->GetAvailableMoves();

			//
			// TODO: Add test logic here
			//
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
	};
}
