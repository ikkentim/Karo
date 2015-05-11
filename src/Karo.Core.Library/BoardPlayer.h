#pragma once

#define OPPONENT(p) (p == BoardPlayer::Player1 \
    ? BoardPlayer::Player2 : p == BoardPlayer::Player2 \
    ? BoardPlayer::Player1 : BoardPlayer::None)

enum BoardPlayer {
    None = 0,
    Player1 = 1,
    Player2 = 2
};