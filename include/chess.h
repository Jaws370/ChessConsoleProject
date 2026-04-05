#pragma once

#include "game_data.h"
#include "types.h"

#include <string>

class chess {
	game_data gd;

	piece_color p1_color;
	piece_color p2_color;

	lookup_tables lookup_table;
	between_tables between_table;

	void table_init();
	int minimax(game_data pseudo_gd, int depth, bool is_maximizing);

public:
	explicit chess(const std::string &fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

	void move(const int old_pos, const int new_pos) { gd.move(old_pos, new_pos, lookup_table, between_table); }
	void ai_move(int depth);

	bool check_move(int old_pos, int new_pos);

	sb get_valid_moves(const int pos) { return gd.get_valid_moves(pos, lookup_table, between_table); }

	void set_board(const std::string &fen) { gd.set(fen); };
	std::string get_board() const { return gd.get(); };
};
