#include "game_data.h"

#include <algorithm>
#include <iostream>

/**
 * Moves a piece from prev_pos to new_pos and updates its values accordingly.
 *
 * @param prev_pos the previous position of the piece
 * @param new_pos the new position of the piece
 * @param lt the full lookup table object
 * @param bt the full between table object
 */
bool game_data::move(const sb prev_pos, const sb new_pos, const lookup_tables &lt, const between_tables &bt) {
	const piece_color piece_color = get_color(prev_pos);

	auto [friendly_board, enemy_board] = get_boards(piece_color);

	std::array<piece_data, 16> &friendly_pieces = piece_color == piece_color::WHITE ? white_pieces : black_pieces;
	const std::array<piece_data, 16> &enemy_pieces = piece_color == piece_color::WHITE ? black_pieces : white_pieces;

	sb attack_board = 0;
	int attack_count = 0;
	piece_data attacker;

	// check for pieces attacking the king
	for (const auto &enemy_piece: enemy_pieces) {
		if (enemy_piece.attacks & friendly_pieces[15].position) {
			attacker = enemy_piece;
			attack_count++;
		}
		attack_board |= enemy_piece.attacks;
	}

	const int piece_index = piece_lookup[sb_to_int(prev_pos)];

	// return error if invalid piece
	if (piece_index == 255) {
		std::cerr << "Invalid piece sent to move" << std::endl;
		return false;
	}

	// get the piece and board index
	piece_data &piece{friendly_pieces[piece_index]};

	// checks for pin logic
	if (piece.pinner_id != 255) {
		const int pinner_index = sb_to_int(enemy_pieces[piece.pinner_id].position);
		const int king_index = sb_to_int(friendly_pieces[15].position);

		// need to make only moves on the line between the pinning piece and the king
		const sb ray = bt[pinner_index][king_index];
		if (!(ray & new_pos)) { return false; }
	}

	// lambda for checking pawn moves
	const auto is_valid_pawn_move = [&]() -> bool {
		sb temp_board = piece_color == piece_color::WHITE ? prev_pos << 8 : prev_pos >> 8;

		// check take left (towards the A file with H file mask)
		if ((*enemy_board & ((temp_board << 1) & ~0x0101010101010101ULL)) & new_pos) { return true; }

		// check take right (towards the H file with A file mask)
		if ((*enemy_board & ((temp_board >> 1) & ~0x8080808080808080ULL)) & new_pos) { return true; }

		// check move forwards
		if (!(temp_board & (*friendly_board | *enemy_board))) {
			if (temp_board & new_pos) { return true; }

			// check double move forwards
			temp_board = piece_color == piece_color::WHITE ? temp_board << 8 : temp_board >> 8;

			const bool on_starting_rank = piece_color == piece_color::WHITE
				                              ? prev_pos & 0x000000000000FF00ULL
				                              : prev_pos & 0x00FF000000000000ULL;

			const bool is_path_clear = !(temp_board & (*friendly_board | *enemy_board));

			if (on_starting_rank && is_path_clear && (temp_board & new_pos)) { return true; }
		}
		return false;
	};

	// check if the move is valid for pawn
	if (piece.type == piece_type::PAWN) { if (!is_valid_pawn_move()) { return false; } }
	// check if the move is invalid for non-pawn
	else if ((new_pos & (piece.attacks & ~*friendly_board)) == 0) { return false; }

	// non-king check logic
	if (attack_count && piece.type != piece_type::KING) {
		// king must move if under double check
		if (attack_count > 1) { return false; }

		std::array<piece_data *, 8> observers;
		const int num_observers = get_observers(friendly_pieces[15], *friendly_board, *enemy_board, lt.queen_table,
		                                        observers);

		// if zero observers, then the piece is not a slider... must move the king or take
		if (num_observers == 0) { if (!(new_pos & attacker.position)) { return false; } }

		// see if move blocks check
		const int king_index = sb_to_int(friendly_pieces[15].position);
		for (int i = 0; i < num_observers; i++) {
			const auto *observer = observers[i];
			if (observer->color == piece.color) { continue; }

			const int observer_index = sb_to_int(observer->position);
			const sb ray = bt[king_index][observer_index];

			// if the piece doesn't block or doesn't capture
			if (!(ray & new_pos) && !(new_pos & observer->position)) { return false; }
		}
	}

	// extra king logic
	if (piece.type == piece_type::KING) {
		// king cannot move into danger
		if (new_pos & attack_board) { return false; }

		// king check logic
		if (attack_count) {
			std::array<piece_data *, 8> observers;
			const int num_observers = get_observers(friendly_pieces[15], *friendly_board, *enemy_board, lt.queen_table,
			                                        observers);

			const sb kingless_friendly_board = *friendly_board & ~friendly_pieces[15].position;

			// need to check if the pieces attack the new square the king is moving to (X-ray attacks)
			for (int i = 0; i < num_observers; i++) {
				auto observer = *observers[i];
				if (observer.color == piece.color) { continue; }

				// get the new attacks of the observers
				auto [observer_friendly_board, observer_enemy_board] = get_boards(observers[i]->color);
				update_attacks(observer, kingless_friendly_board, *observer_enemy_board, lt);

				// add the new attack positions to the attack board
				attack_board |= observer.attacks;
			}

			// king cannot move into danger
			if (new_pos & attack_board) { return false; }
		}
	}

	// reset old piece lookup
	piece_lookup[sb_to_int(prev_pos)] = 255;

	// capture if an enemy piece is at new_pos
	if (piece_lookup[sb_to_int(new_pos)] != 255) { capture(piece, new_pos); }

	// update new piece lookup
	piece_lookup[sb_to_int(new_pos)] = piece_index;

	// update friendly color board
	*friendly_board &= ~prev_pos;
	*friendly_board |= new_pos;

	// update old observers
	std::array<piece_data *, 8> observers;
	int num_observers = get_observers(piece, *friendly_board, *enemy_board, lt.queen_table, observers);

	for (int i = 0; i < num_observers; i++) {
		auto [observer_friendly_board, observer_enemy_board] = get_boards(observers[i]->color);
		update_attacks(*observers[i], *observer_friendly_board, *observer_enemy_board, lt);
	}

	// todo need to remove old pins

	// update position of the piece
	piece.position = new_pos;
	// update attacks of the moved piece
	update_attacks(piece, *friendly_board, *enemy_board, lt);

	// update new observers
	observers = std::array<piece_data *, 8>{};
	num_observers = get_observers(piece, *friendly_board, *enemy_board, lt.queen_table, observers);

	for (int i = 0; i < num_observers; i++) {
		auto [observer_friendly_board, observer_enemy_board] = get_boards(observers[i]->color);
		update_attacks(*observers[i], *observer_friendly_board, *observer_enemy_board, lt);
	}

	// todo need to recheck for new pins

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
                               const lookup_tables &lt) {
	piece.attacks = 0;
	switch (piece.type) {
		case piece_type::PAWN: {
			// move forward one
			const sb temp_board = piece.color == piece_color::WHITE ? piece.position << 8 : piece.position >> 8;

			// add take left to attacks (towards the A file with H file mask)
			piece.attacks |= ((temp_board << 1) & ~0x0101010101010101);

			// add take right to attacks (towards the H file with A file mask)
			piece.attacks |= ((temp_board >> 1) & ~0x8080808080808080);

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
template<size_t N>
void game_data::update_sliders(piece_data &piece, const sb friendly_board, const sb enemy_board, const lb<N> &table) {
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

		// if the arm has a value greater than the position, then we need the lsb else we need the msb
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


		// remove the first hit from the hits
		hits &= ~(0x1ULL << first_hit_index);

		// checks if there is no second hit to check
		if (hits == 0) { continue; }

		// only need to calculate the second hit for pins... pins can only be on opposite color piece and king
		if ((0x1ULL << first_hit_index) & friendly_board) { continue; }

		int second_hit_index = 0;
		std::array<piece_data, 16> &enemy_pieces = piece.color == piece_color::WHITE ? black_pieces : white_pieces;

		// if the arm has a value greater than the position, then we need the lsb else we need the msb
		if (arm > piece.position) {
			// gets the trailing zeros of the board, giving the index of the first hit
			second_hit_index = __builtin_ctzll(hits);

			// Builds the mask for the first hit by shifting to the position of the first hit + 1
			// (so the first hit is included), then subtracts one so the board fills with ones from the hit to the
			// final bit. Because we want to save everything less significant than the hit, we are done.
			mask = (0x1ULL << (first_hit_index + 1)) - 1;
		} else {
			// Gets leading zeros of the board. To get the index, takes this value from 63 (max index of the board).
			second_hit_index = 63 - __builtin_clzll(hits);

			// Here the mask will get all ones less significant than the hit, but we want the ones more significant.
			// We do not add one this time but subtract one (to fill the board with ones from the hit to the final bit)
			// and invert it. This will still keep the hit but will set all bits more significant than it to one.
			mask = ~((0x1ULL << first_hit_index) - 1);
		}

		// if the second hit is the king, then the first piece is pinned
		if (enemy_pieces[15].position & (0x1ULL << second_hit_index)) {
			enemy_pieces[piece_lookup[first_hit_index]].pinner_id = piece.id;
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

		if (piece_lookup[hit_index] == 255) {
			std::cerr << "Invalid piece lookup sent to get_observers" << std::endl;
			return 0;
		}

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
