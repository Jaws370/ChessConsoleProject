#include "game_data.h"

#include <algorithm>
#include <iostream>

/**
 * Checks a piece at current_pos to see if future_pos is a valid move.
 *
 * @param current_pos the current position
 * @param future_pos the future position to check
 * @param lt the lookup tables for pieces
 * @return a boolean which is true if the move is valid and false otherwise
 */
bool game_data::is_move_valid(const sb current_pos, const sb future_pos, const lookup_tables &lt) const {
	const piece_color piece_color = get_color(current_pos);

	const sb friendly_board = piece_color == piece_color::WHITE ? white_board : black_board;
	const std::array<piece_data, 16> &friendly_pieces = piece_color == piece_color::WHITE ? white_pieces : black_pieces;

	const int piece_index = piece_lookup[sb_to_int(current_pos)];

	if (piece_index == 255) { std::cerr << "Invalid piece sent to isMoveValid" << std::endl; }

	const piece_data piece = friendly_pieces[piece_index];

	// check for the pawn
	if (piece.type == piece_type::PAWN) {
		sb temp_board = piece_color == piece_color::WHITE ? current_pos << 8 : current_pos >> 8;
		const sb enemy_board = piece_color == piece_color::BLACK ? white_board : black_board;

		// check take left
		if ((enemy_board & (temp_board << 1)) & future_pos) { return true; }

		// check take right
		if ((enemy_board & (temp_board >> 1)) & future_pos) { return true; }

		// check move forwards
		if (!(temp_board & (friendly_board | enemy_board))) {
			if (temp_board & future_pos) { return true; }

			// check double move forwards
			temp_board = piece_color == piece_color::WHITE ? temp_board << 8 : temp_board >> 8;

			const bool on_starting_rank = piece_color == piece_color::WHITE
				                              ? current_pos & 0x000000000000FF00ULL
				                              : current_pos & 0x00FF000000000000ULL;

			const bool is_path_clear = !(temp_board & (friendly_board | enemy_board));

			if (on_starting_rank && is_path_clear && temp_board & future_pos) { return true; }
		}

		return false;
	}

	// check for non-pawn
	if (future_pos & (piece.attacks & ~friendly_board)) { return true; }

	return false;
}

/* true is good to go, false is invalid move... eventually needs to be computed in move function
bool game_data::checking_checks(const piece_data &piece, const sb current_pos, const sb future_pos,
                                const lookup_tables &lt) const {
	// need to check if under check
	const std::array<piece_data, 16> friendly_pieces = piece.color == piece_color::WHITE ? white_pieces : black_pieces;

	const std::array<int, 64> enemy_attack_weight = piece.color == piece_color::WHITE
		                                                ? black_attack_weight
		                                                : white_attack_weight;

	const sb king_pos = friendly_pieces[15].position;

	// if no one is attacking the king, then no checks
	if (enemy_attack_weight[king_pos] == 0) { return true; }
	// need to check for double check (king must move)
	if (enemy_attack_weight[king_pos] > 1) { if (piece.type != piece_type::KING) { return false; } }

	const sb enemy_checkers = piece.color == piece_color::WHITE ? black_checkers : white_checkers;

	// need to check if we take the attacker
	if (future_pos & enemy_checkers) { return true; }

	const sb checker_id = piece_lookup[sb_to_int(enemy_checkers)];

	const std::array<piece_data, 16> enemy_pieces = piece.color == piece_color::WHITE ? black_pieces : white_pieces;
	const piece_data checker_piece = enemy_pieces[checker_id];

	// check king moving out of check
	const int future_index = sb_to_int(future_pos);
	if (piece.type == piece_type::KING) {
		// if the move takes us to an attacked square
		if (enemy_attack_weight[future_index] != 0) { return false; }

		// if the piece checking us is observing the space we are moving to
		if (checker_piece.observing & future_pos) {
			piece_data temp_piece = checker_piece;
			const sb friendly_board = piece.color == piece_color::WHITE ? white_board : black_board;
			const sb temp_friendly_board = (friendly_board & ~current_pos) | future_pos;

			const sb enemy_board = piece.color == piece_color::WHITE ? black_board : white_board;

			// update the attacks of our temp checker
			update_attacks(temp_piece, enemy_board, temp_friendly_board, lt);

			// if those new attacks contain the king, invalid move
			if (temp_piece.attacks & king_pos) { return false; }
		}

		// otherwise the move is valid
		return true;
	}

	// deal with pins
	const int current_index = sb_to_int(current_pos);
	if (pins[current_index] != 0) {
		// need to calculate the arm of the piece pinning us and then make sure we are only moving along that arm\
		// todo much later
	}

	// deal with blocking with a piece
	switch (checker_piece.type) {
		case piece_type::BISHOP: {
			const lb<4> &bishop_table = lt.bishop_table;
			int arm_index = 0;
			for (int i = checker_piece.position > king_pos ? 0 : 4; i < 8; i++) {
				if (bishop_table[i][checker_piece.position] & king_pos) {
					arm_index = i;
					break;
				}
			}

			if (future_pos & bishop_table[arm_index][checker_piece.position]) { return true; }
			break;
		}
		case piece_type::ROOK: {
			const lb<4> &rook_table = lt.rook_table;
			int arm_index = 0;
			for (int i = checker_piece.position > king_pos ? 0 : 2; i < 8; i++) {
				if (rook_table[i][checker_piece.position] & king_pos) {
					arm_index = i;
					break;
				}
			}

			if (future_pos & rook_table[arm_index][checker_piece.position]) { return true; }
			break;
		}
		case piece_type::QUEEN: {
			const lb<8> &queen_table = lt.queen_table;
			int arm_index = 0;
			for (int i = checker_piece.position > king_pos ? 0 : 4; i < 8; i++) {
				if (queen_table[i][checker_piece.position] & king_pos) {
					arm_index = i;
					break;
				}
			}

			if (future_pos & queen_table[arm_index][checker_piece.position]) { return true; }
			break;
		}
		default: break;
	}

	return false;
} */

/**
 * Moves a piece from prev_pos to new_pos and updates its values accordingly.
 *
 * @param prev_pos the previous position of the piece
 * @param new_pos the new position of the piece
 * @param lt the full lookup table object
 * @param bt the full between table
 */
bool game_data::move(const sb prev_pos, const sb new_pos, const lookup_tables &lt, const between_tables &bt) {
	const piece_color piece_color = get_color(prev_pos);

	auto [friendly_board, enemy_board] = get_boards(piece_color);

	std::array<piece_data, 16> &friendly_pieces = piece_color == piece_color::WHITE ? white_pieces : black_pieces;
	const std::array<piece_data, 16> &enemy_pieces = piece_color == piece_color::WHITE ? black_pieces : white_pieces;

	int attack_count = 0;
	piece_data attacker;

	for (const auto &enemy_piece: enemy_pieces) {
		if (enemy_piece.attacks & friendly_pieces[15].position) {
			attack_count++;
			attacker = enemy_piece;
		}
	}

	const int piece_index = piece_lookup[sb_to_int(prev_pos)];

	// return error if invalid piece
	if (piece_index == 255) {
		std::cerr << "Invalid piece sent to move" << std::endl;
		return false;
	}

	// get the piece and board index
	piece_data &piece{friendly_pieces[piece_index]};

	// check for a valid move
	if (attack_count) {
		// king must move if under double check
		if (piece.type != piece_type::KING && attack_count > 1) { return false; }

		std::array<piece_data *, 8> observers;
		int num_observers = get_observers(friendly_pieces[15], friendly_board, enemy_board, lt.queen_table, observers);

		// if zero observers, then the piece is not a slider... must move the king or take
		if (num_observers == 0) { if (piece.type != piece_type::KING || new_pos & attacker.position) { return false; } }

		// check if move blocks check
		const int king_index = sb_to_int(friendly_pieces[15].position);
		for (int i = 0; i < num_observers; i++) {
			const auto *observer = observers[i];
			if (observer->color == piece.color) { continue; }

			const int observer_index = sb_to_int(observer->position);
			const sb ray = bt[king_index][observer_index];

			// of the piece doesn't block or doesn't capture
			if (!(ray & new_pos) && !(new_pos & observer->position)) { return false; }

			// if the piece is pinned and trying to move off the pin, invalid move
			if (piece.is_pinned && !(ray & new_pos)) { return false; }
		}
	}

	// reset old piece lookup
	piece_lookup[sb_to_int(prev_pos)] = 255;

	// capture if an enemy piece is at new_pos
	if (piece_lookup[sb_to_int(new_pos)] != 255) { capture(piece, new_pos); }

	// update new piece lookup
	piece_lookup[sb_to_int(new_pos)] = piece_index;

	// update friendly color board
	friendly_board &= ~prev_pos;
	friendly_board |= new_pos;

	// update old observers
	std::array<piece_data *, 8> observers;
	int num_observers = get_observers(piece, friendly_board, enemy_board, lt.queen_table, observers);

	for (int i = 0; i < num_observers; i++) {
		auto [observer_friendly_board, observer_enemy_board] = get_boards(observers[i]->color);
		update_attacks(*observers[i], observer_friendly_board, observer_enemy_board, lt);
	}

	// update position of the piece
	piece.position = new_pos;
	// update attacks of the moved piece
	update_attacks(piece, friendly_board, enemy_board, lt);

	// update new observers
	observers = std::array<piece_data *, 8>{};
	num_observers = get_observers(piece, friendly_board, enemy_board, lt.queen_table, observers);

	for (int i = 0; i < num_observers; i++) {
		auto [observer_friendly_board, observer_enemy_board] = get_boards(observers[i]->color);
		update_attacks(*observers[i], observer_friendly_board, observer_enemy_board, lt);
	}

	return true;
}

/**
 * This function handles capture logic during a move.
 *
 * @param piece the piece_data of the piece doing the capturing
 * @param new_pos the position of the conflict square
 */
void game_data::capture(const piece_data &piece, const sb new_pos) {
	sb &enemy_board = piece.color == piece_color::WHITE ? black_board : white_board;

	std::array<piece_data, 16> &enemy_pieces = piece.color == piece_color::WHITE ? black_pieces : white_pieces;

	const int capture_index = sb_to_int(new_pos);

	enemy_board &= ~new_pos;

	piece_data &captured_piece = enemy_pieces[piece_lookup[capture_index]];
	piece_lookup[capture_index] = 255;

	captured_piece.reset();
}


/**
 * This function checks the type of the piece being passed, then updates the attacks and observers corresponding to
 * the piece type.
 *
 * @param piece the piece_data of the piece being updated
 * @param friendly_board
 * @param enemy_board
 * @param lt the full list of lookup tables
 */
void game_data::update_attacks(piece_data &piece, const sb friendly_board, const sb enemy_board,
                               const lookup_tables &lt) const {
	piece.attacks = 0;
	switch (piece.type) {
		case piece_type::PAWN: {
			// move forward one
			const sb temp_board = piece.color == piece_color::WHITE ? piece.position << 8 : piece.position >> 8;

			// add take left to attacks
			piece.attacks |= temp_board << 1;

			// add take right to attacks
			piece.attacks |= temp_board >> 1;

			return;
		} // pawn
		case piece_type::BISHOP: {
			update_sliders(piece, friendly_board, enemy_board, lt.bishop_table);
			return;
		} // bishop
		case piece_type::KNIGHT: {
			piece.attacks |= lt.knight_table[sb_to_int(piece.position)][0];
			return;
		} // knight
		case piece_type::ROOK: {
			update_sliders(piece, friendly_board, enemy_board, lt.rook_table);
			return;
		} // rook
		case piece_type::QUEEN: {
			update_sliders(piece, friendly_board, enemy_board, lt.queen_table);
			return;
		} // queen
		case piece_type::KING: {
			// todo check stuff to make sure king isn't moving somewhere bad
			piece.attacks |= lt.king_table[sb_to_int(piece.position)][0];
			return;
		} // king

		default: { std::cerr << "Invalid piece sent to makeMove" << std::endl; } // invalid piece
	}
}

/**
 * This function checks each arm of the lookup table for pieces, then masks over those pieces to get the attacks and
 * observers of the piece.
 *
 * @tparam N number of arms given by the lookup_table element
 * @param piece the piece_data of the piece being updated
 * @param friendly_board
 * @param enemy_board
 * @param table the lookup table for the piece
 */
template
<
	size_t N
>
void game_data::update_sliders(piece_data &piece, const sb friendly_board, const sb enemy_board,
                               const lb<N> &table) const {
	piece.attacks = 0;

	// go through each arm
	for (const auto &arm: table[sb_to_int(piece.position)]) {
		// calculate the hits on the arm
		sb hits = (friendly_board | enemy_board) & arm;

		// return full arm if no hits
		if (hits == 0) {
			piece.attacks |= arm;
			continue;
		}

		int first_hit_index = 0;

		sb mask = 0;

		// get msb or lsb and build masks
		if (arm > piece.position) {
			// gets the trailing zeros of the board, giving the index of the first hit
			first_hit_index = __builtin_ctzll(hits);

			// Builds the mask for the first hit by shifting to the position of the first hit + 1
			// (so the first hit is included), then subtracts one so the board fills with ones from the hit to the
			// final bit. Because we want to save everything less significant than the hit, we are done.
			mask = (0x1ULL << (first_hit_index + 1)) - 1;
		} else {
			// Gets leading zeros of the board. To get the index, takes this value from 63 (max index of the board).
			first_hit_index = 63 - __builtin_clzll(hits);

			// Here the mask will get all ones less significant than the hit, but we want the ones more significant.
			// We do not add one this time but subtract one (to fill the board with ones from the hit to the final bit)
			// and invert it. This will still keep the hit but will set all bits more significant than it to one.
			mask = ~((0x1ULL << first_hit_index) - 1);
		}

		// add arm to attacks with the hit
		piece.attacks |= arm & mask;

		// removes the first hit so that repeating the process gives the second hit
		hits = hits & ~(0x1ULL << first_hit_index);

		// if the second check doesn't hit
		if (hits == 0) { continue; }

		// resets variables
		int second_hit_index = 0;
		mask = 0;

		// get msb and lsb for the second hit
		if (arm > piece.position) {
			// same as before with new hits
			second_hit_index = __builtin_ctzll(hits);
			mask = (0x1ULL << (second_hit_index + 1)) - 1;
		} else {
			// same as before with new hits
			second_hit_index = 63 - __builtin_clzll(hits);
			mask = ~((0x1ULL << second_hit_index) - 1);
		}
	}
}

// get affected pieces (pieces observing the position) to update their attacks
int game_data::get_observers(const piece_data &piece, const sb friendly_board,
                             const sb enemy_board, const lb<8> &table, std::array<piece_data *, 8> &observers) {
	int count = 0;

	std::array<piece_data, 16> &friendly_pieces = piece.color == piece_color::WHITE ? white_pieces : black_pieces;
	std::array<piece_data, 16> &enemy_pieces = piece.color == piece_color::WHITE ? black_pieces : white_pieces;

	for (int i = 0; i < 8; i++) {
		const auto &arm = table[sb_to_int(piece.position)][i];
		// calculate the hits on the arm
		const sb hits = (friendly_board | enemy_board) & arm;

		// return full arm if no hits
		if (hits == 0) { continue; }

		int hit_index = 0;

		// get msb or lsb and build masks
		if (arm > piece.position) {
			// gets the trailing zeros of the board, giving the index of the first hit
			hit_index = __builtin_ctzll(hits);
		} else {
			// Gets leading zeros of the board. To get the index, takes this value from 63 (max index of the board).
			hit_index = 63 - __builtin_clzll(hits);
		}

		const sb hit_board = 0x1ULL << hit_index;

		// get the piece that got hit
		piece_data &hit_piece = hit_board & friendly_board
			                        ? friendly_pieces[piece_lookup[hit_index]]
			                        : enemy_pieces[piece_lookup[hit_index]];

		// checks the type of the piece... if it is correct for the arm being checked, add it to observers
		switch (hit_piece.type) {
			case piece_type::BISHOP:
				if ((i % 2)) { observers[count++] = &hit_piece; }
				break;
			case piece_type::ROOK:
				if (!(i % 2)) { observers[count++] = &hit_piece; }
				break;
			case piece_type::QUEEN: {
				observers[count++] = &hit_piece;
				break;
			}
			default: break;
		}
	}

	return count;
}
