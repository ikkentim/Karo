#pragma once
using namespace System;
using namespace System::Collections::Generic;

#include "Intelligence.h"

namespace Karo {
    namespace AI {
        public ref class Player : public Karo::Common::IPlayer
        {
        public:
            Player();
            ~Player();
            void DoMove(Karo::Common::Move^ previousMove, int timeLimit, Action<Karo::Common::Move^>^ done) override;

        private:
            Intelligence * intelligence_;
        };
    }
}