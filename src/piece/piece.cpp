#include "Piece.hpp"

#include <cstdint>
#include <iostream>

std::pair<bool, sb> Piece::move(const fb &bBoard, const sb &currentPos, const sb &futurePos) {
	const int bIndex = getBoardIndex(bBoard, currentPos);

	if (bIndex == -1) {
		std::cerr << "Invalid position sent to move" << std::endl;
		return {false, {}};
	}

	if (const sb validMoves = getValidMoves(bBoard, currentPos); (futurePos & validMoves) == 0) { return {false, {}}; }

	return {true, bBoard[bIndex] & ~currentPos | futurePos};
}

sb Piece::getValidMoves(const fb &bBoard, const sb &currentPos) {
	const int bIndex = getBoardIndex(bBoard, currentPos);

	if (bIndex == -1) { std::cerr << "Invalid position sent to getValidMoves" << std::endl; }

	sb validMoves = 0;

	switch (bIndex / 2) {
		case 0: // pawn
		case 1: {
			// bishop
			// check position with edges
			auto [edges, boundaries] = edgeCheck(currentPos);

			uint8_t validDirections{0xF}; // MSB is tl, tr, br, bl

			// top edge
			if ((0x8 & edges) == 0) {
				validDirections &= ~0xC; // tl, tr
			}

			// right edge
			if ((0x4 & edges) == 0) {
				validDirections &= ~0x6; // tr, br
			}

			// bottom edge
			if ((0x2 & edges) == 0) {
				validDirections &= ~0x3; // bl, br
			}

			// left edge
			if ((0x1 & edges) == 0) {
				validDirections &= ~0x9; // tl, bl
			}

			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					if ((validDirections & (0x8 >> ((i * 2) + j))) != 0) {
						constexpr std::array<int, 4> directions{9, 7};
						validMoves |= getDirection(bBoard, boundaries, currentPos, directions[j], i == 0, bIndex % 2);
					}
				}
			}
			break;
		}
		case 2: // knight
		case 3: {
			// rook
			// check position with edges
			auto [edges, boundaries] = edgeCheck(currentPos);

			uint8_t validDirections{0xF}; // MSB is l, t, r, b

			// top edge
			if ((0x8 & edges) == 0) {
				validDirections &= ~0x4; // t
			}

			// right edge
			if ((0x4 & edges) == 0) {
				validDirections &= ~0x2; // r
			}

			// bottom edge
			if ((0x2 & edges) == 0) {
				validDirections &= ~0x1; // b
			}

			// left edge
			if ((0x1 & edges) == 0) {
				validDirections &= ~0x8; // l
			}

			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					if ((validDirections & (0x8 >> ((i * 2) + j))) != 0) {
						constexpr std::array<int, 4> directions{1, 8};
						validMoves |= getDirection(bBoard, boundaries, currentPos, directions[j], i == 0, bIndex % 2);
					}
				}
			}
			break;
		}
		case 4: {
			// queen
			// check position with edges
			auto [edges, boundaries] = edgeCheck(currentPos);

			uint8_t validDirections{0xFF}; // MSB is l, tl, t, tr, r, br, b, bl

			// top edge
			if ((0x8 & edges) == 0) {
				validDirections &= ~0x70; // tl, t, tr
			}

			// right edge
			if ((0x4 & edges) == 0) {
				validDirections &= ~0x1C; // tr, r, br
			}

			// bottom edge
			if ((0x2 & edges) == 0) {
				validDirections &= ~0x7; // br, b, bl
			}

			// left edge
			if ((0x1 & edges) == 0) {
				validDirections &= ~0xC1; // bl, l, tl
			}

			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 4; j++) {
					if ((validDirections & (0x80 >> ((i * 4) + j))) != 0) {
						constexpr std::array<int, 4> directions{1, 9, 8, 7};
						validMoves |= getDirection(bBoard, boundaries, currentPos, directions[j], i == 0, bIndex % 2);
					}
				}
			}

			break;
		}
		case 5: // king
		default:
			break;
	}

	return validMoves;
}

int Piece::getBoardIndex(const fb &bBoard, const sb &currentPos) {
	for (int i = 0; i < 12; i++) { if ((bBoard[i] & currentPos) != 0) { return i; } }

	return -1;
}

std::pair<uint8_t, sb> Piece::edgeCheck(const sb &position) {\
	uint8_t edges{0};
	sb boundaries{0};

	// each bit represents each of the sides

	for (int i = 0; i < 4; i++) {
		constexpr std::array<sb, 4> bEdges{
			0xFF00000000000000ULL, // top
			0x0101010101010101ULL, // right
			0x00000000000000FFULL, // bottom
			0x8080808080808080ULL // left
		};
		if ((bEdges[i] & position) == 0) {
			edges |= (static_cast<uint8_t>(0x8) >> i);
			boundaries |= bEdges[i];
		}
	}

	return {edges, boundaries};
}

sb Piece::getDirection(const fb &bBoard, const sb &boundaries, const sb &currentPos, const int &shift,
                       const bool &direction, const bool &color) {
	sb outBoard{0};
	sb tempBoard{currentPos};

	const sb sColorBoard{getColorBoard(bBoard, color)};
	const sb dColorBoard{getColorBoard(bBoard, !color)};

	for (int i = 0; i < 8; i++) {
		tempBoard = direction ? tempBoard << shift : tempBoard >> shift;

		if ((tempBoard & sColorBoard) != 0) { return outBoard; }

		outBoard |= tempBoard;

		if ((tempBoard & dColorBoard) != 0 || (tempBoard & boundaries) != 0) { return outBoard; }
	}

	return outBoard;
}

sb Piece::getColorBoard(const fb &bBoard, const bool &color) {
	sb outBoard{0};

	for (int i = color; i < 12; i += 2) { outBoard |= bBoard[i]; }

	return outBoard;
}
