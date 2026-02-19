#pragma once

#include <array>
#include <cstdint>

using sb = uint64_t; // represents each square on the board as a single bit

enum class piece_color : int { BLACK = 0, WHITE = 1, NONE = NULL };

enum class piece_type : int {
	PAWN = 0,
	BISHOP = 1,
	KNIGHT = 2,
	ROOK = 3,
	QUEEN = 4,
	KING = 5,
	EMPTY = NULL
};

struct piece_data {
	sb attacks; // the board representing all attacks of a piece
	sb position; // the board representing the position of the piece
	sb observing; // the board representing all the square the piece is observing
	piece_type type;
	piece_color color;
	uint8_t id;
	bool is_slider;

	piece_data() : attacks(0), position(0), observing(0), type(piece_type::EMPTY), color(piece_color::NONE), id(0),
	               is_slider(false) {}

	void set(const piece_type type, const piece_color color, const uint8_t id, const bool is_slider) {
		attacks = position = observing = 0;
		this->type = type;
		this->color = color;
		this->id = id;
		this->is_slider = is_slider;
	} // must refetch attacks / position / observing

	void reset() {
		attacks = position = observing = 0;
		color = piece_color::NONE;
		type = piece_type::EMPTY;
		is_slider = false;
	}
};

struct observer_data {
	std::array<uint8_t, 8> ids;
	int length = 0;

	void add(const uint8_t id) { ids[length++] = id; }

	void remove(const uint8_t id) {
		for (int i = 0; i < length; i++) {
			if (ids[i] == id) {
				ids[i] = ids[--length];
				return;
			}
		}
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

struct game_data {
	sb white_board;
	sb black_board;
	std::array<uint8_t, 64> piece_lookup;
	std::array<piece_data, 16> white_pieces;
	std::array<piece_data, 16> black_pieces;
	std::array<observer_data, 64> white_observers;
	std::array<observer_data, 64> black_observers;

	bool is_move_valid(const sb current_pos, const sb future_pos) const;
	void move(const sb prev_pos, const sb new_pos, const lookup_tables &lt);

private:
	static inline int sb_to_int(const sb board) { return __builtin_ctzll(board); }

	inline piece_color get_color(const sb pos) const {
		return (white_board & pos) ? piece_color::WHITE : piece_color::BLACK;
	}

	void capture(const piece_data &piece);

	void update_attacks(piece_data &piece, const lookup_tables &lt);
	void update_observers(const int observer_index, const lookup_tables &lt);

	template<size_t N>
	void update_sliders(piece_data &piece, const lb<N> &table);
};
