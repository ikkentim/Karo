#pragma once

#include "BoardPlayer.h"
namespace Karo {
    namespace Core {
        public enum class KaroPlayer {
            None = (int)BoardPlayer::None,
            Player1 = (int)BoardPlayer::Player1,
            Player2 = (int)BoardPlayer::Player2
        };
    }
}