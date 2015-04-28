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

		private:
			Karo::Core::Karo^ BoardState;
			Random^ rand = gcnew Random();
		};
	}
}