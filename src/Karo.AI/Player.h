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
            int Evaluate();
            property int prune_count { int get(){
                return intelligence_->prune_count;
			}};
        private:
            Intelligence * intelligence_;
        };
    }
}