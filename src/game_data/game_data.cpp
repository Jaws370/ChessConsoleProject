#include "game_data.h"

#include <algorithm>
#include <iostream>

/**
 * Checks a piece at current_pos to see if future_pos is a valid move.
 *
 * @param current_pos the current position
 * @param future_pos the future position to check
 * @return a boolean which is true if the move is valid and false otherwise
 */
bool game_data::is_move_valid(const sb current_pos, const sb future_pos) const {
	const color piece_color = get_color(current_pos);

	const sb &friendly_board = piece_color == color::WHITE ? white_board : black_board;
	const std::array<piece_data, 16> &friendly_pieces = piece_color == color::WHITE ? white_pieces : black_pieces;

	const int piece_index = piece_lookup[sb_to_int(current_pos)];

	if (piece_index == 255) {
		std::cerr << "Invalid piece sent to isMoveValid" << std::endl;
		return false;
	}

	const piece_data piece = friendly_pieces[piece_index];

	// check for the pawn
	if (piece.type == piece_type::PAWN) {
		sb temp_board = piece_color == color::WHITE ? current_pos << 8 : current_pos >> 8;
		const sb enemy_board = piece_color == color::BLACK ? white_board : black_board;

		// check take left
		if ((enemy_board & (temp_board << 1)) & future_pos) { return true; }

		// check take right
		if ((enemy_board & (temp_board >> 1)) & future_pos) { return true; }

		// check move forwards
		if (!(temp_board & (friendly_board | enemy_board))) {
			if (temp_board & future_pos) { return true; }

			// check double move forwards
			temp_board = piece_color == color::WHITE ? temp_board << 8 : temp_board >> 8;

			const bool on_starting_rank = piece_color == color::WHITE
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

/**
 * Moves a piece from prev_pos to new_pos and updates its values accordingly.
 * NOTE: FUNCTION ASSUMES MOVE HAS BEEN ALREADY VALIDATED
 *
 * @param prev_pos the previous position of the piece
 * @param new_pos the new position of the piece
 * @param lt the full lookup table object
 */
void game_data::move(const sb prev_pos, const sb new_pos, const lookup_tables &lt) {
	const color piece_color = get_color(prev_pos);

	std::array<piece_data, 16> &friendly_pieces = piece_color == color::WHITE ? white_pieces : black_pieces;
	std::array<observer_data, 64> &friendly_observers = piece_color == color::WHITE
		                                                    ? white_observers
		                                                    : black_observers;

	const int piece_index = piece_lookup[sb_to_int(prev_pos)];

	// return error if invalid piece
	if (piece_index == 255) {
		std::cerr << "Invalid piece sent to move" << std::endl;
		return;
	}

	// get the piece and board index
	piece_data &piece{friendly_pieces[piece_index]};

	// remove piece's observers
	while (piece.observing) {
		const int observer_index = __builtin_ctzll(piece.observing);
		friendly_observers[observer_index].remove(piece.id);
		piece.observing &= ~(0x1ULL << observer_index);
	}

	// update position of the piece
	piece.position = new_pos;

	// update piece lookup
	piece_lookup[sb_to_int(prev_pos)] = 255;
	piece_lookup[sb_to_int(new_pos)] = piece_index;

	// update color board
	if (piece_color == color::WHITE) {
		white_board &= ~prev_pos;
		white_board |= new_pos;
	} else {
		black_board &= ~prev_pos;
		black_board |= new_pos;
	}

	// reset piece's attacks
	piece.attacks = 0;

	// update attacks and observers of the moved piece
	update_attacks(piece, lt);

	// update sliders observing prev_pos
	const int prev_index = sb_to_int(prev_pos);
	update_observers(prev_index, lt);

	// update sliders observing new_pos
	const int new_index = sb_to_int(new_pos);
	update_observers(new_index, lt);
}

/**
 * This function checks the type of the piece being passed, then updates the attacks and observers corresponding to
 * the piece type.
 *
 * @param piece the piece_data of the piece being updated
 * @param lt the full list of lookup tables
 */
void game_data::update_attacks(piece_data &piece, const lookup_tables &lt) {
	switch (piece.type) {
		case piece_type::PAWN: {
			// move forward one
			const sb temp_board = piece.color == color::WHITE ? piece.position << 8 : piece.position >> 8;

			// add take left to attacks
			piece.attacks |= temp_board << 1;

			// add take right to attacks
			piece.attacks |= temp_board >> 1;

			return;
		} // pawn
		case piece_type::BISHOP: {
			update_sliders(piece, lt.bishop_table);
			return;
		} // bishop
		case piece_type::KNIGHT: {
			piece.attacks |= lt.knight_table[sb_to_int(piece.position)][0];
			return;
		} // knight
		case piece_type::ROOK: {
			update_sliders(piece, lt.rook_table);
			return;
		} // rook
		case piece_type::QUEEN: {
			update_sliders(piece, lt.queen_table);
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
 * This function loops through all observers at a given board index and updates their own attacks and observers.
 *
 * @param observer_index the sb_to_int of the index whose observers need to be updated
 * @param lt the full lookup tables list
 */
void game_data::update_observers(const int observer_index, const lookup_tables &lt) {
	const auto &[white_ids, white_length] = white_observers[observer_index];
	for (int i = 0; i < white_length; i++) {
		piece_data &piece = white_pieces[white_ids[i]];
		update_attacks(piece, lt);
	}

	const auto &[black_ids, black_length] = black_observers[observer_index];
	for (int i = 0; i < black_length; i++) {
		piece_data &piece = black_pieces[black_ids[i]];
		update_attacks(piece, lt);
	}
}

/**
 * This function checks each arm of the lookup table for pieces, then masks over those pieces to get the attacks and
 * observers of the piece.
 *
 * @tparam N number of arms given by the lookup_table element
 * @param piece the piece_data of the piece being updated
 * @param table the lookup table for the piece
 */
template<size_t N>
void game_data::update_sliders(piece_data &piece, const lb<N> &table) {
	const sb friendly_board = piece.color == color::WHITE ? white_board : black_board;
	const sb enemy_board = piece.color == color::BLACK ? white_board : black_board;

	sb observing_board = 0;
	piece.attacks = 0;

	// go through each arm
	for (const auto &arm: table[sb_to_int(piece.position)]) {
		// calculate the hits on the arm
		sb hits = (friendly_board | enemy_board) & arm;

		// return full arm if no hits
		if (hits == 0) {
			piece.attacks |= arm;
			observing_board |= arm;
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
		if (hits == 0) {
			observing_board |= arm;
			continue;
		}

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

		// update observers
		observing_board |= arm & mask;
	}

	std::array<observer_data, 64> &friendly_observers = piece.color == color::WHITE
		                                                    ? white_observers
		                                                    : black_observers;

	// update the piece's observing data
	piece.observing = observing_board;

	// convert the observer board to the observer array
	while (observing_board) {
		const int observer_index = __builtin_ctzll(observing_board);

		friendly_observers[observer_index].add(piece.id);

		observing_board &= ~(0x1ULL << observer_index);
	}
}
