#ifndef BOARD_HPP
#define BOARD_HPP

#include <bitset>
#include <array>

class Board
{
public:
	void printBoard();
	void updateBoard();

private:
	std::array<std::bitset<8>, 8> getPiece(int piece);
	std::bitset<8> getBoardRow(int piece, int row);
	std::array<std::array<std::bitset<8>, 8>, 14> bBoard;
};

#endif
