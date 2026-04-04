#include "../include/chess.h"

#include <ios>
#include <unordered_map>

void chess::set_game_data(const std::string &fen) {
	std::array<uint8_t, 12> b_boards{};

	// dictionary for piece characters
	std::unordered_map<char, int> piece_lookup{{'P', 0}, {'N', 1}, {'B', 2}, {'R', 3}, {'Q', 4}, {'K', 5}};

	// loop through fen string
	int pos{0};
	for (const char c: fen) {
		// skip over formatting
		if (c == '/') { continue; }

		// go through empty positons
		if (isdigit(c)) {
			pos += c - '0';
			continue;
		}

		// get the correct board index
		int board{0};
		if (!std::isupper(c)) { board = 6; }
		board += piece_lookup[std::toupper(c)];

		// adds the piece to bit board
		b_boards[board] |= 0x1ULL << pos;

		pos++;
	}

	int white_piece_count{0};
	int black_piece_count{0};

	// create the piece arrays
	for (int i{0}; i < 12; i++) {
		// get the index of the piece in the bit board
		const int b_pos{__builtin_ctzll(b_boards[i])};

		// if i >= 6, this is a white board else black
		if (i >= 6) {
			// set color board, color pieces, and piece_lookup
			gd.white_board |= 0x1ULL << b_pos;
			gd.piece_lookup[b_pos] = white_piece_count;

			// king must go last
			if (i == 11) {
				gd.white_pieces[15] = piece_data(0x1ULL << b_pos, static_cast<piece_type>(i - 6),
				                                 piece_color::WHITE, 15);
				continue;
			}

			gd.white_pieces[white_piece_count] = piece_data(0x1ULL << b_pos, static_cast<piece_type>(i - 6),
			                                                piece_color::WHITE, white_piece_count);
			white_piece_count++;
		} else {
			// set color board, color pieces, and piece_lookup
			gd.black_board |= 0x1ULL << b_pos;
			gd.piece_lookup[b_pos] = black_piece_count;

			// king must go last
			if (i == 5) {
				gd.black_pieces[15] = piece_data(0x1ULL << b_pos, static_cast<piece_type>(i),
				                                 piece_color::WHITE, 15);
				continue;
			}

			gd.black_pieces[black_piece_count] = piece_data(0x1ULL << b_pos, static_cast<piece_type>(i),
			                                                piece_color::BLACK, black_piece_count);
			black_piece_count++;
		}
	}
}

void chess::table_init() {
	std::array<std::array<sb, 8>, 64> default_table{};

	// generate all the arms starting left then going clockwise
	for (int i{0}; i < 64; i++) {
		const sb pos{0x1ULL << i};
		constexpr std::array directions{1, 9, 8, 7, -1, -9, -8, -7};

		// edge masks
		constexpr sb top_edge{0xFF00000000000000ULL};
		constexpr sb bottom_edge{0x00000000000000FFULL};
		constexpr sb right_edge{0x8080808080808080ULL};
		constexpr sb left_edge{0x0101010101010101ULL};
		sb edges{top_edge | bottom_edge | right_edge | left_edge};

		std::array<int, 8> direction_skip_set{0};

		if (pos & top_edge) {
			edges &= ~top_edge;
			direction_skip_set[1] = direction_skip_set[2] = direction_skip_set[3] = 1;
		}

		if (pos & bottom_edge) {
			edges &= ~bottom_edge;
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
				if (temp_pos & edges) { break; }

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
		const sb pos{0x1ULL << i};

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
		const sb pos{0x1ULL << i};

		for (int j{0}; j < 8; j++) {
			// directions of movement
			constexpr std::array directions{1, 7, 8, 9, -1, -9, -8, -7};

			// edge masks
			constexpr sb top_edge{0xFF00000000000000ULL};
			constexpr sb bottom_edge{0x00000000000000FFULL};
			constexpr sb right_edge{0x8080808080808080ULL};
			constexpr sb left_edge{0x0101010101010101ULL};

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
		const sb pos1 = 0x1ULL << i;
		for (int j{0}; j < 64; j++) {
			if (i == j) { continue; }

			const sb pos2 = 0x1ULL << j;

			// if already filled, continue
			if (between_table[i][j] != 0) { continue; }

			// can take an arm at the first pos and then the same arm at the second and subtract
			for (int k{0}; k < 8; k++) {
				if (default_table[i][k] & pos2) {
					between_table[i][j] = default_table[i][k] & ~default_table[j][k];
					between_table[i][j] |= pos1 | pos2;
					between_table[j][i] = between_table[i][j];
					break;
				}
			}
		}
	}
}

chess::chess(const std::string &fen) {
	// set to default position
	gd = game_data();

	// set the game data
	set_game_data(fen);

	// init the util tables
	table_init();
}
