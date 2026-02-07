#ifndef PIECE_HPP
#define PIECE_HPP

#include <array>
#include <bitset>

class Piece
{
public:
	static std::pair<bool, std::array<std::bitset<8>, 8>> move(const std::array<std::array<std::bitset<8>, 8>, 14> &bBoard, std::array<std::bitset<8>, 8> &currentPos, std::array<std::bitset<8>, 8> &futurePos);
	static std::array<std::bitset<8>, 8> getMoves(std::array<std::bitset<8>, 8> &currentPos);

private:
};

#endif
