#include "game_data.h"

#include <algorithm>
#include <span>

void game_data::move(const int old_pos, const int new_pos, const game_data gd) {}

sb game_data::get_valid_moves(const int pos, const lookup_tables &lookup_table, const between_tables &between_table) {
	sb output{0};

	// get the piece
	piece_color piece_color{get_color(0x1ULL << pos)};
	piece_data piece{piece_color ? white_pieces[piece_lookup[pos]] : black_pieces[piece_lookup[pos]]};

	switch (piece.type) {
		case piece_type::PAWN: {
			output = pawn_logic(piece);
			break;
		}
		case piece_type::KING: {
			output = king_logic(piece, pos, lookup_table);
			break;
		}
		case piece_type::KNIGHT: {
			auto [friendly_board, enemy_board]{get_boards(piece_color)};
			// set the output of the knight to the lookup table minus friendly pieces
			output = lookup_table.knight_table[pos][0] & ~*friendly_board;
			break;
		}
		case piece_type::BISHOP:
		case piece_type::ROOK:
		case piece_type::QUEEN: {
			output = slider_logic(piece, lookup_table, between_table);
			break;
		}
		default: break;
	}

	// if the piece is pinned
	if (piece.pinner_id != 255) {
		auto [friendly_pieces, enemy_pieces]{get_pieces(piece.color)};
		// get the pinner
		const piece_data &pinner{(*friendly_pieces)[piece.pinner_id]};

		// because we can guarantee being pinned, valid moves must only be on the line between pinner and king
		output &= between_table[sb_to_int(pinner.position)][sb_to_int((*friendly_pieces)[15].position)];
	}

	return output;
}

sb game_data::pawn_logic(const piece_data &piece) {
	sb output{0};

	const piece_color piece_color = piece.color;
	auto [friendly_board, enemy_board]{get_boards(piece_color)};

	sb temp_board{piece_color == piece_color::WHITE ? piece.position << 8 : piece.position >> 8};

	// check take left (towards the A file with H file mask)
	if ((*enemy_board & ((temp_board << 1) & ~0x0101010101010101ULL))) { output |= temp_board << 1; }

	// check take right (towards the H file with A file mask)
	if ((*enemy_board & ((temp_board >> 1) & ~0x8080808080808080ULL))) { output |= temp_board >> 1; }

	// check move forwards
	if (!(temp_board & (*friendly_board | *enemy_board))) {
		output |= temp_board;

		// check double move forwards
		temp_board = piece_color == piece_color::WHITE ? temp_board << 8 : temp_board >> 8;

		const bool on_starting_rank = piece_color == piece_color::WHITE
			                              ? piece.position & 0x000000000000FF00ULL
			                              : piece.position & 0x00FF000000000000ULL;

		const bool is_path_clear{!(temp_board & (*friendly_board | *enemy_board))};

		if (on_starting_rank && is_path_clear) { output |= temp_board; }
	}

	return output;
}

sb game_data::king_logic(const piece_data &piece, const int pos, const lookup_tables &lookup_table) {
	// use the lookup table for init
	sb output{lookup_table.king_table[pos][0]};

	auto [friendly_board, enemy_board]{get_boards(piece.color)};
	auto [friendly_pieces, enemy_pieces]{get_pieces(piece.color)};

	sb attack_board{0};

	// get attack board
	for (const auto &enemy_piece: *enemy_pieces) {
		// if the enemy piece is attacking the king
		if (enemy_piece.attacks & piece.position) {
			// if the piece is a slider
			if (piece.is_slider) {
				// find the arm containing the king
				for (const auto &arm: lookup_table.queen_table[sb_to_int(enemy_piece.position)]) {
					// if the arm contains the king (only one arm can contain the king)
					if (arm & piece.position) {
						output &= ~arm;
						break;
					}
				}
			}
		}
		// add this piece's attacks to the attack board
		attack_board |= enemy_piece.attacks;
	}

	// remove attacked squares from possible moves
	output &= attack_board;

	// remove own pieces
	output &= ~*friendly_board;

	// reminder! pinned pieces are included in attacked pieces, so no need to recheck for them

	return output;
}

sb game_data::slider_logic(const piece_data &piece, const lookup_tables &lookup_table,
                           const between_tables &between_table) {
	sb output{0};
	std::span<const sb> lt;

	// assign the correct lookup table for the piece trying to move
	switch (piece.type) {
		case piece_type::BISHOP: {
			lt = lookup_table.bishop_table[sb_to_int(piece.position)];
			break;
		}
		case piece_type::ROOK: {
			lt = lookup_table.rook_table[sb_to_int(piece.position)];
			break;
		}
		case piece_type::QUEEN: {
			lt = lookup_table.queen_table[sb_to_int(piece.position)];
			break;
		}
		default: break;
	}

	// iterate over each arm in the lookup table
	for (const auto &arm: lt) {
		// ray cast to find first hit
		const piece_data *hit_ptr = ray_cast_x1(arm, piece);
		// if no hit, then full arm is valid for movement
		if (!hit_ptr) {
			output |= arm;
			continue;
		}
		const piece_data hit = *hit_ptr;
		// create the possible move set as being the moves between the two positions
		output = between_table[sb_to_int(piece.position)][sb_to_int(hit.position)] & ~(
			         piece.position | (piece.color == hit.color ? hit.position : 0x0ULL));
	}

	return output;
}


piece_data *game_data::ray_cast_x1(const sb arm, const piece_data &piece) {
	auto [friendly_board, enemy_board]{get_boards(piece.color)};
	auto [friendly_pieces, enemy_pieces]{get_pieces(piece.color)};

	// calculate the hits on the arm
	const sb hits{(*friendly_board | *enemy_board) & arm};

	// return null if no hits
	if (hits == 0) { return nullptr; }

	int hit_index{0};

	// get msb or lsb and build masks
	if (arm > piece.position) {
		// gets the trailing zeros of the board, giving the index of the first hit
		hit_index = __builtin_ctzll(hits);
	} else {
		// gets leading zeros of the board; to get the index, takes this value from 63 (max index of the board)
		hit_index = 63 - __builtin_clzll(hits);
	}

	const sb hit_board{0x1ULL << hit_index};

	// get the pointer of the piece that got hit
	piece_data *hit_ptr{
		(hit_board & *friendly_board)
			? &((*friendly_pieces)[piece_lookup[hit_index]])
			: &((*enemy_pieces)[piece_lookup[hit_index]])
	};

	// return the pointer of the piece
	return hit_ptr;
}

std::pair<piece_data *, piece_data *> game_data::ray_cast_x2(sb arm, const piece_data &piece) { return {}; }
