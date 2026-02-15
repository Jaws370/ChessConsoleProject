#include "Piece.hpp"

#include <algorithm>
#include <future>
#include <iostream>
#include <vector>

/**
 * Attempts to move a chess piece from the current position to a future position on the board.
 * This method performs validation on the move, updates game data accordingly, and handles
 * the logic for moves, attacks, and observers based on the type of piece.
 *
 * @param gd The current game data, including the board state and piece information.
 * @param currentPos The current position of the piece represented as a bitboard.
 * @param futurePos The desired future position of the piece represented as a bitboard.
 * @param lt The lookup tables providing precomputed data for various piece movements.
 * @return `true` if the move is successfully completed; `false` if the move is invalid.
 */
bool Piece::move(GameData &gd, const sb &currentPos, const sb &futurePos, LookupTables lt) {
	// get the piece's board index
	const int bIndex = getBoardIndex(gd.board, currentPos);

	// return error if invalid position
	if (bIndex == -1) {
		std::cerr << "Invalid position sent to move" << std::endl;
		return {false};
	}

	// boards for the possible moves, attacks, and observers
	sb possibleMoves = 0;
	sb possibleAttacks = 0;
	sb possibleObservers = 0;

	// get the moves, attacks, and observers for each piece
	lb table = 0;
	switch (bIndex / 2) {
		case 0: {
			// move forward one
			sb tempBoard = bIndex % 2 == 0 ? currentPos << 8 : currentPos >> 8;
			const sb sColorBoard = getColorBoard(gd.board, bIndex % 2);
			const sb dColorBoard = getColorBoard(gd.board, !(bIndex % 2));

			// check take left and add to attacks
			if ((dColorBoard & (tempBoard << 1)) != 0) { possibleMoves |= (tempBoard << 1); }
			possibleAttacks |= (tempBoard << 1);

			// check take right and add to attacks
			if ((dColorBoard & (tempBoard >> 1)) != 0) { possibleMoves |= (tempBoard >> 1); }
			possibleAttacks |= (tempBoard >> 1);

			// check move forwards
			if ((tempBoard & (sColorBoard | dColorBoard)) == 0) {
				possibleMoves |= tempBoard;

				// check double move forwards
				tempBoard = bIndex % 2 == 0 ? tempBoard << 8 : tempBoard >> 8;
				if (((bIndex % 2 && (currentPos & 0x00FF000000000000ULL) != 0) ||
				     (!(bIndex % 2) && (currentPos & 0x000000000000FF00ULL) != 0)) && (
					    tempBoard & (sColorBoard | dColorBoard)) == 0) { possibleMoves |= tempBoard; }
			}
			goto end;
		} // pawn
		case 1: {
			table = lt.bishopLookupTable;
			goto slider;
		} // bishop
		case 2: {
			table = lt.knightLookupTable;
			goto nonSlider;
		} // knight
		case 3: {
			table = lt.rookLookupTable;
			goto slider;
		} // rook
		case 4: {
			table = lt.queenLookupTable;
			goto slider;
		} // queen
		case 5: {
			table = lt.kingLookupTable;
			goto nonSlider;
		} // king

		default: {
			std::cerr << "Invalid piece sent to makeMove" << std::endl;
			return false;
		} // invalid piece
	}

slider: {
		// send data and lookuptables to calculateSliderMoves()
		calculateSliderMoves(gd.board, bIndex, possibleMoves, possibleAttacks, possibleObservers, currentPos, table);
		goto end;
	}

nonSlider: {
		// get the board of the piece and then use it to update attacks and moves
		const sb sColorBoard = getColorBoard(gd.board, bIndex % 2);

		possibleAttacks |= table[boardToInt(currentPos)][0];
		possibleMoves |= (table[boardToInt(currentPos)][0] & ~(sColorBoard));
	}

end:
	// return false if the move is invalid
	if ((possibleMoves & futurePos) != 0) { return {false}; }

	pb &sColorPieces = bIndex % 2 ? gd.whitePieces : gd.blackPieces;
	ob &sColorObservers = bIndex % 2 ? gd.whiteObservers : gd.blackObservers;
	const int pIndex = getPieceIndexFromPosition(sColorPieces, currentPos);

	// update position and attacks of the piece
	sColorPieces[pIndex].position = futurePos;
	sColorPieces[pIndex].attacks = possibleAttacks;

	// observer stuff for sliders
	if (sColorPieces[pIndex].isSlider) {
		for (auto &vec: sColorObservers) { std::erase(vec, sColorPieces[pIndex].id); }

		while (possibleObservers != 0) {
			const int observerIndex = __builtin_ctzll(possibleObservers);

			sColorObservers[observerIndex].push_back(sColorPieces[pIndex].id);

			possibleObservers &= ~(0x1ULL << observerIndex);
		}
	}

	// update sliders observing currentPos
	const int prevIndex = boardToInt(currentPos);
	updatePieceAttacks(gd.board, gd.whitePieces, gd.whiteObservers[prevIndex], gd.whiteObservers);
	updatePieceAttacks(gd.board, gd.blackPieces, gd.blackObservers[prevIndex], gd.blackObservers);

	// update sliders observing futurePos
	const int newIndex = boardToInt(futurePos);
	updatePieceAttacks(gd.board, gd.whitePieces, gd.whiteObservers[newIndex], gd.whiteObservers);
	updatePieceAttacks(gd.board, gd.blackPieces, gd.blackObservers[newIndex], gd.blackObservers);

	// update bBoard
	gd.board[bIndex] = (gd.board[bIndex] & ~currentPos) | futurePos;

	// return result
	return {true};
}

/**
 * Calculates the moves for the slider pieces
 *
 * @tparam N size n of the input lookuptable
 * @param bBoard the current board of the game
 * @param bIndex index of the board being edited
 * @param possibleMoves variable containing the possible moves
 * @param possibleAttacks variable containing the possible attacks
 * @param possibleObservers variable containing the possible observers
 * @param currentPos the current board position
 * @param table the lookuptable for this piece
 */
template<size_t N>
void Piece::calculateSliderMoves(const fb &bBoard, const int bIndex, sb &possibleMoves, sb &possibleAttacks,
                                 sb &possibleObservers, const sb &currentPos, const lb<N> &table) {
	const sb sColorBoard = getColorBoard(bBoard, bIndex % 2);
	const sb dColorBoard = getColorBoard(bBoard, !(bIndex % 2));

	// go through each arm
	for (auto arm: table[boardToInt(currentPos)]) {
		// calculate the hits on the arm
		sb hits = (sColorBoard | dColorBoard) & arm;

		// return full arm if no hits
		if (hits == 0) {
			possibleMoves |= arm;
			possibleAttacks |= arm;
			possibleObservers |= arm;
			continue;
		}

		int sigBitIndex = 0;

		sb hitMask = 0;
		sb hitlessMask = 0;

		// get msb or lsb and build masks
		if (arm > currentPos) {
			sigBitIndex = __builtin_ctzll(hits);

			hitlessMask = ((0x1ULL << sigBitIndex) - 1); // mask removing hit
			hitMask = ((0x1ULL << (sigBitIndex + 1)) - 1); // mask keeping hit
		} else {
			sigBitIndex = 63 - __builtin_clzll(hits);

			hitlessMask = ~((0x1ULL << (sigBitIndex + 1)) - 1); // mask removing hit
			hitMask = ~((0x1ULL << sigBitIndex) - 1); // mask keeping hit
		}

		// make moves be hitless if hit piece is same color
		if ((hitMask & sColorBoard) != 0) { possibleMoves |= (arm & hitlessMask); } else {
			possibleMoves |= (arm & hitMask);
		}

		// add arm to attacks with the hit
		possibleAttacks |= (arm & hitMask);


		// second hits for observers and x-ray attacks
		hits = (hits & ~(0x1ULL << sigBitIndex));

		// if the second check doesn't hit
		if (hits == 0) {
			possibleObservers |= arm;
			continue;
		}

		hitMask = 0;

		// get msb and lsb for the second hit
		if (arm > currentPos) {
			sigBitIndex = __builtin_ctzll(hits);
			hitMask = ((0x1ULL << (sigBitIndex + 1)) - 1); // mask keeping hit
		} else {
			sigBitIndex = 63 - __builtin_clzll(hits);
			hitMask = ~((0x1ULL << sigBitIndex) - 1); // mask keeping hit
		}

		// update observers
		possibleObservers |= (arm & hitMask);
	}
}

// precalculatedMove

// still need for AI
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
						validMoves |= getDirection(bBoard, boundaries, currentPos, directions[j], i == 0,
						                           bIndex % 2);
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
						validMoves |= getDirection(bBoard, boundaries, currentPos, directions[j], i == 0,
						                           bIndex % 2);
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
						validMoves |= getDirection(bBoard, boundaries, currentPos, directions[j], i == 0,
						                           bIndex % 2);
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

std::pair<uint8_t, sb> Piece::edgeCheck(const sb &position, const std::array<sb, 4> &bEdges) {
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

void Piece::updatePieceData(const fb &bBoard,
                            pb &wPieces, pb &bPieces,
                            ob &whiteObservers, ob &blackObservers,
                            const sb &prevPos, const sb &newPos) {
	const int bIndex = getBoardIndex(bBoard, prevPos);

	if (bIndex == -1) { std::cerr << "Invalid position sent to updatePieceData" << std::endl; }

	const bool color = bIndex % 2 == 0;

	pb &sColorPieces = color ? wPieces : bPieces;
	ob &sColorObservers = color ? whiteObservers : blackObservers;
	const int pIndex = getPieceIndexFromPosition(sColorPieces, prevPos);

	if (sColorPieces[pIndex].isSlider) {
		for (auto &vec: sColorObservers) { std::erase(vec, sColorPieces[pIndex].id); }
	}

	sColorPieces[pIndex].attacks = getAttacks(bBoard, newPos, sColorPieces[pIndex].id, sColorObservers);
	sColorPieces[pIndex].position = newPos;

	const int prevIndex = boardToInt(prevPos);
	updatePieceAttacks(bBoard, wPieces, whiteObservers[prevIndex], whiteObservers);
	updatePieceAttacks(bBoard, bPieces, blackObservers[prevIndex], blackObservers);

	const int newIndex = boardToInt(newPos);
	updatePieceAttacks(bBoard, wPieces, whiteObservers[newIndex], whiteObservers);
	updatePieceAttacks(bBoard, bPieces, blackObservers[newIndex], blackObservers);
}

void Piece::updatePieceAttacks(const fb &bBoard, pb &pieces, const std::vector<uint8_t> &observers,
                               ob &sColorObservers) {
	for (uint8_t id: observers) {
		const int index = getPieceIndexFromId(pieces, id);
		pieces[index].attacks = getAttacks(bBoard, pieces[index].position, id, sColorObservers);
	}
}

sb Piece::getAttacks(const fb &bBoard, const sb &currentPos, const uint8_t &pieceId, ob &oBoard) {
	const int bIndex = getBoardIndex(bBoard, currentPos);
	if (bIndex == -1) { std::cerr << "Invalid position sent to getValidMoves" << std::endl; }

	sb attacks{0};

	switch (bIndex / 2) {
		case 0: {
			const sb tempBoard = bIndex % 2 == 0 ? currentPos << 8 : currentPos >> 8;

			// take left
			attacks |= (tempBoard << 1);
			// take right
			attacks |= (tempBoard >> 1);

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
						attacks |= getAttackDirection(bBoard, boundaries, currentPos, directions[j], i == 0,
						                              bIndex % 2,
						                              oBoard, pieceId);
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

			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 4; j++) {
					if ((validDirections & (0x80 >> ((i * 4) + j))) != 0) {
						constexpr std::array<int, 4> directions{6, 10, 15, 17};

						attacks |= i == 0 ? currentPos << directions[j] : currentPos >> directions[j];
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
						attacks |= getAttackDirection(bBoard, boundaries, currentPos, directions[j], i == 0,
						                              bIndex % 2,
						                              oBoard, pieceId);
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

			uint8_t validDirections{0xFF}; // MSB is r, tr, t, tl, l, bl, b, br

			// top edge
			if ((0x8 & edges) == 0) {
				validDirections &= ~0x70; // tl, t, tr
			}

			// right edge
			if ((0x4 & edges) == 0) {
				validDirections &= ~0xC1; // tr, r, br
			}

			// bottom edge
			if ((0x2 & edges) == 0) {
				validDirections &= ~0x7; // br, b, bl
			}

			// left edge
			if ((0x1 & edges) == 0) {
				validDirections &= ~0x1C; // bl, l, tl
			}

			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 4; j++) {
					if ((validDirections & (0x80 >> ((i * 4) + j))) != 0) {
						constexpr std::array<int, 4> directions{1, 9, 8, 7};
						attacks |= getAttackDirection(bBoard, boundaries, currentPos, directions[j], i == 0,
						                              bIndex % 2,
						                              oBoard, pieceId);
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

			uint8_t validDirections{0xFF}; // MSB is r, tr, t, tl, l, bl, b, br

			// top edge
			if ((0x8 & edges) == 0) {
				validDirections &= ~0x70; // tl, t, tr
			}

			// right edge
			if ((0x4 & edges) == 0) {
				validDirections &= ~0xC1; // tr, r, br
			}

			// bottom edge
			if ((0x2 & edges) == 0) {
				validDirections &= ~0x7; // br, b, bl
			}

			// left edge
			if ((0x1 & edges) == 0) {
				validDirections &= ~0x1C; // bl, l, tl
			}

			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 4; j++) {
					if ((validDirections & (0x80 >> ((i * 4) + j))) != 0) {
						constexpr std::array<int, 4> directions{1, 9, 8, 7};

						attacks |= i == 0 ? currentPos << directions[j] : currentPos >> directions[j];
					}
				}
			}

			break;
		} // king
		default:
			break;
	}

	return attacks;
}

sb Piece::getAttackDirection(const fb &bBoard, const sb &boundaries, const sb &currentPos, const int &shift,
                             const bool &direction, const bool &color, ob &oBoard, const uint8_t &pieceId) {
	sb outBoard{0};
	sb tempBoard{currentPos};

	const sb fBoard{getColorBoard(bBoard, color) | getColorBoard(bBoard, !color)};

	bool hasSeenPiece = false;

	for (int i = 0; i < 8; i++) {
		tempBoard = direction ? tempBoard << shift : tempBoard >> shift;

		if (!hasSeenPiece) { outBoard |= tempBoard; }

		oBoard[boardToInt(tempBoard)].push_back(pieceId);

		if ((tempBoard & fBoard) != 0) {
			if (hasSeenPiece) { return outBoard; }
			hasSeenPiece = true;
			continue;
		}

		if ((tempBoard & boundaries) != 0) { return outBoard; }
	}

	return outBoard;
}
