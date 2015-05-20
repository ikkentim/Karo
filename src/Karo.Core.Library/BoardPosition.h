#pragma once
struct BoardPosition {
    int x;
    int y;

    BoardPosition()
        : x(0), y(0) { }

    BoardPosition(int xx, int yy)
        : x(xx), y(yy) { }

    inline bool operator==(BoardPosition o) {
        return x == o.x && y == o.y;
    }

    inline bool operator!=(BoardPosition o) {
        return !(*this == o);
    }
};