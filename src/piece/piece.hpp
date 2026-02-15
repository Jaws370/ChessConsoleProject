#pragma once

#include <array>
#include <cstdint>
#include <vector>

using sb = uint64_t;
using fb = std::array<sb, 12>;

struct PieceData {
	sb attacks;
	sb position;
	uint8_t boardIndex;
	uint8_t id;
	bool isSlider;
};

using pb = std::array<PieceData, 16>;
using ob = std::array<std::vector<uint8_t>, 64>;

class Piece {
public:
	static std::pair<bool, sb> move(const fb &bBoard, const sb &currentPos, const sb &futurePos);
	static sb getValidMoves(const fb &bBoard, const sb &currentPos);
	static sb getDirection(const fb &bBoard, const sb &boundaries, const sb &currentPos, const int &shift,
	                       const bool &direction, const bool &color);

	static std::pair<uint8_t, sb> edgeCheck(const sb &position, const std::array<sb, 4> &bEdges);
	static int getBoardIndex(const fb &bBoard, const sb &currentPos);
	static sb getColorBoard(const fb &bBoard, const bool &color);

	static int getPieceIndexFromPosition(const pb &dPieces, const sb &currentPos)
	static int getPieceIndexFromId(const pb &dPieces, const uint8_t &id);
	static int boardToInt(sb board);
	static void updatePieceData(const fb &bBoard, sb &wAttackBoard, sb &bAttackBoard,
	                            pb &wPieces, pb &bPieces,
	                            ob &whiteObervers, ob &blackObservers,
	                            const sb &prevPos, const sb &newPos);
	static void updateAttackBoards();
	static sb getAttacks(const fb &bBoard, const sb &currentPos, const bool &isObservingNew);
	static sb getAttackDirection(const fb &bBoard, const sb &boundaries, const sb &currentPos, const int &shift,
	                             const bool &direction, const bool &color);
};
