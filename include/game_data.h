#pragma once

#include "types.h"

#include <string>

class game_data {
	piece_data *ray_cast_x1(sb arm, const piece_data &piece);
	std::pair<piece_data *, piece_data *> ray_cast_x2(sb arm, const piece_data &piece);

	sb pawn_logic(const piece_data &piece);
	sb king_logic(const piece_data &piece, int pos, const lookup_tables &lookup_table);
	sb slider_logic(const piece_data &piece, const lookup_tables &lookup_table,
	                const between_tables &between_table);

	void update_attack_boards(const lookup_tables &lookup_table, const between_tables &between_table);
	void update_pins(auto &piece_set, const auto &table);

public:
	sb white_board{};
	sb black_board{};
	sb en_passant_board{};
	std::array<uint8_t, 64> piece_lookup{};
	std::array<piece_data, 16> white_pieces; // the last piece must be king
	std::array<piece_data, 16> black_pieces; // the last piece must be king
	std::array<sb, 2> side_attacks{};

	explicit game_data(const std::string &fen, const lookup_tables &lookup_table, const between_tables &between_table) {
		set(fen, lookup_table, between_table);
	}

	[[nodiscard]] std::string get() const;
	void set(const std::string &fen, const lookup_tables &lookup_table, const between_tables &between_table);

	static int sb_to_int(const sb board) { return __builtin_ctzll(board); }

	[[nodiscard]] piece_color get_color(sb pos) const;
	[[nodiscard]] std::pair<sb *, sb *> get_boards(piece_color color);
	[[nodiscard]] std::pair<std::array<piece_data, 16> *, std::array<piece_data, 16> *> get_pieces(piece_color color);

	[[nodiscard]] int evaluate_position() const;

	[[nodiscard]] sb get_valid_moves(int pos, const lookup_tables &lookup_table, const between_tables &between_table);
	void move(int old_idx, int new_idx, const lookup_tables &lookup_table, const between_tables &between_table);
};
