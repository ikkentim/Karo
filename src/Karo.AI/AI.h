#pragma once
using namespace System;

namespace Karo {
	namespace AI {

		public ref class AI :public Karo::Common::IPlayer
		{
		public:
			AI();

			Karo::Common::Move^ ChooseBestMove(Karo::Core::Karo^, int, Karo::Core::Player);
			int Evaluate(Karo::Core::Karo^, Karo::Core::Player);
			void DoMove(Karo::Common::Move^ previousMove, int timeLimit, Action<Karo::Common::Move^>^ done) override;

			array < Tuple<int, int>^> ^ neighbours = gcnew array < Tuple < int, int > ^ > {
				gcnew Tuple<int, int>(0, 1),
					gcnew Tuple<int, int>(1, -1),
					gcnew Tuple<int, int>(1, 0),
					gcnew Tuple<int, int>(1, 1),
					gcnew Tuple<int, int>(0, 1),
					gcnew Tuple<int, int>(-1, 1),
					gcnew Tuple<int, int>(-1, 0),
					gcnew Tuple<int, int>(-1, -1)
			};

		private:
			Karo::Core::Karo^ BoardState;
		};
	}
}