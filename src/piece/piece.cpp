#include "Piece.hpp"

std::pair<bool, sb> Piece::move(const fb &bBoard, const sb &currentPos, const sb &futurePos) {

	if (const int subBoard = getBoardIndex(bBoard, currentPos); subBoard == -1) {
		return {false, {}};
	}

	bool isValidMove = false;



	return {isValidMove, {}};
}

int Piece::getBoardIndex(const fb &bBoard, const sb &currentPos) {
	for (int i = 0; i < 12; i++) {
		if ((bBoard[i] & currentPos) != 0) {
			return i;
		}
	}

	return -1;
}