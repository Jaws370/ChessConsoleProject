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
	sb valid_moves; // the moves that the piece can make
	sb attacks; // the board representing all attacks of a piece
	sb position; // the board representing the position of the piece
	piece_type type;
	piece_color color;
	uint8_t id; // the position of the piece in the piece arrays
	bool is_pinned;

	piece_data() : valid_moves(0), attacks(0), position(0), type(piece_type::EMPTY), color(piece_color::NONE), id(0),
	               is_pinned(false) {}

	void set(const piece_type type, const piece_color color, const uint8_t id) {
		attacks = position = 0;
		this->type = type;
		this->color = color;
		this->id = id;
	} // must refetch attacks / position / observing

	void reset() {
		attacks = position = 0;
		color = piece_color::NONE;
		type = piece_type::EMPTY;
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

// arms between all two positions on the board
// should only include the 8 cardinal directions with all other positons being sb = 0xFFFFFFFFFFFFFFFFULL
using between_tables = std::array<std::array<sb, 64>, 64>;

struct game_data {
	sb white_board;
	sb black_board;
	std::array<uint8_t, 64> piece_lookup;
	std::array<piece_data, 16> white_pieces; // the last piece must be king
	std::array<piece_data, 16> black_pieces; // the last piece must be king
	std::array<uint8_t, 64> pins;

	bool is_move_valid(sb current_pos, sb future_pos, const lookup_tables &lt) const;
	bool move(const sb prev_pos, const sb new_pos, const lookup_tables &lt, const between_tables &bt);

private:
	static int sb_to_int(const sb board) { return __builtin_ctzll(board); }

	piece_color get_color(const sb pos) const {
		if (pos & white_board) { return piece_color::WHITE; }
		if (pos & black_board) { return piece_color::BLACK; }
		return piece_color::NONE;
	}

	std::pair<sb, sb> game_data::get_boards(const piece_color color) const {
		return color == piece_color::WHITE
			       ? std::pair{white_board, black_board}
			       : std::pair{black_board, white_board};
	}

	/*
	bool checking_checks(const piece_data &piece, sb current_pos, sb future_pos,
	                     const lookup_tables &lt) const; */

	void capture(const piece_data &piece, sb new_pos);

	void update_attacks(piece_data &piece, sb friendly_board, sb enemy_board,
	                    const lookup_tables &lt) const;

	int get_observers(const piece_data &piece, sb friendly_board, sb enemy_board, const lb<8> &table,
	                  std::array<piece_data *, 8> &observers);

	template<size_t N>
	void update_sliders(piece_data &piece, sb friendly_board, sb enemy_board, const lb<N> &table) const;
};
