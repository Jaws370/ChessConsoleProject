#include "../include/chess.h"

#include <bitset>
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

int chess::negamax(game_data pseudo_gd, const piece_color color, const int depth, int alpha, const int beta) {
	if (depth == 0) { return (color == piece_color::WHITE ? 1 : -1) * pseudo_gd.evaluate_position(); }

	int max = INT_MIN;
	sb own_board = color == piece_color::WHITE ? pseudo_gd.white_board : pseudo_gd.black_board;

	const auto opponent_color = color == piece_color::WHITE ? piece_color::BLACK : piece_color::WHITE;

	while (own_board) {
		const int piece_index = __builtin_ctzll(own_board);
		sb valid_moves = pseudo_gd.get_valid_moves(piece_index, tables.lookup_table, tables.between_table);

		while (valid_moves) {
			const int move_index = __builtin_ctzll(valid_moves);
			// make sure the move is valid
			if (check_move(piece_index, move_index, pseudo_gd)) {
				game_data new_pseudo_gd = pseudo_gd;
				new_pseudo_gd.move(piece_index, move_index, tables.lookup_table, tables.between_table);

				// check if the new move is good
				const int result = -negamax(new_pseudo_gd, opponent_color, depth - 1, -beta, -alpha);
				max = std::max(max, result);
				alpha = std::max(alpha, max);

				if (alpha >= beta) { return max; }
			}

			valid_moves &= ~(sb{1} << move_index);
		}

		own_board &= ~(sb{1} << piece_index);
	}

	return max;
}

bool chess::check_move(const int old_idx, const int new_idx, game_data &search_gd) const {
	piece_color piece_color = search_gd.get_color(sb{1} << old_idx);
	const piece_data piece = piece_color == piece_color::WHITE
		                         ? search_gd.white_pieces[gd.piece_lookup[old_idx]]
		                         : search_gd.black_pieces[gd.piece_lookup[old_idx]];
	auto [friendly_pieces, enemy_pieces] = search_gd.get_pieces(piece_color);

	// get valid moves and check them against the new position
	const sb valid_moves = search_gd.get_valid_moves(old_idx, tables.lookup_table, tables.between_table);
	const bool is_move_valid = valid_moves & (sb{1} << new_idx);

	// is the move is not valid, then can never be true
	if (!is_move_valid) { return false; }

	// king can always move to a valid space no matter how many checks (also non-check moves)
	if (piece.type == piece_type::KING) { return true; }

	// check if the king is under attack
	if (search_gd.side_attacks[1 - static_cast<int>(piece_color)] & (*friendly_pieces)[15].position) {
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
	game_data pseudo_gd = gd;
	std::pair best_move{-1, -1};

	int best_score = INT_MIN;
	sb own_board = p2_color == piece_color::WHITE ? pseudo_gd.white_board : pseudo_gd.black_board;

	const auto opponent_color = p2_color == piece_color::WHITE ? piece_color::BLACK : piece_color::WHITE;

	while (own_board) {
		const int piece_index = __builtin_ctzll(own_board);
		sb valid_moves = pseudo_gd.get_valid_moves(piece_index, tables.lookup_table, tables.between_table);

		while (valid_moves) {
			const int move_index = __builtin_ctzll(valid_moves);
			// make sure the move is valid
			if (check_move(piece_index, move_index, pseudo_gd)) {
				game_data new_pseudo_gd = pseudo_gd;
				new_pseudo_gd.move(piece_index, move_index, tables.lookup_table, tables.between_table);

				// check if the new move is good
				const int result = -negamax(new_pseudo_gd, opponent_color, depth - 1, INT_MIN + 1, INT_MAX);

				if (result > best_score) {
					best_move = std::make_pair(piece_index, move_index);
					best_score = result;
				}
			}

			valid_moves &= ~(sb{1} << move_index);
		}

		own_board &= ~(sb{1} << piece_index);
	}

	// make the best move
	if (best_move.first == -1 || best_move.second == -1) {
		std::cerr << "AI ERROR: NO BEST MOVE FOUND" << std::endl;
		return;
	}

	move(best_move.first, best_move.second);
}
