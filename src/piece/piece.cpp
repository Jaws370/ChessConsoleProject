#include "Piece.hpp"

#include <algorithm>
#include <future>
#include <iostream>
#include <vector>

bool Piece::isMoveValid(const GameData &gd, const sb &currentPos, const sb &futurePos) {
	const int bIndex = getBoardIndex(gd.board, currentPos);

	if (bIndex == -1) {
		std::cerr << "Invalid position sent to isMoveValid" << std::endl;
		return false;
	}

	const Color pieceColor = getColor(bIndex);

	const sb &sColorBoard = getColorBoard(gd.board, bIndex % 2);
	const pb &sColorPieces = (pieceColor == Color::WHITE) ? gd.whitePieces : gd.blackPieces;
	const int pIndex = getPieceIndex(sColorPieces, currentPos);

	if (pIndex == -1) {
		std::cerr << "Invalid piece sent to isMoveValid" << std::endl;
		return false;
	}

	const PieceData pieceData = sColorPieces[pIndex];

	// check for the pawn
	const auto pieceType = getPieceType(bIndex);

	if (pieceType == PieceType::PAWN) {
		sb tempBoard = (pieceColor == Color::WHITE) ? currentPos << 8 : currentPos >> 8;
		const sb dColorBoard = getColorBoard(gd.board, !(bIndex % 2));

		// check take left
		if ((dColorBoard & (tempBoard << 1)) & futurePos) { return true; }

		// check take right
		if ((dColorBoard & (tempBoard >> 1)) & futurePos) { return true; }

		// check move forwards
		if (!(tempBoard & (sColorBoard | dColorBoard))) {
			if (tempBoard & futurePos) { return true; }

			// check double move forwards
			tempBoard = (pieceColor == Color::WHITE) ? tempBoard << 8 : tempBoard >> 8;

			const bool onStartingRank = (pieceColor == Color::WHITE)
				                            ? (currentPos & 0x000000000000FF00ULL)
				                            : (currentPos & 0x00FF000000000000ULL);

			const bool isPathClear = !(tempBoard & (sColorBoard | dColorBoard));

			if (onStartingRank && isPathClear && (tempBoard & futurePos)) { return true; }
		}

		return false;
	}

	// check for non-pawn
	if (futurePos & (pieceData.attacks & ~(sColorBoard))) { return true; }

	return false;
}

// assumes the move has already been validated
void Piece::move(GameData &gd, const sb &prevPos, const sb &newPos, const LookupTables &lt) {
	const int bIndex = getBoardIndex(gd.board, prevPos);

	// return error if invalid position
	if (bIndex == -1) {
		std::cerr << "Invalid position sent to move" << std::endl;
		return;
	}

	const Color pieceColor = getColor(prevPos);

	pb &sColorPieces = (pieceColor == Color::WHITE) ? gd.whitePieces : gd.blackPieces;
	ob &sColorObservers = (pieceColor == Color::WHITE) ? gd.whiteObservers : gd.blackObservers;
	const int pIndex = getPieceIndex(sColorPieces, prevPos);

	// return error if invalid piece
	if (pIndex == -1) {
		std::cerr << "Invalid piece sent to move" << std::endl;
		return;
	}

	// get the piece
	PieceData &piece{sColorPieces[pIndex]};

	// remove piece's observers
	if (piece.isSlider) { for (auto &vec: sColorObservers) { std::erase(vec, piece.id); } } // very slow

	// update position of the piece
	piece.position = newPos;

	// update bBoard
	gd.board[bIndex] = (gd.board[bIndex] & ~prevPos) | newPos;

	// reset piece's attacks
	piece.attacks = 0;

	// get the attacks and observers for each piece
	const PieceType pieceType = getPieceType(bIndex);

	// update attacks and observers of the moved piece
	updateAtksAndObsvrs(gd, prevPos, piece, bIndex, pieceType, lt);

	// update sliders observing prevPos
	const int prevIndex = boardToInt(prevPos);
	updateObservering(gd, prevIndex, bIndex, lt);

	// update sliders observing newPos
	const int newIndex = boardToInt(newPos);
	updateObservering(gd, newIndex, bIndex, lt);
}

void Piece::updateAtksAndObsvrs(GameData &gd, const sb &pos, PieceData &piece, const int &bIndex,
                                const PieceType &pieceType, const LookupTables &lt) {
	switch (pieceType) {
		case PieceType::PAWN: {
			// move forward one
			const sb tempBoard = bIndex % 2 == 0 ? pos >> 8 : pos << 8;

			// add take left to attacks
			piece.attacks |= (tempBoard << 1);

			// add take right to attacks
			piece.attacks |= (tempBoard >> 1);

			return;
		} // pawn
		case PieceType::BISHOP: {
			calculateSliderMoves(gd, piece, pos, bIndex, lt.bishopLookupTable);
			return;
		} // bishop
		case PieceType::KNIGHT: {
			piece.attacks |= lt.knightLookupTable[boardToInt(pos)][0];
			return;
		} // knight
		case PieceType::ROOK: {
			calculateSliderMoves(gd, piece, pos, bIndex, lt.rookLookupTable);
			return;
		} // rook
		case PieceType::QUEEN: {
			calculateSliderMoves(gd, piece, pos, bIndex, lt.queenLookupTable);
			return;
		} // queen
		case PieceType::KING: {
			piece.attacks |= lt.kingLookupTable[boardToInt(pos)][0];
			return;
		} // king

		default: { std::cerr << "Invalid piece sent to makeMove" << std::endl; } // invalid piece
	}
}

void Piece::updateObservering(GameData &gd, const int &observingIndex, const int &bIndex,
                              const LookupTables &lt) {
	std::vector<uint8_t> observing = gd.whiteObservers[observingIndex];
	for (auto &pieceId: observing) {
		const int pIndex = getPieceIndex(gd.whitePieces, pieceId);
		PieceData &piece{gd.whitePieces[pIndex]};
		updateAtksAndObsvrs(gd, piece.position, piece, bIndex, static_cast<PieceType>(bIndex / 2), lt);
	}

	observing = gd.blackObservers[observingIndex];
	for (auto &pieceId: observing) {
		const int pIndex = getPieceIndex(gd.blackPieces, pieceId);
		PieceData &piece{gd.blackPieces[pIndex]};
		updateAtksAndObsvrs(gd, piece.position, piece, bIndex, static_cast<PieceType>(bIndex / 2), lt);
	}
}

template<size_t N>
void Piece::calculateSliderMoves(GameData &gd, PieceData &piece, const sb &pos, const int &bIndex,
                                 const lb<N> &table) {
	const Color pieceColor = getColor(bIndex);

	const sb sColorBoard = getColorBoard(gd.board, bIndex % 2);
	const sb dColorBoard = getColorBoard(gd.board, !(bIndex % 2));

	sb bObservers = 0;
	piece.attacks = 0;

	// go through each arm
	for (auto arm: table[boardToInt(pos)]) {
		// calculate the hits on the arm
		sb hits = (sColorBoard | dColorBoard) & arm;

		// return full arm if no hits
		if (hits == 0) {
			piece.attacks |= arm;
			bObservers |= arm;
			continue;
		}

		int sigBitIndex = 0;

		sb mask = 0;

		// get msb or lsb and build masks
		if (arm > pos) {
			sigBitIndex = __builtin_ctzll(hits);

			mask = ((0x1ULL << (sigBitIndex + 1)) - 1); // mask keeping hit
		} else {
			sigBitIndex = 63 - __builtin_clzll(hits);

			mask = ~((0x1ULL << sigBitIndex) - 1); // mask keeping hit
		}

		// add arm to attacks with the hit
		piece.attacks |= (arm & mask);


		// second hits for observers and x-ray attacks
		hits = (hits & ~(0x1ULL << sigBitIndex));

		// if the second check doesn't hit
		if (hits == 0) {
			bObservers |= arm;
			continue;
		}

		mask = 0;

		// get msb and lsb for the second hit
		if (arm > pos) {
			sigBitIndex = __builtin_ctzll(hits);
			mask = ((0x1ULL << (sigBitIndex + 1)) - 1); // mask keeping hit
		} else {
			sigBitIndex = 63 - __builtin_clzll(hits);
			mask = ~((0x1ULL << sigBitIndex) - 1); // mask keeping hit
		}

		// update observers
		bObservers |= (arm & mask);
	}

	ob &sColorObservers = (pieceColor == Color::WHITE) ? gd.whiteObservers : gd.blackObservers;

	// convert the observer board to the observer array
	while (bObservers) {
		const int observerIndex = __builtin_ctzll(bObservers);

		sColorObservers[observerIndex].push_back(piece.id);

		bObservers &= ~(0x1ULL << observerIndex);
	}
}

int Piece::getBoardIndex(const fb &bBoard, const sb &currentPos) {
	for (int i = 0; i < 12; i++) { if ((bBoard[i] & currentPos) != 0) { return i; } }
	return -1;
}

sb Piece::getColorBoard(const fb &bBoard, const bool &color) {
	sb outBoard{0};
	for (int i = color; i < 12; i += 2) { outBoard |= bBoard[i]; }
	return outBoard;
}

int Piece::getPieceIndex(const pb &dPieces, const sb &currentPos) {
	for (int i = 0; i < 16; i++) { if (dPieces[i].position & currentPos) { return i; } }
	return -1;
}

int Piece::getPieceIndex(const pb &dPieces, const uint8_t &id) {
	for (int i = 0; i < 16; i++) { if (dPieces[i].id == id) { return i; } }
	return -1;
}
