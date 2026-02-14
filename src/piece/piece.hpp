#pragma once

#include <array>
#include <cstdint>

using sb = uint64_t;
using fb = std::array<sb, 12>;

class Piece {
public:
	static std::pair<bool, sb> move(const fb &bBoard, const sb &currentPos, const sb &futurePos);
	static sb getValidMoves(const fb &bBoard, const sb &currentPos);
	static int getBoardIndex(const fb &bBoard, const sb &currentPos);
	static std::pair<uint8_t, sb> edgeCheck(const sb &position);
	static sb getDirection(const fb &bBoard, const sb &boundaries, const sb &currentPos, const int &shift,
	                       const bool &direction, const bool &color);
	static sb getColorBoard(const fb &bBoard, const bool &color);
};
