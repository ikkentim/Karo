#pragma once
using namespace System;
using namespace System::Collections::Generic;

namespace Karo {
	namespace AI {

		public ref class AI :public Karo::Common::IPlayer
		{
		public:
			AI();
			~AI();

			Karo::Common::Move^ ChooseBestMove(Karo::Core::Karo, int, Karo::Core::Player);
			void DoMove(Karo::Common::Move^ previousMove, int timeLimit, Func<Karo::Common::Move^>^ done) override;

		private:
			Karo::Core::Karo BoardState;
		};
	}
}