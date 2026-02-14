#include "Board.hpp"

#include <iostream>
#include <array>
#include <bitset>

Board::Board() { bBoard.fill(0); }

void Board::printBoard() {
	std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
	std::cout << "| a | a | a | a | a | a | a | a |" << std::endl;
	std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
	std::cout << "| a | a | a | a | a | a | a | a |" << std::endl;
	std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
	std::cout << "| a | a | a | a | a | a | a | a |" << std::endl;
	std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
	std::cout << "| a | a | a | a | a | a | a | a |" << std::endl;
	std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
	std::cout << "| a | a | a | a | a | a | a | a |" << std::endl;
	std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
	std::cout << "| a | a | a | a | a | a | a | a |" << std::endl;
	std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
	std::cout << "| a | a | a | a | a | a | a | a |" << std::endl;
	std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
	std::cout << "| a | a | a | a | a | a | a | a |" << std::endl;
	std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
}

void Board::updateBoard() {};

sb Board::getPiece(int piece) { return {}; };
