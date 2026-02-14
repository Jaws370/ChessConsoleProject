#pragma once

#include <array>
#include <cstdint>

using sb = uint64_t;
using fb = std::array<sb, 12>;

class Board {
public:
	Board();
	void printBoard();
	void updateBoard();

private:
	sb getPiece(int piece);
	fb bBoard;
};
