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
	// N value is the number of arms for sliders... lb[0] will be left, then it continues clockwise
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

enum class PieceType : int {
	PAWN = 0,
	BISHOP = 1,
	KNIGHT = 2,
	ROOK = 3,
	QUEEN = 4,
	KING = 5
};

enum class Color : int { BLACK = 0, WHITE = 1 };

class Piece {
public:
	static bool isMoveValid(const GameData &gd, const sb &currentPos, const sb &futurePos);
	static void move(GameData &gd, const sb &prevPos, const sb &newPos, const LookupTables &lt);

	template<size_t N>
	static void calculateSliderMoves(GameData &gd, PieceData &piece, const sb &pos, const int &bIndex,
	                                 const lb<N> &table);

	static void updateAtksAndObsvrs(GameData &gd, const sb &pos, PieceData &piece, const int &bIndex,
	                                const PieceType &pieceType, const LookupTables &lt);
	static void updateObservering(GameData &gd, const int &observingIndex, const int &bIndex,
	                              const LookupTables &lt);

	static int getBoardIndex(const fb &bBoard, const sb &currentPos);
	static sb getColorBoard(const fb &bBoard, const bool &color);

	static int getPieceIndex(const pb &dPieces, const sb &currentPos);
	static int getPieceIndex(const pb &dPieces, const uint8_t &id);

	static inline PieceType getPieceType(const int bIndex) { return static_cast<PieceType>(bIndex / 2); }
	static inline Color getColor(const int bIndex) { return static_cast<Color>(bIndex % 2); }
	static inline int boardToInt(const sb board) { return __builtin_ctzll(board); }
};
