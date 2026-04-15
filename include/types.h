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
	bool has_moved;
	int value;

	piece_data(const uint64_t position, const piece_type type, const piece_color color,
	           const uint8_t id) : position(position), attacks(0), type(type), color(color), id(id),
	                               pinner_id(255), is_slider(false), has_moved(false), value(0) {
		if (type == piece_type::BISHOP || type == piece_type::ROOK || type == piece_type::QUEEN) { is_slider = true; }

		switch (type) {
			case piece_type::PAWN: {
				value = 1;
				break;
			}
			case piece_type::BISHOP: {
				value = 3;
				is_slider = true;
				break;
			}
			case piece_type::KNIGHT: {
				value = 3;
				break;
			}
			case piece_type::ROOK: {
				value = 5;
				is_slider = true;
				break;
			}
			case piece_type::QUEEN: {
				value = 8;
				is_slider = true;
				break;
			}
			default: break;
		}
	}

	piece_data() : position(0), attacks(0), type(piece_type::EMPTY), color(piece_color::NONE), id(255), pinner_id(255),
	               is_slider(false), value(0) {};

	void set(const uint64_t position, const piece_type type, const piece_color color, const uint8_t id) {
		attacks = 0;
		this->position = position;
		this->type = type;
		this->color = color;
		this->id = id;

		if (type == piece_type::BISHOP || type == piece_type::ROOK || type == piece_type::QUEEN) { is_slider = true; }

		switch (type) {
			case piece_type::PAWN: {
				value = 1;
				break;
			}
			case piece_type::BISHOP: {
				value = 3;
				is_slider = true;
				break;
			}
			case piece_type::KNIGHT: {
				value = 3;
				break;
			}
			case piece_type::ROOK: {
				value = 5;
				is_slider = true;
				break;
			}
			case piece_type::QUEEN: {
				value = 8;
				is_slider = true;
				break;
			}
			default: break;
		}
	}

	void reset() {
		position = attacks = 0;
		type = piece_type::EMPTY;
		color = piece_color::NONE;
		pinner_id = 255;
		is_slider = false;
		value = 0;
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

// struct for the two game tables
struct table_bundle {
	between_tables between_table{};
	lookup_tables lookup_table{};

	table_bundle() {
		std::array<std::array<sb, 8>, 64> default_table{};

		// generate all the arms starting left then going clockwise
		for (int i{0}; i < 64; i++) {
			const sb pos{sb{1} << i};
			constexpr std::array directions{1, 9, 8, 7, -1, -9, -8, -7};

			// edge masks
			constexpr sb top_edge{0xFF00000000000000ULL};
			constexpr sb bottom_edge{0x00000000000000FFULL};
			constexpr sb left_edge{0x8080808080808080ULL};
			constexpr sb right_edge{0x0101010101010101ULL};
			sb edges{top_edge | bottom_edge | right_edge | left_edge};

			std::array<int, 8> direction_skip_set{};

			if (pos & top_edge) {
				edges &= ~(top_edge & ~0x8100000000000000ULL);
				direction_skip_set[1] = direction_skip_set[2] = direction_skip_set[3] = 1;
			}

			if (pos & bottom_edge) {
				edges &= ~(bottom_edge & ~0x0000000000000081ULL);
				direction_skip_set[5] = direction_skip_set[6] = direction_skip_set[7] = 1;
			}

			if (pos & right_edge) {
				edges &= ~right_edge;
				direction_skip_set[3] = direction_skip_set[4] = direction_skip_set[5] = 1;
			}

			if (pos & left_edge) {
				edges &= ~left_edge;
				direction_skip_set[0] = direction_skip_set[1] = direction_skip_set[7] = 1;
			}

			for (int j{0}; j < 8; j++) {
				sb temp_pos{pos};

				if (direction_skip_set[j] == 1) { continue; }

				while (true) {
					if (!temp_pos || temp_pos & edges) { break; }

					// shift temp_board over
					if (directions[j] > 0) { temp_pos = temp_pos << directions[j]; } else {
						temp_pos = temp_pos >> -directions[j];
					}

					default_table[i][j] |= temp_pos;
				}
			}
		}

		// set all the slider lookup tables
		lookup_table.queen_table = default_table;
		for (int i{0}; i < 64; i++) {
			int rook_arm_count{0};
			int bishop_arm_count{0};
			for (int j{0}; j < 8; j++) {
				if (j % 2 == 0) {
					lookup_table.rook_table[i][rook_arm_count] = default_table[i][j];
					rook_arm_count++;
				}
				if (j % 2 == 1) {
					lookup_table.bishop_table[i][bishop_arm_count] = default_table[i][j];
					bishop_arm_count++;
				}
			}
		}

		// knight lookup table
		for (int i{0}; i < 64; i++) {
			const sb pos{sb{1} << i};

			// edge masks
			constexpr sb top_edge{0xFFFF000000000000ULL};
			constexpr sb bottom_edge{0x000000000000FFFFULL};
			constexpr sb right_edge{0xC0C0C0C0C0C0C0C0ULL};
			constexpr sb left_edge{0x0303030303030303ULL};

			for (int j{0}; j < 8; j++) {
				// directions of movement
				constexpr std::array directions{10, 17, 15, 6, -10, -17, -15, -6};

				// if on the top, skip any index going out of bounds
				if ((pos & top_edge) && (i + directions[j] > 63)) { continue; }
				// if on the bottom, skip any index going out of bounds
				if ((pos & bottom_edge) && (i + directions[j] < 0)) { continue; }

				// add the position to the table
				if (directions[j] > 0) { lookup_table.knight_table[i][0] |= pos << directions[j]; } else {
					lookup_table.knight_table[i][0] |= pos >> -directions[j];
				}
			}

			// mask out opposite edge
			if (pos & right_edge) { lookup_table.knight_table[i][0] &= ~left_edge; }

			if (pos & left_edge) { lookup_table.knight_table[i][0] &= ~right_edge; }
		}

		// king lookup table
		for (int i{0}; i < 64; i++) {
			const sb pos{sb{1} << i};

			for (int j{0}; j < 8; j++) {
				// directions of movement
				constexpr std::array directions{1, 7, 8, 9, -1, -9, -8, -7};

				// edge masks
				constexpr sb top_edge{0xFF00000000000000ULL};
				constexpr sb bottom_edge{0x00000000000000FFULL};
				constexpr sb left_edge{0x8080808080808080ULL};
				constexpr sb right_edge{0x0101010101010101ULL};

				// if on the left, skip index 0 and 3 and 7
				if ((pos & left_edge) && (j == 0 || j == 3 || j == 7)) { continue; }
				// if on the right, skip index 1 and 4 and 5
				if ((pos & right_edge) && (j == 1 || j == 4 || j == 5)) { continue; }
				// if on the top, skip index 1 and 2 and 3
				if ((pos & top_edge) && (j == 1 || j == 2 || j == 3)) { continue; }
				// if on the bottom, skip index 5 and 6 and 7
				if ((pos & bottom_edge) && (j == 5 || j == 6 || j == 7)) { continue; }

				if (directions[j] > 0) { lookup_table.king_table[i][0] |= pos << directions[j]; } else {
					lookup_table.king_table[i][0] |= pos >> -directions[j];
				}
			}
		}

		// between table
		for (int i{0}; i < 64; i++) {
			const sb pos1 = sb{1} << i;
			for (int j{0}; j < 64; j++) {
				if (i == j) continue;
				const sb pos2 = sb{1} << j;
				for (int k{0}; k < 8; k++) {
					// if this arm contains pos2
					if (default_table[i][k] & pos2) {
						sb between_result = 0;
						// check all the arms from pos2
						for (int m{0}; m < 8; m++) {
							// if arm contains pos1
							if (default_table[j][m] & pos1) {
								between_result = default_table[i][k] & default_table[j][m];
								break;
							}
						}
						between_table[i][j] = between_result | pos1 | pos2;
						break;
					}
				}
			}
		}
	}
};
