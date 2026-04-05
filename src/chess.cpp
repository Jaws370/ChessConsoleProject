#include "../include/chess.h"

#include <iostream>
#include <random>

void chess::table_init() {
	std::array<std::array<sb, 8>, 64> default_table{};

	// generate all the arms starting left then going clockwise
	for (int i{0}; i < 64; i++) {
		const sb pos{sb{1} << i};
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
		const sb pos1 = sb{1} << i;
		for (int j{0}; j < 64; j++) {
			if (i == j) { continue; }

			const sb pos2 = sb{1} << j;

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

bool chess::check_move(const int old_pos, const int new_pos) {
	const sb valid_moves = gd.get_valid_moves(old_pos, lookup_table, between_table);
	const sb b_new_pos = sb{1} << new_pos;

	if (valid_moves & b_new_pos) { return true; }

	return false;
}

int chess::minimax(game_data pseudo_gd, const int depth, const bool is_maximizing) {
	if (depth == 0) {
		// || is_check_mate(); )
		return pseudo_gd.evaluate_position();
	}

	if (is_maximizing) {
		int best_score{INT_MIN};

		while (pseudo_gd.white_board) {
			const int piece_index = __builtin_ctzll(pseudo_gd.white_board);
			sb valid_moves = pseudo_gd.get_valid_moves(piece_index, lookup_table, between_table);

			// go through all valid moves for the piece
			while (valid_moves) {
				const int move_index = __builtin_ctzll(valid_moves);
				game_data new_pseudo_gd = pseudo_gd;

				new_pseudo_gd.move(piece_index, move_index, lookup_table, between_table);

				// check if the new move is good
				const int result = minimax(new_pseudo_gd, depth - 1, false);
				best_score = std::max(best_score, result);

				valid_moves &= ~(sb{1} << move_index);
			}

			pseudo_gd.white_board &= ~(sb{1} << piece_index);
		}

		return best_score;
	} else {
		int best_score{INT_MAX};

		while (pseudo_gd.black_board) {
			const int piece_index = __builtin_ctzll(pseudo_gd.black_board);
			sb valid_moves = pseudo_gd.get_valid_moves(piece_index, lookup_table, between_table);

			// go through all valid moves for the piece
			while (valid_moves) {
				const int move_index = __builtin_ctzll(valid_moves);
				game_data new_pseudo_gd = pseudo_gd;

				new_pseudo_gd.move(piece_index, move_index, lookup_table, between_table);

				// check if the new move is good
				const int result = minimax(new_pseudo_gd, depth - 1, true);
				best_score = std::min(best_score, result);

				valid_moves &= ~(sb{1} << move_index);
			}

			pseudo_gd.black_board &= ~(sb{1} << piece_index);
		}

		return best_score;
	}
}

void chess::ai_move(const int depth) {
	bool is_maximizing{false};

	if (p2_color == piece_color::WHITE) { is_maximizing = true; }

	game_data pseudo_gd = gd;
	std::pair<int, int> best_move{-1, -1};

	if (is_maximizing) {
		// get the best move
		int best_score{INT_MIN};

		while (pseudo_gd.white_board) {
			const int piece_index = __builtin_ctzll(pseudo_gd.white_board);
			sb valid_moves = pseudo_gd.get_valid_moves(piece_index, lookup_table, between_table);

			// go through all valid moves for the piece
			while (valid_moves) {
				const int move_index = __builtin_ctzll(valid_moves);
				game_data new_pseudo_gd = pseudo_gd;

				new_pseudo_gd.move(piece_index, move_index, lookup_table, between_table);

				// check if the new move is good
				const int result = minimax(new_pseudo_gd, depth - 1, false);
				if (result > best_score) {
					best_move = std::make_pair(piece_index, move_index);
					best_score = result;
				}

				valid_moves &= ~(sb{1} << move_index);
			}

			pseudo_gd.white_board &= ~(sb{1} << piece_index);
		}
	} else {
		// get the best move
		int best_score{INT_MAX};

		while (pseudo_gd.black_board) {
			const int piece_index = __builtin_ctzll(pseudo_gd.black_board);
			sb valid_moves = pseudo_gd.get_valid_moves(piece_index, lookup_table, between_table);

			// go through all valid moves for the piece
			while (valid_moves) {
				const int move_index = __builtin_ctzll(valid_moves);
				game_data new_pseudo_gd = pseudo_gd;

				new_pseudo_gd.move(piece_index, move_index, lookup_table, between_table);

				// check if the new move is good
				const int result = minimax(new_pseudo_gd, depth - 1, true);
				if (result < best_score) {
					best_move = std::make_pair(piece_index, move_index);
					best_score = result;
				}

				valid_moves &= ~(sb{1} << move_index);
			}

			pseudo_gd.black_board &= ~(sb{1} << piece_index);
		}
	}

	// make the best move
	if (best_move.first == -1 || best_move.second == -1) {
		std::cerr << "AI ERROR: NO BEST MOVE FOUND" << std::endl;
		return;
	}

	move(best_move.first, best_move.second);
}

chess::chess(const std::string &fen): gd(fen) {
	// init the util tables
	table_init();

	// randomly assign colors
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> dist(0, 1);
	int color = dist(rng);

	p1_color = static_cast<piece_color>(color);
	p2_color = static_cast<piece_color>(1 - color);
}
