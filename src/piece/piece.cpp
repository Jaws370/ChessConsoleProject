#include "Piece.hpp"

#include <algorithm>
#include <iostream>

bool Piece::isMoveValid(const GameData &gd, const sb &currentPos, const sb &futurePos) {
	const Color pieceColor = getColor(gd.whiteBoard, currentPos);

	const sb &sColorBoard = pieceColor == Color::WHITE ? gd.whiteBoard : gd.blackBoard;
	const pb &sColorPieces = pieceColor == Color::WHITE ? gd.whitePieces : gd.blackPieces;

	const int pIndex = gd.pieceLookup[boardToInt(currentPos)];

	if (pIndex == 255) {
		std::cerr << "Invalid piece sent to isMoveValid" << std::endl;
		return false;
	}

	const PieceData piece = sColorPieces[pIndex];

	// check for the pawn
	if (piece.type == PieceType::PAWN) {
		sb tempBoard = (pieceColor == Color::WHITE) ? currentPos << 8 : currentPos >> 8;
		const sb dColorBoard = pieceColor == Color::BLACK ? gd.whiteBoard : gd.blackBoard;

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
	if (futurePos & (piece.attacks & ~(sColorBoard))) { return true; }

	return false;
}

// assumes the move has already been validated
void Piece::move(GameData &gd, const sb &prevPos, const sb &newPos, const LookupTables &lt) {
	const Color pieceColor = getColor(gd.whiteBoard, prevPos);

	pb &sColorPieces = (pieceColor == Color::WHITE) ? gd.whitePieces : gd.blackPieces;
	ob &sColorObservers = (pieceColor == Color::WHITE) ? gd.whiteObservers : gd.blackObservers;

	const int pIndex = gd.pieceLookup[boardToInt(prevPos)];

	// return error if invalid piece
	if (pIndex == 255) {
		std::cerr << "Invalid piece sent to move" << std::endl;
		return;
	}

	// get the piece and board index
	PieceData &piece{sColorPieces[pIndex]};
	const int bIndex = piece.boardIndex;

	// return error if invalid board
	if (bIndex == -1) {
		std::cerr << "Invalid board index sent to move" << std::endl;
		return;
	}

	// remove piece's observers
	while (piece.observing) {
		const int observerIndex = __builtin_ctzll(piece.observing);
		sColorObservers[observerIndex].remove(piece.id);
		piece.observing &= ~(0x1ULL << observerIndex);
	}

	// update position of the piece
	piece.position = newPos;

	// update piece lookup
	gd.pieceLookup[boardToInt(prevPos)] = 255;
	gd.pieceLookup[boardToInt(newPos)] = pIndex;

	// update color board
	if (pieceColor == Color::WHITE) {
		gd.whiteBoard &= ~(prevPos);
		gd.whiteBoard |= newPos;
	} else {
		gd.blackBoard &= ~(prevPos);
		gd.blackBoard |= newPos;
	}

	// reset piece's attacks
	piece.attacks = 0;

	// update attacks and observers of the moved piece
	updateAtksAndObsvrs(gd, piece, lt);

	// update sliders observing prevPos
	const int prevIndex = boardToInt(prevPos);
	updateObservering(gd, prevIndex, lt);

	// update sliders observing newPos
	const int newIndex = boardToInt(newPos);
	updateObservering(gd, newIndex, lt);
}

void Piece::updateAtksAndObsvrs(GameData &gd, PieceData &piece, const LookupTables &lt) {
	switch (piece.type) {
		case PieceType::PAWN: {
			// move forward one
			const sb tempBoard = piece.color == Color::WHITE ? piece.position << 8 : piece.position >> 8;

			// add take left to attacks
			piece.attacks |= (tempBoard << 1);

			// add take right to attacks
			piece.attacks |= (tempBoard >> 1);

			return;
		} // pawn
		case PieceType::BISHOP: {
			calculateSliderMoves(gd, piece, piece.position, piece.color, lt.bishopLookupTable);
			return;
		} // bishop
		case PieceType::KNIGHT: {
			piece.attacks |= lt.knightLookupTable[boardToInt(piece.position)][0];
			return;
		} // knight
		case PieceType::ROOK: {
			calculateSliderMoves(gd, piece, piece.position, piece.color, lt.rookLookupTable);
			return;
		} // rook
		case PieceType::QUEEN: {
			calculateSliderMoves(gd, piece, piece.position, piece.color, lt.queenLookupTable);
			return;
		} // queen
		case PieceType::KING: {
			piece.attacks |= lt.kingLookupTable[boardToInt(piece.position)][0];
			return;
		} // king

		default: { std::cerr << "Invalid piece sent to makeMove" << std::endl; } // invalid piece
	}
}

void Piece::updateObservering(GameData &gd, const int &observingIndex, const LookupTables &lt) {
	ObserverData observing = gd.whiteObservers[observingIndex];
	for (int i = 0; i < observing.counter; i++) {
		PieceData &piece{gd.whitePieces[observing.ids[i]]};
		updateAtksAndObsvrs(gd, piece, lt);
	}

	observing = gd.blackObservers[observingIndex];
	for (int i = 0; i < observing.counter; i++) {
		PieceData &piece{gd.blackPieces[observing.ids[i]]};
		updateAtksAndObsvrs(gd, piece, lt);
	}
}

template<size_t N>
void Piece::calculateSliderMoves(GameData &gd, PieceData &piece, const sb &pos, const Color &pieceColor,
                                 const lb<N> &table) {
	const sb sColorBoard = pieceColor == Color::WHITE ? gd.whiteBoard : gd.blackBoard;
	const sb dColorBoard = pieceColor == Color::BLACK ? gd.whiteBoard : gd.blackBoard;

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

	// update the piece's observing data
	piece.observing = bObservers;

	// convert the observer board to the observer array
	while (bObservers) {
		const int observerIndex = __builtin_ctzll(bObservers);

		sColorObservers[observerIndex].add(piece.id);

		bObservers &= ~(0x1ULL << observerIndex);
	}
}
