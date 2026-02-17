#pragma once

#include <array>
#include <cstdint>

using sb = uint64_t;

enum class PieceType : int {
	PAWN = 0,
	BISHOP = 1,
	KNIGHT = 2,
	ROOK = 3,
	QUEEN = 4,
	KING = 5
};

enum class Color : int { BLACK = 0, WHITE = 1 };

struct PieceData {
	sb attacks;
	sb position;
	sb observing;
	uint8_t boardIndex;
	PieceType type;
	Color color;
	uint8_t id;
	bool isSlider;
};

struct ObserverData {
	std::array<uint8_t, 8> ids;
	int counter = 0;

	void add(const uint8_t &id) { ids[counter++] = id; }

	void remove(const uint8_t &id) {
		for (int i = 0; i < counter; i++) {
			if (ids[i] == id) {
				ids[i] = ids[--counter];
				return;
			}
		}
	}
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
using ob = std::array<ObserverData, 64>;

using lp = std::array<uint8_t, 64>;

struct GameData {
	sb whiteBoard;
	sb blackBoard;
	pb whitePieces;
	pb blackPieces;
	lp pieceLookup;
	ob whiteObservers;
	ob blackObservers;
};

class Piece {
public:
	static bool isMoveValid(const GameData &gd, const sb &currentPos, const sb &futurePos);
	static void move(GameData &gd, const sb &prevPos, const sb &newPos, const LookupTables &lt);

	template<size_t N>
	static void calculateSliderMoves(GameData &gd, PieceData &piece, const sb &pos, const Color &pieceColor,
	                                 const lb<N> &table);

	static void updateAtksAndObsvrs(GameData &gd, PieceData &piece, const LookupTables &lt);
	static void updateObservering(GameData &gd, const int &observingIndex, const LookupTables &lt);

	static inline Color getColor(const sb &whiteBoard, const sb &pos) {
		return (whiteBoard & pos) ? Color::WHITE : Color::BLACK;
	}

	static inline int boardToInt(const sb board) { return __builtin_ctzll(board); }
};
