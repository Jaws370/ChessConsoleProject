#ifndef BOARD_HPP
#define BOARD_HPP

#include <bitset>
#include <array>

using fb = std::array<std::bitset<64>, 12>;
using sb = std::bitset<64>;
using row = std::bitset<8>;

class Board
{
public:
	Board();
	void printBoard();
	void updateBoard();

private:
	row getRow(int piece, int row);
	sb getPiece(int piece);
	fb bBoard;
};

#endif
