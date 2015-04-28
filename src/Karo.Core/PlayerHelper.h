// PlayerHelper.h

#pragma once
#include "Player.h"

namespace Karo {
	namespace Core {
		public ref class PlayerHelper {
		public:
			static Player Opposite(Player);
		};
	}
}