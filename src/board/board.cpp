#include "Board.hpp"

#include <iostream>
#include <array>
#include <bitset>

void Board::printBoard()
{
	std::cout << "hello world" << std::endl;
}

void Board::updateBoard() {};

std::array<std::bitset<8>, 8> Board::getPiece(int piece) {};

std::bitset<8> Board::getBoardRow(int piece, int row) {};
