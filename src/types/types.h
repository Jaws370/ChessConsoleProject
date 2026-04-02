#pragma once

#include <array>
#include <cstdint>

using sb = uint64_t; // represents each square on the board as a single bit

enum class piece_color : int { BLACK = 0, WHITE = 1, NONE = -1 };

enum class piece_type : int {
	PAWN = 0,
	BISHOP = 1,
	KNIGHT = 2,
	ROOK = 3,
	QUEEN = 4,
	KING = 5,
	EMPTY = -1
};

struct piece_data {
	sb position; // the board representing the position of the piece
	sb attacks; // the board representing all attacks of a piece
	piece_type type;
	piece_color color;
	uint8_t id; // the position of the piece in the piece arrays
	uint8_t pinner_id; // the id if the piece pinning this piece... 255 if no pin
	bool is_slider;

	piece_data() : position(0), attacks(0), type(piece_type::EMPTY), color(piece_color::NONE), id(0),
	               pinner_id(255), is_slider(false) {}

	void set(const piece_type type, const piece_color color, const uint8_t id) {
		attacks = position = 0;
		this->type = type;
		this->color = color;
		this->id = id;

		if (type == piece_type::BISHOP || type == piece_type::ROOK || type == piece_type::QUEEN) { is_slider = true; }
	}

	void reset() {
		position = attacks = 0;
		type = piece_type::EMPTY;
		color = piece_color::NONE;
		pinner_id = 255;
		is_slider = false;
	}
};

template<size_t N>
using lb = std::array<std::array<sb, N>, 64>;
// represents the lookup table of length 64 for each square and N size for the number of arms

// each entry is indexed by a square (0-63), containing N arms.
// arms are ordered: left, then clockwise
struct lookup_tables {
	lb<4> bishop_table;
	lb<1> knight_table;
	lb<4> rook_table;
	lb<8> queen_table;
	lb<1> king_table;
};

// arms between all two positions on the board including the start and end
// should only include the 8 cardinal directions with all other positons being sb = 0x0ULL
using between_tables = std::array<std::array<sb, 64>, 64>;
