#pragma once

#include "types/types.h"

class game_data {
	sb get_valid_moves(int pos, const lookup_tables &lookup_table, const between_tables &between_table);

	sb pawn_logic(const piece_data &piece);
	sb king_logic(const piece_data &piece, int pos, const lookup_tables &lookup_table);
	sb slider_logic(const piece_data &piece, const lookup_tables &lookup_table,
	                const between_tables &between_table);


	piece_data *ray_cast_x1(sb arm, const piece_data &piece);
	std::pair<piece_data *, piece_data *> ray_cast_x2(sb arm, const piece_data &piece);

	static int sb_to_int(const sb board) { return __builtin_ctzll(board); }

	piece_color get_color(sb pos) const;
	std::pair<sb *, sb *> get_boards(piece_color color);
	std::pair<std::array<piece_data, 16> *, std::array<piece_data, 16> *> get_pieces(piece_color color);

public:
	sb white_board;
	sb black_board;
	std::array<uint8_t, 64> piece_lookup;
	std::array<piece_data, 16> white_pieces; // the last piece must be king
	std::array<piece_data, 16> black_pieces; // the last piece must be king

	void move(int old_pos, int new_pos, const lookup_tables &lookup_table, const between_tables &between_table);
};
