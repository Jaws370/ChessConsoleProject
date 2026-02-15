#include "Piece.hpp"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

std::pair<bool, sb> Piece::move(const fb &bBoard, const sb &currentPos, const sb &futurePos) {
	const int bIndex = getBoardIndex(bBoard, currentPos);

	if (bIndex == -1) {
		std::cerr << "Invalid position sent to move" << std::endl;
		return {false, {}};
	}

	if (const sb validMoves = getValidMoves(bBoard, currentPos); (futurePos & validMoves) == 0) { return {false, {}}; }

	return {true, (bBoard[bIndex] & ~currentPos) | futurePos};
}

sb Piece::getValidMoves(const fb &bBoard, const sb &currentPos) {
	const int bIndex = getBoardIndex(bBoard, currentPos);

	if (bIndex == -1) { std::cerr << "Invalid position sent to getValidMoves" << std::endl; }

	sb validMoves = 0;

	switch (bIndex / 2) {
		case 0: {
			sb tempBoard = bIndex % 2 == 0 ? currentPos << 8 : currentPos >> 8;

			const sb sColorBoard = getColorBoard(bBoard, bIndex % 2);
			const sb dColorBoard = getColorBoard(bBoard, !(bIndex % 2));

			// take left
			if ((dColorBoard & (tempBoard << 1)) != 0) { validMoves |= (tempBoard << 1); }

			// take right
			if ((dColorBoard & (tempBoard >> 1)) != 0) { validMoves |= (tempBoard >> 1); }

			// move forward
			if ((tempBoard & (sColorBoard | dColorBoard)) == 0) {
				validMoves |= tempBoard;

				// move forward twice
				tempBoard = bIndex % 2 == 0 ? tempBoard << 8 : tempBoard >> 8;
				if ((bIndex % 2 && (currentPos & 0x00FF000000000000ULL) != 0) ||
				    (!(bIndex % 2) && (currentPos & 0x000000000000FF00ULL) != 0)) { validMoves |= tempBoard; }
			}
			break;
		} // pawn
		case 1: {
			constexpr std::array<sb, 4> bEdges{
				0xFF00000000000000ULL, // top
				0x0101010101010101ULL, // right
				0x00000000000000FFULL, // bottom
				0x8080808080808080ULL // left
			};
			auto [edges, boundaries] = edgeCheck(currentPos, bEdges);

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
						constexpr std::array<int, 2> directions{9, 7};
						validMoves |= getDirection(bBoard, boundaries, currentPos, directions[j], i == 0, bIndex % 2);
					}
				}
			}
			break;
		} // bishop
		case 2: {
			constexpr std::array<sb, 4> iBEdges{
				0xFFFF000000000000ULL, // top
				0x0303030303030303ULL, // right
				0x000000000000FFFFULL, // bottom
				0xC0C0C0C0C0C0C0C0ULL // left
			};

			auto [iEdges, iBoundaries] = edgeCheck(currentPos, iBEdges);

			constexpr std::array<sb, 4> oBEdges{
				0xFF00000000000000ULL, // top
				0x0101010101010101ULL, // right
				0x00000000000000FFULL, // bottom
				0x8080808080808080ULL // left
			};

			auto [oEdges, oBoundaries] = edgeCheck(currentPos, oBEdges);

			uint8_t validDirections{0xFF}; // MSB is lr, ll, tr, tl, lll, llr, bl, br

			// top edge
			if ((0x8 & oEdges) == 0) {
				validDirections &= ~0xF0; // lr ll tr tl
			} else if ((0x8 & iEdges) == 0) {
				validDirections &= ~0x30; // tr tl
			}

			// right edge
			if ((0x4 & oEdges) == 0) {
				validDirections &= ~0xA5; // lr tr llr br
			} else if ((0x4 & iEdges) == 0) {
				validDirections &= ~0x84; // lr, llr
			}

			// bottom edge
			if ((0x2 & oEdges) == 0) {
				validDirections &= ~0xF; // lll, llr, bl, br
			} else if ((0x2 & iEdges) == 0) {
				validDirections &= ~0x3; // bl, br
			}

			// left edge
			if ((0x1 & oEdges) == 0) {
				validDirections &= ~0x5A; // ll, tl, lll, bl
			} else if ((0x1 & iEdges) == 0) {
				validDirections &= ~0x48; // ll, lll
			}

			const sb sColorBoard{getColorBoard(bBoard, (bIndex % 2))};

			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 4; j++) {
					if ((validDirections & (0x80 >> ((i * 4) + j))) != 0) {
						constexpr std::array<int, 4> directions{6, 10, 15, 17};

						if (const sb tempBoard = i == 0 ? currentPos << directions[j] : currentPos >> directions[j];
							(tempBoard & sColorBoard) == 0) { validMoves |= tempBoard; }
					}
				}
			}

			break;
		} // knight
		case 3: {
			constexpr std::array<sb, 4> bEdges{
				0xFF00000000000000ULL, // top
				0x0101010101010101ULL, // right
				0x00000000000000FFULL, // bottom
				0x8080808080808080ULL // left
			};

			auto [edges, boundaries] = edgeCheck(currentPos, bEdges);

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
						constexpr std::array<int, 2> directions{1, 8};
						validMoves |= getDirection(bBoard, boundaries, currentPos, directions[j], i == 0, bIndex % 2);
					}
				}
			}
			break;
		} // rook
		case 4: {
			constexpr std::array<sb, 4> bEdges{
				0xFF00000000000000ULL, // top
				0x0101010101010101ULL, // right
				0x00000000000000FFULL, // bottom
				0x8080808080808080ULL // left
			};

			auto [edges, boundaries] = edgeCheck(currentPos, bEdges);

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
		} // queen
		case 5: {
			constexpr std::array<sb, 4> bEdges{
				0xFF00000000000000ULL, // top
				0x0101010101010101ULL, // right
				0x00000000000000FFULL, // bottom
				0x8080808080808080ULL // left
			};

			auto [edges, boundaries] = edgeCheck(currentPos, bEdges);

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

			const sb sColorBoard{getColorBoard(bBoard, (bIndex % 2))};

			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 4; j++) {
					if ((validDirections & (0x80 >> ((i * 4) + j))) != 0) {
						constexpr std::array<int, 4> directions{1, 7, 8, 9};

						if (const sb tempBoard = i == 0 ? currentPos << directions[j] : currentPos >> directions[j];
							(tempBoard & sColorBoard) == 0) { validMoves |= tempBoard; }
					}
				}
			}

			break;
		} // king
		default:
			break;
	}

	return validMoves;
}

int Piece::getBoardIndex(const fb &bBoard, const sb &currentPos) {
	for (int i = 0; i < 12; i++) { if ((bBoard[i] & currentPos) != 0) { return i; } }

	return -1;
}

std::pair<uint8_t, sb> Piece::edgeCheck(const sb &position, const std::array<sb, 4> &bEdges) {\
	uint8_t edges{0};
	sb boundaries{0};

	// each bit represents each of the sides
	for (int i = 0; i < 4; i++) {
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

int Piece::getPieceIndexFromPosition(const pb &dPieces, const sb &currentPos) {
	for (int i = 0; i < 16; i++) { if ((dPieces[i].position & currentPos) != 0) { return i; } }

	return -1;
}

int Piece::getPieceIndexFromId(const pb &dPieces, const uint8_t &id) {
	for (int i = 0; i < 16; i++) { if (dPieces[i].id == id) { return i; } }

	return -1;
}

int Piece::boardToInt(sb board) {
	int i = 0;

	while (board > 0x1ULL) {
		board >>= 1;
		i++;
	}

	return i;
}

void Piece::updatePieceData(const fb &bBoard, sb &wAttackBoard, sb &bAttackBoard,
                            pb &wPieces, pb &bPieces,
                            ob &whiteObervers, ob &blackObservers,
                            const sb &prevPos, const sb &newPos) {
	const int bIndex = getBoardIndex(bBoard, prevPos);

	if (bIndex == -1) { std::cerr << "Invalid position sent to getValidMoves" << std::endl; }

	const bool color = bIndex % 2 == 0;

	pb &sColorPieces = color ? wPieces : bPieces;
	ob &sColorObservers = color ? whiteObervers : blackObservers;
	const int pIndex = getPieceIndexFromPosition(sColorPieces, prevPos);

	if (sColorPieces[pIndex].isSlider) {
		for (auto &vec: sColorObservers) {
			vec.erase(
				std::remove(vec.begin(), vec.end(), sColorPieces[pIndex].id), vec.end()
			);
		}
	}

	sColorPieces[pIndex].attacks = getAttacks(bBoard, newPos, true);
	sColorPieces[pIndex].position = newPos;

	const int prevIndex = boardToInt(prevPos);
	std::vector<uint8_t> idBuffer = whiteObervers[prevIndex];

	for (uint8_t id: idBuffer) {
		const int index = getPieceIndexFromId(wPieces, id);
		wPieces[index].attacks = getAttacks(bBoard, wPieces[index].position, false);
	}
	idBuffer = blackObservers[prevIndex];

	for (uint8_t id: idBuffer) {
		const int index = getPieceIndexFromId(bPieces, id);
		bPieces[index].attacks = getAttacks(bBoard, bPieces[index].position, false);
	}

	const int newIndex = boardToInt(newPos);
	idBuffer = blackObservers[newIndex];

	for (uint8_t id: idBuffer) {
		const int index = getPieceIndexFromId(bPieces, id);
		bPieces[index].attacks = getAttacks(bBoard, bPieces[index].position, false);
	}
	idBuffer = whiteObervers[newIndex];

	for (uint8_t id: idBuffer) {
		const int index = getPieceIndexFromId(wPieces, id);
		wPieces[index].attacks = getAttacks(bBoard, wPieces[index].position, false);
	}
}
