#pragma once

struct BoardTile {
    int x;
    int y;
	bool tagged;

    BoardTile()
        : x(0), y(0) { }

    BoardTile(int xx, int yy)
        : x(xx), y(yy) { }

	void tag(){
		tagged = true;
	}

	void untag(){
		tagged = false;
	}

    inline bool operator==(BoardTile o) {
        return x == o.x && y == o.y;
    }

    inline bool operator!=(BoardTile o) {
        return !(*this == o);
    }
};