#include "Piece.hpp"

#include <cmath>
#include <iostream>
#include <bits/ostream.tcc>

std::pair<bool, sb> Piece::move(const fb &bBoard, const sb &currentPos, const sb &futurePos) {

	const int bIndex = getBoardIndex(bBoard, currentPos);

	if (bIndex == -1) {
		return {false, {}};
	}

	if (const sb validMoves = getValidMoves(bBoard, currentPos); (futurePos & validMoves) == 0) {
		return {false, {}};
	}

	return {true, bBoard[bIndex] & ~currentPos | futurePos};
}

sb Piece::getValidMoves(const fb &bBoard, const sb &currentPos) {

	const int bIndex = getBoardIndex(bBoard, currentPos);

	if (bIndex == -1) {
		std::cerr << "Invalid position sent to getValidMoves" << std::endl;
	}

	int pColor = bIndex % 2;

	switch (floor(bIndex/2)) {
		case 0: // pawn
		case 1: // bishop
		case 2: // knight
		case 3: // rook
		case 4: // queen
			// check position with edges
			std::bitset<4> edges; sb boundaries;
			std::pair(edges, boundaries) = edgeCheck(currentPos);

			std::bitset<8> validDirections{"11111111"}; // MSB is top, then clockwise

			// top edge
			if ((std::bitset<4>("1000") & edges) != 0) {
				validDirections &= ~std::bitset<8>("11000001");
			}

			// right edge
			if ((std::bitset<4>("0100") & edges) != 0) {
				validDirections &= ~std::bitset<8>("01110000");
			}

			// bottom edge
			if ((std::bitset<4>("0010") & edges) != 0) {
				validDirections &= ~std::bitset<8>("00011100");
			}

			// left edge
			if ((std::bitset<4>("0001") & edges) != 0) {
				validDirections &= ~std::bitset<8>("00000111");
			}

			// need to loop along each direction... make loop function that is recursive? could take in all the directions then do each one at a time?
			// make input how much bitshift each time

			break;
		case 5: // king
		default:
	}

	return {};
}

int Piece::getBoardIndex(const fb &bBoard, const sb &currentPos) {
	for (int i = 0; i < 12; i++) {
		if ((bBoard[i] & currentPos) != 0) {
			return i;
		}
	}

	return -1;
}

std::pair<std::bitset<4>, sb> Piece::edgeCheck(const sb &position) {\

	std::bitset<4> edges = 0;
	sb boundaries = 0;

	// each bit represents each of the sides
	const std::array<sb, 4> bEdges = {
		sb("111111110000000000000000000000000000000000000000000000000000000"),  // top
		sb("0000000100000001000000010000000100000001000000010000000100000001"), // right
		sb("0000000000000000000000000000000000000000000000000000000011111111"), // bottom
		sb("1000000010000000100000001000000010000000100000001000000010000000")  // left
	};

	for (int i = 0; i < 4; i++) {
		if ((bEdges[i] & position) == 0) {
			edges |= (std::bitset<4>("1000") >> i);
			boundaries |= bEdges[i];
		}
	}

	return {edges, boundaries};
}