#include "../include/chess.h"

#include <iostream>
#include <random>

chess::chess(const std::string &fen): gd(fen, tables.lookup_table, tables.between_table) {
	// randomly assign colors
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution dist(0, 1);
	int color = dist(rng);

	p1_color = static_cast<piece_color>(color);
	p2_color = static_cast<piece_color>(1 - color);
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
			sb valid_moves = pseudo_gd.get_valid_moves(piece_index, tables.lookup_table, tables.between_table);

			// go through all valid moves for the piece
			while (valid_moves) {
				const int move_index = __builtin_ctzll(valid_moves);
				game_data new_pseudo_gd = pseudo_gd;

				new_pseudo_gd.move(piece_index, move_index, tables.lookup_table, tables.between_table);

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
			sb valid_moves = pseudo_gd.get_valid_moves(piece_index, tables.lookup_table, tables.between_table);

			// go through all valid moves for the piece
			while (valid_moves) {
				const int move_index = __builtin_ctzll(valid_moves);
				game_data new_pseudo_gd = pseudo_gd;

				new_pseudo_gd.move(piece_index, move_index, tables.lookup_table, tables.between_table);

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

bool chess::check_move(const int old_idx, const int new_idx) {
	piece_color piece_color = gd.get_color(sb{1} << old_idx);
	const piece_data piece = piece_color == piece_color::WHITE
		                         ? gd.white_pieces[gd.piece_lookup[old_idx]]
		                         : gd.black_pieces[gd.piece_lookup[old_idx]];
	auto [friendly_pieces, enemy_pieces] = gd.get_pieces(piece_color);

	// get valid moves and check them against the new position
	const sb valid_moves = gd.get_valid_moves(old_idx, tables.lookup_table, tables.between_table);
	const bool is_move_valid = valid_moves & (sb{1} << new_idx);

	// is the move is not valid, then can never be true
	if (!is_move_valid) { return false; }

	// check if the king is under attack
	if (gd.side_attacks[static_cast<int>(piece_color)] & (*friendly_pieces)[15].position) {
		// check if the king is in double check
		int check_count{0};
		const piece_data *attacker = nullptr;
		for (auto &enemy_piece: *enemy_pieces) {
			if (enemy_piece.attacks & (*friendly_pieces)[15].position) {
				attacker = &enemy_piece;
				check_count++;
				if (check_count > 1) { break; }
			}
		}

		// king can always move to a valid space no matter how many checks (also non-check moves)
		if (piece.type == piece_type::KING) { return true; }

		// if there are more than two attackers, would've had to be the king
		if (check_count > 1) { return false; }

		// if there is one check and king isn't moving, then must block or take
		if (check_count == 1) {
			const sb new_pos = sb{1} << new_idx;

			// checks for null attacker (edge case)
			if (!attacker) { return false; }

			// any move taking the attacker is valid
			if (new_pos == attacker->position) { return true; }

			// if the attacker isn't a slider, can only take it
			if (!attacker->is_slider) { return false; }

			// the move must block the attacker
			if (!(new_pos & tables.between_table[game_data::sb_to_int((*friendly_pieces)[15].position)][
				      game_data::sb_to_int(attacker->position)])) { return false; }
		}
	}

	return true;
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
			sb valid_moves = pseudo_gd.get_valid_moves(piece_index, tables.lookup_table, tables.between_table);

			// go through all valid moves for the piece
			while (valid_moves) {
				const int move_index = __builtin_ctzll(valid_moves);
				game_data new_pseudo_gd = pseudo_gd;

				new_pseudo_gd.move(piece_index, move_index, tables.lookup_table, tables.between_table);

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
			sb valid_moves = pseudo_gd.get_valid_moves(piece_index, tables.lookup_table, tables.between_table);

			// go through all valid moves for the piece
			while (valid_moves) {
				const int move_index = __builtin_ctzll(valid_moves);
				game_data new_pseudo_gd = pseudo_gd;

				new_pseudo_gd.move(piece_index, move_index, tables.lookup_table, tables.between_table);

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
