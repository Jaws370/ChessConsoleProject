#pragma once

#include <array>
#include <bitset>

using sb = std::bitset<64>;
using fb = std::array<sb, 12>;

class Piece
{
public:
	static std::pair<bool, sb> move(const fb &bBoard, const sb &currentPos, const sb &futurePos);
	static sb getValidMoves(const fb &bBoard, const sb &currentPos);
	static int getBoardIndex(const fb &bBoard, const sb &currentPos);
	static std::pair<std::bitset<4>, sb> edgeCheck(const sb &position);
};
