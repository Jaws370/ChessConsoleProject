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

template<size_t N>
using lb = std::array<std::array<sb, N>, 64>;

struct LookupTables {
	// N value is the number of arms for sliders... lt[0] will be left, then it continues clockwise
	lb<4> bishopLookupTable;
	lb<1> knightLookupTable;
	lb<4> rookLookupTable;
	lb<8> queenLookupTable;
	lb<1> kingLookupTable;
};

using pb = std::array<PieceData, 16>;
using ob = std::array<std::vector<uint8_t>, 64>;

struct GameData {
	fb board;
	pb whitePieces;
	pb blackPieces;
	ob whiteObservers;
	ob blackObservers;
};

class Piece {
public:
	static std::pair<bool, sb> move(const fb &bBoard, const sb &currentPos, const sb &futurePos);
	static sb getValidMoves(const fb &bBoard, const sb &currentPos);
	static sb getDirection(const fb &bBoard, const sb &boundaries, const sb &currentPos, const int &shift,
	                       const bool &direction, const bool &color);

	static std::pair<uint8_t, sb> edgeCheck(const sb &position, const std::array<sb, 4> &bEdges);
	static int getBoardIndex(const fb &bBoard, const sb &currentPos);
	static sb getColorBoard(const fb &bBoard, const bool &color);

	template<size_t N>
	static void calculateSliderMoves(const fb &bBoard, const int bIndex, sb &possibleMoves, sb &possibleAttacks,
	                                 sb &possibleObservers, const sb &currentPos, const lt<N> &table);

	static int getPieceIndexFromPosition(const pb &dPieces, const sb &currentPos)
	static int getPieceIndexFromId(const pb &dPieces, const uint8_t &id);
	static int boardToInt(sb board);
	static void updatePieceData(const fb &bBoard,
	                            pb &wPieces, pb &bPieces,
	                            ob &whiteObservers, ob &blackObservers,
	                            const sb &prevPos, const sb &newPos);
	static void updatePieceAttacks(const fb &bBoard, pb &pieces, const std::vector<uint8_t> &observers,
	                               ob &sColorObservers);
	static void updateAttackBoards();
	static sb getAttacks(const fb &bBoard, const sb &currentPos, const uint8_t &pieceId, ob &oBoard);
	static sb getAttackDirection(const fb &bBoard, const sb &boundaries, const sb &currentPos, const int &shift,
	                             const bool &direction, const bool &color, ob &oBoard, const uint8_t &pieceId);
};
