#include <iostream>
#include "../include/chess.h"

int main() {
	chess game;

	std::cout << game.get_board() << std::endl;

	game.set_board("8/8/8/8/8/8/7p/8");

	std::cout << game.get_board() << std::endl;

	return 0;
}
