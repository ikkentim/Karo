// PlayerHelper.h

#pragma once
#include "stdafx.h"
#include "PlayerHelper.h"

namespace Karo {
	namespace Core {
		Player PlayerHelper::Opposite(Player player)
		{
			if (player == Karo::Core::Player::Player1)
				return Karo::Core::Player::Player2;

			return Karo::Core::Player::Player1;
		}
	}
}