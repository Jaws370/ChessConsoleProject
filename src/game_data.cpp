#include "../include/game_data.h"

#include <span>
#include <unordered_map>

piece_color game_data::get_color(const sb pos) const {
	if (pos & white_board) { return piece_color::WHITE; }
	if (pos & black_board) { return piece_color::BLACK; }
	return piece_color::NONE;
}

std::pair<sb *, sb *> game_data::get_boards(const piece_color color) {
	return color == piece_color::WHITE
		       ? std::pair{&white_board, &black_board}
		       : std::pair{&black_board, &white_board};
}

std::pair<std::array<piece_data, 16> *, std::array<piece_data, 16> *> game_data::get_pieces(const piece_color color) {
	return color == piece_color::WHITE
		       ? std::pair{&white_pieces, &black_pieces}
		       : std::pair{&black_pieces, &white_pieces};
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

	const sb hit_board{sb{1} << hit_index};

	// get the pointer of the piece that got hit
	piece_data *hit_ptr{
		(hit_board & *friendly_board)
			? &((*friendly_pieces)[piece_lookup[hit_index]])
			: &((*enemy_pieces)[piece_lookup[hit_index]])
	};

	// return the pointer of the piece
	return hit_ptr;
}

std::pair<piece_data *, piece_data *> game_data::ray_cast_x2(const sb arm, const piece_data &piece) {
	auto [friendly_board, enemy_board]{get_boards(piece.color)};
	auto [friendly_pieces, enemy_pieces]{get_pieces(piece.color)};

	// calculate the hits on the arm
	sb hits{(*friendly_board | *enemy_board) & arm};

	// return null if no hits
	if (hits == 0) { return std::pair{nullptr, nullptr}; }

	int first_hit_index{0};

	// get msb or lsb and build masks
	if (arm > piece.position) {
		// gets the trailing zeros of the board, giving the index of the first hit
		first_hit_index = __builtin_ctzll(hits);
	} else {
		// gets leading zeros of the board; to get the index, takes this value from 63 (max index of the board)
		first_hit_index = 63 - __builtin_clzll(hits);
	}

	sb hit_board{sb{1} << first_hit_index};

	// get the pointer of the piece that got hit
	piece_data *first_hit_ptr{
		(hit_board & *friendly_board)
			? &((*friendly_pieces)[piece_lookup[first_hit_index]])
			: &((*enemy_pieces)[piece_lookup[first_hit_index]])
	};

	hits &= ~hit_board;

	// return first_hit if no hits
	if (hits == 0) { return std::pair{first_hit_ptr, nullptr}; }

	int second_hit_index{0};

	// get msb or lsb and build masks
	if (arm > piece.position) {
		// gets the trailing zeros of the board, giving the index of the first hit
		second_hit_index = __builtin_ctzll(hits);
	} else {
		// gets leading zeros of the board; to get the index, takes this value from 63 (max index of the board)
		second_hit_index = 63 - __builtin_clzll(hits);
	}

	hit_board = sb{1} << second_hit_index;

	// get the pointer of the piece that got hit
	piece_data *second_hit_ptr{
		(hit_board & *friendly_board)
			? &((*friendly_pieces)[piece_lookup[second_hit_index]])
			: &((*enemy_pieces)[piece_lookup[second_hit_index]])
	};

	// return the pointer of the piece
	return std::pair{first_hit_ptr, second_hit_ptr};
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
	std::span<const sb> lt{};

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
		// create the possible move set as being the moves between the two positions (first removed, last sometimes)
		output = between_table[sb_to_int(piece.position)][sb_to_int(hit.position)] & ~(
			         piece.position | (piece.color == hit.color ? hit.position : 0x0ULL));
	}

	return output;
}

sb game_data::get_valid_moves(const int pos, const lookup_tables &lookup_table, const between_tables &between_table) {
	sb output{0};

	// get the piece
	piece_color piece_color{get_color(sb{1} << pos)};
	const piece_data piece{
		piece_color == piece_color::WHITE ? white_pieces[piece_lookup[pos]] : black_pieces[piece_lookup[pos]]
	};

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

void game_data::move(const int old_pos, const int new_pos, const lookup_tables &lookup_table,
                     const between_tables &between_table) {
	// get the piece
	piece_color piece_color{get_color(sb{1} << old_pos)};
	piece_data *piece{
		piece_color == piece_color::WHITE ? &white_pieces[piece_lookup[old_pos]] : &black_pieces[piece_lookup[old_pos]]
	};

	// remove all prev pins if king moves
	if (piece->type == piece_type::KING) {
		// iterate over all arms
		for (const auto arm: lookup_table.queen_table[sb_to_int(piece->position)]) {
			// cast out rays
			piece_data *rayed_piece = ray_cast_x1(arm, *piece);
			// if there is a piece on the ray, remove any pin
			if (rayed_piece) { rayed_piece->pinner_id = 255; }
		}
	}

	// get boards
	auto [friendly_board, enemy_board] = get_boards(piece->color);

	// update captured piece
	if (piece_lookup[new_pos] != 255) {
		// get the captured piece (will always be the opposite color)
		piece_data *captured_piece = piece_color == piece_color::WHITE
			                             ? &black_pieces[piece_lookup[old_pos]]
			                             : &white_pieces[piece_lookup[old_pos]];
		*enemy_board &= ~captured_piece->position;
		captured_piece->reset();
	}

	// update game_data
	piece_lookup[old_pos] = 255;
	piece_lookup[new_pos] = piece->id;
	*friendly_board &= ~(sb{1} << old_pos);
	*friendly_board |= sb{1} << new_pos;

	// update piece
	piece->position = sb{1} << new_pos;
	piece->attacks = get_valid_moves(new_pos, lookup_table, between_table);

	// update pins
	auto [friendly_pieces, enemy_pieces]{get_pieces(piece->color)};

	// reminder! all arms must include the current position of the piece so the pins re-update on king move!

	// iterate over all arms for friendly king
	for (const auto arm: lookup_table.queen_table[sb_to_int((*friendly_pieces)[15].position)]) {
		// only check arms that have changed
		if (!(arm & piece->position || arm & (sb{1} << old_pos))) { continue; }
		// cast out rays
		auto [fst, snd] = ray_cast_x2(arm, *piece);
		// check if the piece could be a pinner
		if (!snd->is_slider) { continue; }
		// if the second rayed piece has attacks along arm towards king
		if (snd->attacks & arm) {
			// the first rayed piece is pinned
			fst->pinner_id = snd->id;
		}
	}

	// iterate over all arms for enemy king
	for (const auto arm: lookup_table.queen_table[sb_to_int((*enemy_pieces)[15].position)]) {
		// only check arms that have changed
		if (!(arm & piece->position || arm & (sb{1} << old_pos))) { continue; }
		// cast out rays
		auto [fst, snd] = ray_cast_x2(arm, *piece);
		// check if the piece could be a pinner
		if (!snd->is_slider) { continue; }
		// if the second rayed piece has attacks along arm towards king
		if (snd->attacks & arm) {
			// the first rayed piece is pinned
			fst->pinner_id = snd->id;
		}
	}
}

int game_data::evaluate_position() const {
	int eval = 0;
	// count up material
	for (const auto &piece: white_pieces) { eval += piece.value; }

	for (const auto &piece: black_pieces) { eval -= piece.value; }

	return eval;
}

std::string game_data::get() const {
	std::string output;
	const sb full_board{white_board | black_board};

	int counter{0};

	for (int i{63}; i >= 0; i--) {
		const sb temp_pos = sb{1} << i;

		if (i % 8 == 7 && i != 63) {
			if (counter != 0) {
				output += std::to_string(counter);
				counter = 0;
			}

			output += '/';
		}

		if (full_board & temp_pos) {
			if (counter != 0) {
				output += std::to_string(counter);
				counter = 0;
			}

			// get the piece
			piece_color piece_color{get_color(temp_pos)};
			const piece_data piece{
				piece_color == piece_color::WHITE ? white_pieces[piece_lookup[i]] : black_pieces[piece_lookup[i]]
			};

			constexpr char piece_chars[] = {'P', 'N', 'B', 'R', 'Q', 'K'};

			// dictionary for piece characters
			char c{piece_chars[static_cast<int>(piece.type)]};

			// if the piece is black, set to lower case
			if (piece_color == piece_color::BLACK) { c = std::tolower(c); }

			output += c;
		} else { counter++; }
	}

	if (counter != 0) { output += std::to_string(counter); }

	return output;
}


void game_data::set(const std::string &fen) {
	// reset all old data
	white_board = 0;
	black_board = 0;
	white_pieces = {};
	black_pieces = {};
	std::fill(piece_lookup.begin(), piece_lookup.end(), 255);

	std::array<sb, 12> b_boards{};

	// dictionary for piece characters
	std::unordered_map<char, int> char_lookup{{'P', 0}, {'N', 1}, {'B', 2}, {'R', 3}, {'Q', 4}, {'K', 5}};

	// loop through fen string
	int pos{63};
	for (const char c: fen) {
		// skip over formatting
		if (c == '/') { continue; }

		// go through empty positons
		if (isdigit(c)) {
			pos -= c - '0';
			continue;
		}

		// get the correct board index
		int board{0};
		if (std::isupper(c)) { board = 6; }
		board += char_lookup[std::toupper(c)];

		// adds the piece to bit board
		b_boards[board] |= sb{1} << pos;

		pos--;
	}

	int white_piece_count{0};
	int black_piece_count{0};

	// create the piece arrays
	for (int i{0}; i < 12; i++) {
		while (b_boards[i]) {
			// get the index of the piece in the bit board
			const int b_pos{__builtin_ctzll(b_boards[i])};

			// if i >= 6, this is a white board else black
			if (i >= 6) {
				// set color board, color pieces, and piece_lookup
				white_board |= sb{1} << b_pos;
				piece_lookup[b_pos] = white_piece_count;

				// king must go last
				if (i == 11) {
					piece_lookup[b_pos] = 15;
					white_pieces[15] = piece_data(sb{1} << b_pos, static_cast<piece_type>(i - 6),
					                              piece_color::WHITE, 15);
					b_boards[i] &= ~(sb{1} << b_pos);
					continue;
				}

				white_pieces[white_piece_count] = piece_data(sb{1} << b_pos, static_cast<piece_type>(i - 6),
				                                             piece_color::WHITE, white_piece_count);
				white_piece_count++;
			} else {
				// set color board, color pieces, and piece_lookup
				black_board |= sb{1} << b_pos;
				piece_lookup[b_pos] = black_piece_count;

				// king must go last
				if (i == 5) {
					piece_lookup[b_pos] = 15;
					black_pieces[15] = piece_data(sb{1} << b_pos, static_cast<piece_type>(i),
					                              piece_color::BLACK, 15);
					b_boards[i] &= ~(sb{1} << b_pos);
					continue;
				}

				black_pieces[black_piece_count] = piece_data(sb{1} << b_pos, static_cast<piece_type>(i),
				                                             piece_color::BLACK, black_piece_count);
				black_piece_count++;
			}

			b_boards[i] &= ~(sb{1} << b_pos);
		}
	}
}
