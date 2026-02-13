#pragma once

#include <array>
#include <bitset>

using sb = std::bitset<64>;
using fb = std::array<sb, 12>;

class Piece
{
public:
	static std::pair<bool, sb> move(const fb &bBoard, const sb &currentPos, const sb &futurePos);
	static sb getMoves(const fb &bBoard, sb &currentPos);
	static int getBoardIndex(const fb &bBoard, const sb &currentPos);

private:
};
