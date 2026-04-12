#include <bitset>
#include <iostream>
#include "../include/chess.h"

void print_bit_board(const sb board) {
	const std::bitset<64> b_set_board = board;

	for (int i{63}; i >= 0; i--) {
		std::cout << (b_set_board[i] == 1 ? 'X' : '0') << " ";

		if (i % 8 == 0) { std::cout << std::endl; }
	}
}

bool test_valid_moves(const sb board, const sb correct_board, const std::string &test_name) {
	if (board == correct_board) {
		std::cout << "[PASSED] " << test_name << std::endl;
		return true;
	}

	std::cout << "[FAILED] " << test_name << std::endl;
	print_bit_board(board);
	std::cout << std::endl << "output should be:" << std::endl;
	print_bit_board(correct_board);

	return false;
}

bool test_check_moves(const bool result, const bool correct_result, const std::string &test_name) {
	if (result == correct_result) {
		std::cout << "[PASSED] " << test_name << std::endl;
		return true;
	}

	std::cout << "[FAILED] " << test_name << std::endl;
	std::cout << (result ? "true" : "false") << std::endl << "output should be: " << (correct_result ? "true" : "false")
			<< std::endl;

	return false;
}

int main() {
	std::string failed_tests;
	int passed{0};
	int total{0};

	chess game;

	std::string test_name = "white pawn at index 9, default board";
	uint64_t correct_board = 0x0000000002020000ULL;
	if (test_valid_moves(game.get_valid_moves(9), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/5n2/8/8/8");
	test_name = "black knight at index 26, empty board";
	correct_board = 0x00000A1100110A00ULL;
	if (test_valid_moves(game.get_valid_moves(26), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/5p2/8/8/8");
	test_name = "black pawn at index 26, empty board";
	correct_board = 0x0000000000040000ULL;
	if (test_valid_moves(game.get_valid_moves(26), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/5b2/8/8/8");
	test_name = "black bishop at index 26, empty board";
	correct_board = 0x4020110A000A1120ULL;
	if (test_valid_moves(game.get_valid_moves(26), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/5r2/8/8/8");
	test_name = "black rook at index 26, empty board";
	correct_board = 0x04040404FB040404ULL;
	if (test_valid_moves(game.get_valid_moves(26), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/5q2/8/8/8");
	test_name = "black queen at index 26, empty board";
	correct_board = 0x4424150EFB0E1524ULL;
	if (test_valid_moves(game.get_valid_moves(26), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/5k2/8/8/8");
	test_name = "black king at index 26, empty board";
	correct_board = 0x0000000E0A0E0000ULL;
	if (test_valid_moves(game.get_valid_moves(26), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;
	// --- white pawn (not on rank 8) ---
	game.set_board("8/8/8/8/8/8/8/7P");
	test_name = "white pawn at index 0 (h1), empty board";
	correct_board = 0x0000000000000100ULL;
	if (test_valid_moves(game.get_valid_moves(0), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/8/8/8/P7");
	test_name = "white pawn at index 7 (a1), empty board";
	correct_board = 0x0000000000008000ULL;
	if (test_valid_moves(game.get_valid_moves(7), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// --- black pawn (not on rank 1) ---
	game.set_board("7p/8/8/8/8/8/8/8");
	test_name = "black pawn at index 56 (h8), empty board";
	correct_board = 0x0001000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(56), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("p7/8/8/8/8/8/8/8");
	test_name = "black pawn at index 63 (a8), empty board";
	correct_board = 0x0080000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(63), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// --- knight ---
	game.set_board("8/8/8/8/8/8/8/7N");
	test_name = "knight at index 0 (h1), empty board";
	correct_board = 0x0000000000020400ULL;
	if (test_valid_moves(game.get_valid_moves(0), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/8/8/8/N7");
	test_name = "knight at index 7 (a1), empty board";
	correct_board = 0x0000000000402000ULL;
	if (test_valid_moves(game.get_valid_moves(7), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("7N/8/8/8/8/8/8/8");
	test_name = "knight at index 56 (h8), empty board";
	correct_board = 0x0004020000000000ULL;
	if (test_valid_moves(game.get_valid_moves(56), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("N7/8/8/8/8/8/8/8");
	test_name = "knight at index 63 (a8), empty board";
	correct_board = 0x0020400000000000ULL;
	if (test_valid_moves(game.get_valid_moves(63), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// --- bishop ---
	game.set_board("8/8/8/8/8/8/8/7B");
	test_name = "bishop at index 0 (h1), empty board";
	correct_board = 0x8040201008040200ULL;
	if (test_valid_moves(game.get_valid_moves(0), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/8/8/8/B7");
	test_name = "bishop at index 7 (a1), empty board";
	correct_board = 0x0102040810204000ULL;
	if (test_valid_moves(game.get_valid_moves(7), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("7B/8/8/8/8/8/8/8");
	test_name = "bishop at index 56 (h8), empty board";
	correct_board = 0x0002040810204080ULL;
	if (test_valid_moves(game.get_valid_moves(56), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("B7/8/8/8/8/8/8/8");
	test_name = "bishop at index 63 (a8), empty board";
	correct_board = 0x0040201008040201ULL;
	if (test_valid_moves(game.get_valid_moves(63), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// --- rook ---
	game.set_board("8/8/8/8/8/8/8/7R");
	test_name = "rook at index 0 (h1), empty board";
	correct_board = 0x01010101010101FEULL;
	if (test_valid_moves(game.get_valid_moves(0), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/8/8/8/R7");
	test_name = "rook at index 7 (a1), empty board";
	correct_board = 0x808080808080807FULL;
	if (test_valid_moves(game.get_valid_moves(7), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("7R/8/8/8/8/8/8/8");
	test_name = "rook at index 56 (h8), empty board";
	correct_board = 0xFE01010101010101ULL;
	if (test_valid_moves(game.get_valid_moves(56), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("R7/8/8/8/8/8/8/8");
	test_name = "rook at index 63 (a8), empty board";
	correct_board = 0x7F80808080808080ULL;
	if (test_valid_moves(game.get_valid_moves(63), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// --- queen ---
	game.set_board("8/8/8/8/8/8/8/7Q");
	test_name = "queen at index 0 (h1), empty board";
	correct_board = 0x81412111090503FEULL;
	if (test_valid_moves(game.get_valid_moves(0), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/8/8/8/Q7");
	test_name = "queen at index 7 (a1), empty board";
	correct_board = 0x8182848890A0C07FULL;
	if (test_valid_moves(game.get_valid_moves(7), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("7Q/8/8/8/8/8/8/8");
	test_name = "queen at index 56 (h8), empty board";
	correct_board = 0xFE03050911214181ULL;
	if (test_valid_moves(game.get_valid_moves(56), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("Q7/8/8/8/8/8/8/8");
	test_name = "queen at index 63 (a8), empty board";
	correct_board = 0x7FC0A09088848281ULL;
	if (test_valid_moves(game.get_valid_moves(63), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// --- king ---
	game.set_board("8/8/8/8/8/8/8/7K");
	test_name = "king at index 0 (h1), empty board";
	correct_board = 0x0000000000000302ULL;
	if (test_valid_moves(game.get_valid_moves(0), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/8/8/8/K7");
	test_name = "king at index 7 (a1), empty board";
	correct_board = 0x000000000000C040ULL;
	if (test_valid_moves(game.get_valid_moves(7), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("7K/8/8/8/8/8/8/8");
	test_name = "king at index 56 (h8), empty board";
	correct_board = 0x0203000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(56), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("K7/8/8/8/8/8/8/8");
	test_name = "king at index 63 (a8), empty board";
	correct_board = 0x40C0000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(63), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"); // default

	// --- white pawns (rank 2, indices 8-15) ---
	test_name = "white pawn at index 8 (h2), default board";
	correct_board = 0x0000000001010000ULL;
	if (test_valid_moves(game.get_valid_moves(8), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "white pawn at index 9 (g2), default board";
	correct_board = 0x0000000002020000ULL;
	if (test_valid_moves(game.get_valid_moves(9), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "white pawn at index 10 (f2), default board";
	correct_board = 0x0000000004040000ULL;
	if (test_valid_moves(game.get_valid_moves(10), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "white pawn at index 11 (e2), default board";
	correct_board = 0x0000000008080000ULL;
	if (test_valid_moves(game.get_valid_moves(11), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "white pawn at index 12 (d2), default board";
	correct_board = 0x0000000010100000ULL;
	if (test_valid_moves(game.get_valid_moves(12), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "white pawn at index 13 (c2), default board";
	correct_board = 0x0000000020200000ULL;
	if (test_valid_moves(game.get_valid_moves(13), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "white pawn at index 14 (b2), default board";
	correct_board = 0x0000000040400000ULL;
	if (test_valid_moves(game.get_valid_moves(14), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "white pawn at index 15 (a2), default board";
	correct_board = 0x0000000080800000ULL;
	if (test_valid_moves(game.get_valid_moves(15), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// --- black pawns (rank 7, indices 48-55) ---
	test_name = "black pawn at index 48 (h7), default board";
	correct_board = 0x0000010100000000ULL;
	if (test_valid_moves(game.get_valid_moves(48), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "black pawn at index 49 (g7), default board";
	correct_board = 0x0000020200000000ULL;
	if (test_valid_moves(game.get_valid_moves(49), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "black pawn at index 50 (f7), default board";
	correct_board = 0x0000040400000000ULL;
	if (test_valid_moves(game.get_valid_moves(50), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "black pawn at index 51 (e7), default board";
	correct_board = 0x0000080800000000ULL;
	if (test_valid_moves(game.get_valid_moves(51), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "black pawn at index 52 (d7), default board";
	correct_board = 0x0000101000000000ULL;
	if (test_valid_moves(game.get_valid_moves(52), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "black pawn at index 53 (c7), default board";
	correct_board = 0x0000202000000000ULL;
	if (test_valid_moves(game.get_valid_moves(53), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "black pawn at index 54 (b7), default board";
	correct_board = 0x0000404000000000ULL;
	if (test_valid_moves(game.get_valid_moves(54), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "black pawn at index 55 (a7), default board";
	correct_board = 0x0000808000000000ULL;
	if (test_valid_moves(game.get_valid_moves(55), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// --- white pieces on rank 1 (all blocked except knights) ---
	test_name = "white rook at index 0 (h1), default board";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(0), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "white knight at index 1 (g1), default board";
	correct_board = 0x0000000000050000ULL;
	if (test_valid_moves(game.get_valid_moves(1), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "white bishop at index 2 (f1), default board";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(2), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "white king at index 3 (e1), default board";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(3), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "white queen at index 4 (d1), default board";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(4), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "white bishop at index 5 (c1), default board";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(5), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "white knight at index 6 (b1), default board";
	correct_board = 0x0000000000A00000ULL;
	if (test_valid_moves(game.get_valid_moves(6), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "white rook at index 7 (a1), default board";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(7), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// --- black pieces on rank 8 (all blocked except knights) ---
	test_name = "black rook at index 56 (h8), default board";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(56), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "black knight at index 57 (g8), default board";
	correct_board = 0x0000050000000000ULL;
	if (test_valid_moves(game.get_valid_moves(57), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "black bishop at index 58 (f8), default board";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(58), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "black king at index 59 (e8), default board";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(59), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "black queen at index 60 (d8), default board";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(60), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "black bishop at index 61 (c8), default board";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(61), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "black knight at index 62 (b8), default board";
	correct_board = 0x0000A00000000000ULL;
	if (test_valid_moves(game.get_valid_moves(62), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	test_name = "black rook at index 63 (a8), default board";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(63), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// =====================
	// ON THE WALL
	// =====================

	// --- white pawn ---
	game.set_board("8/8/8/8/8/8/8/3P4");
	test_name = "white pawn at index 4 (d1), empty board";
	correct_board = 0x0000000000001000ULL;
	if (test_valid_moves(game.get_valid_moves(4), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/P7/8/8/8");
	test_name = "white pawn at index 31 (a4), empty board";
	correct_board = 0x0000008000000000ULL;
	if (test_valid_moves(game.get_valid_moves(31), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/7P/8/8/8");
	test_name = "white pawn at index 24 (h4), empty board";
	correct_board = 0x0000000100000000ULL;
	if (test_valid_moves(game.get_valid_moves(24), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// --- black pawn ---
	game.set_board("8/8/8/8/p7/8/8/8");
	test_name = "black pawn at index 31 (a4), empty board";
	correct_board = 0x0000000000800000ULL;
	if (test_valid_moves(game.get_valid_moves(31), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("3p4/8/8/8/8/8/8/8");
	test_name = "black pawn at index 60 (d8), empty board";
	correct_board = 0x0010000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(60), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/7p/8/8/8");
	test_name = "black pawn at index 24 (h4), empty board";
	correct_board = 0x0000000000010000ULL;
	if (test_valid_moves(game.get_valid_moves(24), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// --- knight ---
	game.set_board("8/8/8/8/8/8/8/3N4");
	test_name = "knight at index 4 (d1), empty board";
	correct_board = 0x0000000000284400ULL;
	if (test_valid_moves(game.get_valid_moves(4), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/N7/8/8/8");
	test_name = "knight at index 31 (a4), empty board";
	correct_board = 0x0000402000204000ULL;
	if (test_valid_moves(game.get_valid_moves(31), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("3N4/8/8/8/8/8/8/8");
	test_name = "knight at index 60 (d8), empty board";
	correct_board = 0x0044280000000000ULL;
	if (test_valid_moves(game.get_valid_moves(60), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/7N/8/8/8");
	test_name = "knight at index 24 (h4), empty board";
	correct_board = 0x0000020400040200ULL;
	if (test_valid_moves(game.get_valid_moves(24), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// --- bishop ---
	game.set_board("8/8/8/8/8/8/8/3B4");
	test_name = "bishop at index 4 (d1), empty board";
	correct_board = 0x0000000182442800ULL;
	if (test_valid_moves(game.get_valid_moves(4), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/B7/8/8/8");
	test_name = "bishop at index 31 (a4), empty board";
	correct_board = 0x0810204000402010ULL;
	if (test_valid_moves(game.get_valid_moves(31), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("3B4/8/8/8/8/8/8/8");
	test_name = "bishop at index 60 (d8), empty board";
	correct_board = 0x0028448201000000ULL;
	if (test_valid_moves(game.get_valid_moves(60), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/7B/8/8/8");
	test_name = "bishop at index 24 (h4), empty board";
	correct_board = 0x1008040200020408ULL;
	if (test_valid_moves(game.get_valid_moves(24), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// --- rook ---
	game.set_board("8/8/8/8/8/8/8/3R4");
	test_name = "rook at index 4 (d1), empty board";
	correct_board = 0x10101010101010EFULL;
	if (test_valid_moves(game.get_valid_moves(4), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/R7/8/8/8");
	test_name = "rook at index 31 (a4), empty board";
	correct_board = 0x808080807F808080ULL;
	if (test_valid_moves(game.get_valid_moves(31), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("3R4/8/8/8/8/8/8/8");
	test_name = "rook at index 60 (d8), empty board";
	correct_board = 0xEF10101010101010ULL;
	if (test_valid_moves(game.get_valid_moves(60), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/7R/8/8/8");
	test_name = "rook at index 24 (h4), empty board";
	correct_board = 0x01010101FE010101ULL;
	if (test_valid_moves(game.get_valid_moves(24), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// --- queen ---
	game.set_board("8/8/8/8/8/8/8/3Q4");
	test_name = "queen at index 4 (d1), empty board";
	correct_board = 0x10101011925438EFULL;
	if (test_valid_moves(game.get_valid_moves(4), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/Q7/8/8/8");
	test_name = "queen at index 31 (a4), empty board";
	correct_board = 0x8890A0C07FC0A090ULL;
	if (test_valid_moves(game.get_valid_moves(31), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("3Q4/8/8/8/8/8/8/8");
	test_name = "queen at index 60 (d8), empty board";
	correct_board = 0xEF38549211101010ULL;
	if (test_valid_moves(game.get_valid_moves(60), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/7Q/8/8/8");
	test_name = "queen at index 24 (h4), empty board";
	correct_board = 0x11090503FE030509ULL;
	if (test_valid_moves(game.get_valid_moves(24), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// --- king ---
	game.set_board("8/8/8/8/8/8/8/3K4");
	test_name = "king at index 4 (d1), empty board";
	correct_board = 0x0000000000003828ULL;
	if (test_valid_moves(game.get_valid_moves(4), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/K7/8/8/8");
	test_name = "king at index 31 (a4), empty board";
	correct_board = 0x000000C040C00000ULL;
	if (test_valid_moves(game.get_valid_moves(31), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("3K4/8/8/8/8/8/8/8");
	test_name = "king at index 60 (d8), empty board";
	correct_board = 0x2838000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(60), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/7K/8/8/8");
	test_name = "king at index 24 (h4), empty board";
	correct_board = 0x0000000302030000ULL;
	if (test_valid_moves(game.get_valid_moves(24), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// =====================
	// ONE SPACE FROM WALL
	// =====================

	// --- white pawn ---
	game.set_board("8/8/8/8/8/8/3P4/8");
	test_name = "white pawn at index 12 (d2), empty board";
	correct_board = 0x0000000010100000ULL;
	if (test_valid_moves(game.get_valid_moves(12), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/1P6/8/8/8");
	test_name = "white pawn at index 30 (b4), empty board";
	correct_board = 0x0000004000000000ULL;
	if (test_valid_moves(game.get_valid_moves(30), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/3P4/8/8/8/8/8/8");
	test_name = "white pawn at index 52 (d7), empty board";
	correct_board = 0x1000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(52), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/6P1/8/8/8");
	test_name = "white pawn at index 25 (g4), empty board";
	correct_board = 0x0000000200000000ULL;
	if (test_valid_moves(game.get_valid_moves(25), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// --- black pawn ---
	game.set_board("8/8/8/8/8/8/3p4/8");
	test_name = "black pawn at index 12 (d2), empty board";
	correct_board = 0x0000000000000010ULL;
	if (test_valid_moves(game.get_valid_moves(12), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/1p6/8/8/8");
	test_name = "black pawn at index 30 (b4), empty board";
	correct_board = 0x0000000000400000ULL;
	if (test_valid_moves(game.get_valid_moves(30), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/3p4/8/8/8/8/8/8");
	test_name = "black pawn at index 52 (d7), empty board";
	correct_board = 0x0000101000000000ULL;
	if (test_valid_moves(game.get_valid_moves(52), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/6p1/8/8/8");
	test_name = "black pawn at index 25 (g4), empty board";
	correct_board = 0x0000000000020000ULL;
	if (test_valid_moves(game.get_valid_moves(25), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// --- knight ---
	game.set_board("8/8/8/8/8/8/3N4/8");
	test_name = "knight at index 12 (d2), empty board";
	correct_board = 0x0000000028440044ULL;
	if (test_valid_moves(game.get_valid_moves(12), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/1N6/8/8/8");
	test_name = "knight at index 30 (b4), empty board";
	correct_board = 0x0000A0100010A000ULL;
	if (test_valid_moves(game.get_valid_moves(30), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/3N4/8/8/8/8/8/8");
	test_name = "knight at index 52 (d7), empty board";
	correct_board = 0x4400442800000000ULL;
	if (test_valid_moves(game.get_valid_moves(52), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/6N1/8/8/8");
	test_name = "knight at index 25 (g4), empty board";
	correct_board = 0x0000050800080500ULL;
	if (test_valid_moves(game.get_valid_moves(25), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// --- bishop ---
	game.set_board("8/8/8/8/8/8/3B4/8");
	test_name = "bishop at index 12 (d2), empty board";
	correct_board = 0x0000018244280028ULL;
	if (test_valid_moves(game.get_valid_moves(12), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/1B6/8/8/8");
	test_name = "bishop at index 30 (b4), empty board";
	correct_board = 0x040810A000A01008ULL;
	if (test_valid_moves(game.get_valid_moves(30), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/3B4/8/8/8/8/8/8");
	test_name = "bishop at index 52 (d7), empty board";
	correct_board = 0x2800284482010000ULL;
	if (test_valid_moves(game.get_valid_moves(52), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/6B1/8/8/8");
	test_name = "bishop at index 25 (g4), empty board";
	correct_board = 0x2010080500050810ULL;
	if (test_valid_moves(game.get_valid_moves(25), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// --- rook ---
	game.set_board("8/8/8/8/8/8/3R4/8");
	test_name = "rook at index 12 (d2), empty board";
	correct_board = 0x101010101010EF10ULL;
	if (test_valid_moves(game.get_valid_moves(12), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/1R6/8/8/8");
	test_name = "rook at index 30 (b4), empty board";
	correct_board = 0x40404040BF404040ULL;
	if (test_valid_moves(game.get_valid_moves(30), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/3R4/8/8/8/8/8/8");
	test_name = "rook at index 52 (d7), empty board";
	correct_board = 0x10EF101010101010ULL;
	if (test_valid_moves(game.get_valid_moves(52), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/6R1/8/8/8");
	test_name = "rook at index 25 (g4), empty board";
	correct_board = 0x02020202FD020202ULL;
	if (test_valid_moves(game.get_valid_moves(25), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// --- queen ---
	game.set_board("8/8/8/8/8/8/3Q4/8");
	test_name = "queen at index 12 (d2), empty board";
	correct_board = 0x101011925438EF38ULL;
	if (test_valid_moves(game.get_valid_moves(12), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/1Q6/8/8/8");
	test_name = "queen at index 30 (b4), empty board";
	correct_board = 0x444850E0BFE05048ULL;
	if (test_valid_moves(game.get_valid_moves(30), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/3Q4/8/8/8/8/8/8");
	test_name = "queen at index 52 (d7), empty board";
	correct_board = 0x38EF385492111010ULL;
	if (test_valid_moves(game.get_valid_moves(52), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/6Q1/8/8/8");
	test_name = "queen at index 25 (g4), empty board";
	correct_board = 0x22120A07FD070A12ULL;
	if (test_valid_moves(game.get_valid_moves(25), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// --- king ---
	game.set_board("8/8/8/8/8/8/3K4/8");
	test_name = "king at index 12 (d2), empty board";
	correct_board = 0x0000000000382838ULL;
	if (test_valid_moves(game.get_valid_moves(12), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/1K6/8/8/8");
	test_name = "king at index 30 (b4), empty board";
	correct_board = 0x000000E0A0E00000ULL;
	if (test_valid_moves(game.get_valid_moves(30), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/3K4/8/8/8/8/8/8");
	test_name = "king at index 52 (d7), empty board";
	correct_board = 0x3828380000000000ULL;
	if (test_valid_moves(game.get_valid_moves(52), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/6K1/8/8/8");
	test_name = "king at index 25 (g4), empty board";
	correct_board = 0x0000000705070000ULL;
	if (test_valid_moves(game.get_valid_moves(25), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// =====================
	// BLOCKER TESTS
	// =====================

	game.set_board("8/8/8/4P3/4P3/8/8/8");
	test_name = "white pawn e4, friendly pawn blocks on e5";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/3p4/3P4/8/8/8");
	test_name = "white pawn d4, enemy pawn blocks on d5";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(28), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/4p3/8/4P3/8");
	test_name = "white pawn e2, enemy on e4 blocks double push";
	correct_board = 0x0000000000080000ULL;
	if (test_valid_moves(game.get_valid_moves(11), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/8/4p3/4P3/8");
	test_name = "white pawn e2, enemy on e3 blocks all";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(11), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/4p3/4p3/8/8/8");
	test_name = "black pawn e5, friendly pawn blocks on e4";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(35), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/3p4/8/3P4/8/8/8/8");
	test_name = "black pawn d7, enemy on d5 blocks double push";
	correct_board = 0x0000100000000000ULL;
	if (test_valid_moves(game.get_valid_moves(52), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/3p4/3P4/8/8/8/8/8");
	test_name = "black pawn d7, enemy on d6 blocks all";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(52), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/3P1N2/1p3n2/4N3/8/8/8");
	test_name = "white knight e4, mixed blockers on landing squares";
	correct_board = 0x0000002200221400ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/2N3P1/4n3/8/4pn2/8/8");
	test_name = "black knight d5, mixed blockers on landing squares";
	correct_board = 0x0014220022100000ULL;
	if (test_valid_moves(game.get_valid_moves(35), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/5P1P/3p4/6N1/3P4/R1B5");
	test_name = "white knight b3, mostly blocked by friendly except d4 enemy";
	correct_board = 0x0000000008000805ULL;
	if (test_valid_moves(game.get_valid_moves(17), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/1Pn5/8/4B3/8/5p2/8");
	test_name = "white bishop e4, friendly on g6, enemies on c6 and c2";
	correct_board = 0x0001221400142241ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/2R3p1/8/4b3/8/2b3N1/8/8");
	test_name = "black bishop d5, enemies on f7 and b3, friendly on f3 and b7";
	correct_board = 0x0020140014020000ULL;
	if (test_valid_moves(game.get_valid_moves(35), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/P3P3/8/2B5/8/3q3r");
	test_name = "white bishop f3, friendly on d5 and h5, enemies on d1 and h1";
	correct_board = 0x0000000050005088ULL;
	if (test_valid_moves(game.get_valid_moves(21), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/1p1p4/2b5/1p1p4/8/8/8");
	test_name = "black bishop c5, all diagonals blocked one step by friendly";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(37), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/4P3/8/8/n3R2b/8/4q3/8");
	test_name = "white rook e4, friendly on e7, enemies on e2/a4/h4";
	correct_board = 0x00000808F7080800ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/3R4/8/r2r3p/8/3N4/8");
	test_name = "black rook d4, friendly on a4/h4, enemies on d6/d2";
	correct_board = 0x000010106E101000ULL;
	if (test_valid_moves(game.get_valid_moves(28), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("1P6/8/8/pR2n3/8/8/1P6/8");
	test_name = "white rook b5, friendly on b8/b2, enemies on e5/a5";
	correct_board = 0x004040B840400000ULL;
	if (test_valid_moves(game.get_valid_moves(38), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/5R2/8/2r2r1B/8/8/5p2/8");
	test_name = "black rook f5, friendly on f2/c5, enemies on f7/h5";
	correct_board = 0x0004041B04040000ULL;
	if (test_valid_moves(game.get_valid_moves(34), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/1Pb1P3/8/2n1Q1r1/8/4q3/8");
	test_name = "white queen e4, mixed blockers on all sides";
	correct_board = 0x0001221C361C2A41ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/4pp2/8/1r2q2R/8/2Q1N1B1/8/8");
	test_name = "black queen d5, mixed blockers on all diagonals and ranks";
	correct_board = 0x41221C371C2A0000ULL;
	if (test_valid_moves(game.get_valid_moves(35), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/pbP5/4n3/8/P1Q5/8/2r5");
	test_name = "white queen c3, friendly on c6/a3, enemies on c1/f6/e5/a6";
	correct_board = 0x000000A8705F70A8ULL;
	if (test_valid_moves(game.get_valid_moves(21), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/3R1p1p/8/3n1q1r/8/3B1Q1N/8");
	test_name = "black queen f4, friendly on f6/h4/h6/d4, enemies on f2/d6/d2/h2";
	correct_board = 0x0000100E0A0E1500ULL;
	if (test_valid_moves(game.get_valid_moves(26), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/3PPn2/3PKb2/3qpr2/8/8");
	test_name = "white king e4, friendly on d4/d5/e5, enemies on f5/f4/f3/e3/d3";
	correct_board = 0x0000000000140000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/2p1pp2/2p1kR2/2B1NQ2/8/8/8");
	test_name = "black king d5, friendly on c5/d6/e6/c6, enemies on e5/e4/d4/c4";
	correct_board = 0x0000000010000000ULL;
	if (test_valid_moves(game.get_valid_moves(35), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/6Pq/6PK/6nr/8/8");
	test_name = "white king h4, friendly on g4/g5, enemies on g3/h3/h5";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(24), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/BN6/kp6/pR6/8/8/8");
	test_name = "black king a5, friendly on a4/b5, enemies on b4/b6/a6";
	correct_board = 0x0000C00040000000ULL;
	if (test_valid_moves(game.get_valid_moves(39), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/1p3p2/8/3B4/8/1n3q2/8");
	test_name = "white bishop d4, enemies two steps away on all diagonals";
	correct_board = 0x0000442800284400ULL;
	if (test_valid_moves(game.get_valid_moves(28), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("4K3/8/8/B3r2R/8/8/8/4Q3");
	test_name = "black rook e5, enemies on all four edges of rank/file";
	correct_board = 0x080808F708080808ULL;
	if (test_valid_moves(game.get_valid_moves(35), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/8/8/8/1P1P4/b7/nQ1P4/qr6");
	test_name = "white queen b2, friendly on b4/d4/d2, enemies on b1/a2/a3/a1";
	correct_board = 0x0000000000E0A0E0ULL;
	if (test_valid_moves(game.get_valid_moves(14), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("8/4R3/7p/6bp/8/4Q3/8/8");
	test_name = "black bishop g5, friendly on h6/h4, enemies on e7/e3";
	correct_board = 0x0008040005080000ULL;
	if (test_valid_moves(game.get_valid_moves(33), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 1. white king e4, enemy rook e1, e-file x-ray
	// valid squares: ['d3', 'd4', 'd5', 'f3', 'f4', 'f5']
	game.set_board("8/8/8/8/4K3/8/8/4r3");
	test_name = "white king e4, enemy rook e1, e-file x-ray";
	correct_board = 0x0000001414140000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 2. white king e4, enemy rook a4, rank x-ray
	// valid squares: ['d3', 'd5', 'e3', 'e5', 'f3', 'f5']
	game.set_board("8/8/8/8/r3K3/8/8/8");
	test_name = "white king e4, enemy rook a4, rank x-ray";
	correct_board = 0x0000001C001C0000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 3. white king e4, enemy bishop b1, diagonal x-ray
	// valid squares: ['d4', 'd5', 'e3', 'e5', 'f3', 'f4']
	game.set_board("8/8/8/8/4K3/8/8/1b6");
	test_name = "white king e4, enemy bishop b1, diagonal x-ray";
	correct_board = 0x00000018140C0000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 4. white king e4, enemy bishop h7, anti-diagonal x-ray
	// valid squares: ['d4', 'd5', 'e3', 'e5', 'f3', 'f4']
	game.set_board("8/7b/8/8/4K3/8/8/8");
	test_name = "white king e4, enemy bishop h7, anti-diagonal x-ray";
	correct_board = 0x00000018140C0000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 5. white king e4, enemy queen d1, diagonal+file x-ray
	// valid squares: ['e3', 'e5', 'f4', 'f5']
	game.set_board("8/8/8/8/4K3/8/8/3q4");
	test_name = "white king e4, enemy queen d1, diagonal+file x-ray";
	correct_board = 0x0000000C04080000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 6. white king e4, enemy queen h1, diagonal x-ray
	// valid squares: ['d3', 'd4', 'e3', 'e5', 'f4', 'f5']
	game.set_board("8/8/8/8/4K3/8/8/7q");
	test_name = "white king e4, enemy queen h1, diagonal x-ray";
	correct_board = 0x0000000C14180000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 7. white king e4, undefended enemy rook f3
	// valid squares: ['d4', 'd5', 'e5', 'f3']
	game.set_board("8/8/8/8/4K3/5r2/8/8");
	test_name = "white king e4, undefended enemy rook f3";
	correct_board = 0x0000001810040000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 8. white king e4, enemy rook f3 defended by f2 rook
	// valid squares: ['d4', 'd5', 'e5']
	game.set_board("8/8/8/8/4K3/5r2/5r2/8");
	test_name = "white king e4, enemy rook f3 defended by f2 rook";
	correct_board = 0x0000001810000000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 9. white king e4 in check from rook e2
	// valid squares: ['d3', 'd4', 'd5', 'f3', 'f4', 'f5']
	game.set_board("8/8/8/8/4K3/8/4r3/8");
	test_name = "white king e4 in check from rook e2";
	correct_board = 0x0000001414140000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 10. white king e4, adjacent enemy queen e5
	// valid squares: ['d3', 'e5', 'f3']
	game.set_board("8/8/8/4q3/4K3/8/8/8");
	test_name = "white king e4, adjacent enemy queen e5";
	correct_board = 0x0000000800140000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 11. white king e4, enemy pawns d3 and f3
	// valid squares: ['d3', 'd4', 'd5', 'e3', 'e5', 'f3', 'f4', 'f5']
	game.set_board("8/8/8/8/4K3/3p1p2/8/8");
	test_name = "white king e4, enemy pawns d3 and f3";
	correct_board = 0x0000001C141C0000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 12. white king e4, enemy pawns c5/e5/c3/e3
	// valid squares: ['d3', 'd5', 'e3', 'e5', 'f3', 'f5']
	game.set_board("8/8/8/2p1p3/4K3/2p1p3/8/8");
	test_name = "white king e4, enemy pawns c5/e5/c3/e3";
	correct_board = 0x0000001C001C0000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 13. white king e4, enemy knight f2
	// valid squares: ['d4', 'd5', 'e3', 'e5', 'f3', 'f4', 'f5']
	game.set_board("8/8/8/8/4K3/8/5n2/8");
	test_name = "white king e4, enemy knight f2";
	correct_board = 0x0000001C140C0000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 14. white king e4, enemy knights d6/f6/d2/f2
	// valid squares: ['d4', 'e3', 'e5', 'f4']
	game.set_board("8/8/3n1n2/8/4K3/8/3n1n2/8");
	test_name = "white king e4, enemy knights d6/f6/d2/f2";
	correct_board = 0x0000000814080000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 15. white king e4, all 8 neighbors friendly pawns
	// valid squares: []
	game.set_board("8/8/8/3PPP2/3PKP2/3PPP2/8/8");
	test_name = "white king e4, all 8 neighbors friendly pawns";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 16. white king e4, enemy rook a4 and bishop h4
	// valid squares: ['d3', 'd5', 'e3', 'e5', 'f3', 'f5']
	game.set_board("8/8/8/8/r3K2b/8/8/8");
	test_name = "white king e4, enemy rook a4 and bishop h4";
	correct_board = 0x0000001C001C0000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 17. white king e4, enemy rooks d6 and d2, d-file attacked
	// valid squares: ['e3', 'e5', 'f3', 'f4', 'f5']
	game.set_board("8/8/3r4/8/4K3/8/3r4/8");
	test_name = "white king e4, enemy rooks d6 and d2, d-file attacked";
	correct_board = 0x0000000C040C0000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 18. white king a1, enemy rooks b2 and c1
	// valid squares: ['b2']
	game.set_board("8/8/8/8/8/8/1r6/K1r5");
	test_name = "white king a1, enemy rooks b2 and c1";
	correct_board = 0x0000000000004000ULL;
	if (test_valid_moves(game.get_valid_moves(7), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 19. white king h8, enemy rook g8
	// valid squares: ['g7', 'g8', 'h7']
	game.set_board("6rK/8/8/8/8/8/8/8");
	test_name = "white king h8, enemy rook g8";
	correct_board = 0x0201000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(56), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 20. white king h8, enemy rooks g2 and h2
	// valid squares: []
	game.set_board("7K/8/8/8/8/8/6rr/8");
	test_name = "white king h8, enemy rooks g2 and h2";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(56), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 21. black king a8, open corner
	// valid squares: ['a7', 'b7', 'b8']
	game.set_board("k7/8/8/8/8/8/8/8");
	test_name = "black king a8, open corner";
	correct_board = 0x40C0000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(63), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 22. black king h8, open corner
	// valid squares: ['g7', 'g8', 'h7']
	game.set_board("7k/8/8/8/8/8/8/8");
	test_name = "black king h8, open corner";
	correct_board = 0x0203000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(56), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 23. black king a4, open edge
	// valid squares: ['a3', 'a5', 'b3', 'b4', 'b5']
	game.set_board("8/8/8/8/k7/8/8/8");
	test_name = "black king a4, open edge";
	correct_board = 0x000000C040C00000ULL;
	if (test_valid_moves(game.get_valid_moves(31), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 24. black king h4, open edge
	// valid squares: ['g3', 'g4', 'g5', 'h3', 'h5']
	game.set_board("8/8/8/8/7k/8/8/8");
	test_name = "black king h4, open edge";
	correct_board = 0x0000000302030000ULL;
	if (test_valid_moves(game.get_valid_moves(24), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 25. black king a1, open corner
	// valid squares: ['a2', 'b1', 'b2']
	game.set_board("8/8/8/8/8/8/8/k7");
	test_name = "black king a1, open corner";
	correct_board = 0x000000000000C040ULL;
	if (test_valid_moves(game.get_valid_moves(7), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 26. black king h1, open corner
	// valid squares: ['g1', 'g2', 'h2']
	game.set_board("8/8/8/8/8/8/8/7k");
	test_name = "black king h1, open corner";
	correct_board = 0x0000000000000302ULL;
	if (test_valid_moves(game.get_valid_moves(0), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 27. black king d6, enemy white queen d1, file x-ray
	// valid squares: ['c5', 'c6', 'c7', 'e5', 'e6', 'e7']
	game.set_board("8/8/3k4/8/8/8/8/3Q4");
	test_name = "black king d6, enemy white queen d1, file x-ray";
	correct_board = 0x0028282800000000ULL;
	if (test_valid_moves(game.get_valid_moves(44), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 28. white king d4, enemy rook d1, d-file x-ray
	// valid squares: ['c3', 'c4', 'c5', 'e3', 'e4', 'e5']  (wait, e4 should be... no, king IS on d4)
	game.set_board("8/8/8/8/3K4/8/8/3r4");
	test_name = "white king d4, enemy rook d1, d-file x-ray";
	correct_board = 0x0000002828280000ULL;
	if (test_valid_moves(game.get_valid_moves(28), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 29. white king d4, enemy rook h1, no x-ray on d-file
	// valid squares: ['c3', 'c4', 'c5', 'd3', 'd5', 'e3', 'e4', 'e5']
	game.set_board("8/8/8/8/3K4/8/8/7r");
	test_name = "white king d4, enemy rook h1, no x-ray";
	correct_board = 0x0000003828380000ULL;
	if (test_valid_moves(game.get_valid_moves(28), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 30. white king d4, enemy rooks c6 and c3, c-file attacked
	// valid squares: ['d3', 'd5', 'e3', 'e4', 'e5']
	game.set_board("8/8/2r5/8/3K4/8/2r5/8");
	test_name = "white king d4, enemy rooks c6 and c3, c-file attacked";
	correct_board = 0x0000001808180000ULL;
	if (test_valid_moves(game.get_valid_moves(28), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// --- PIN TESTS ---

	// FILE PINS
	// white bishop e4 pinned on e-file by rook e7 [PINNED]
	// valid squares: []
	game.set_board("4k3/4r3/8/8/4B3/8/8/4K3");
	test_name = "white bishop e4 pinned on e-file by rook e7";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// white knight e4 pinned on e-file, no valid moves [PINNED]
	// valid squares: []
	game.set_board("4k3/4r3/8/8/4N3/8/8/4K3");
	test_name = "white knight e4 pinned on e-file, no valid moves";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// white rook e4 pinned on e-file, can only move along e-file [PINNED]
	// valid squares: ['e2', 'e3', 'e5', 'e6', 'e7']
	game.set_board("4k3/4r3/8/8/4R3/8/8/4K3");
	test_name = "white rook e4 pinned on e-file, can only move along e-file";
	correct_board = 0x0008080800080800ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// white queen e4 pinned on e-file by rook, moves along file only [PINNED]
	// valid squares: ['e2', 'e3', 'e5', 'e6', 'e7']
	game.set_board("4k3/4r3/8/8/4Q3/8/8/4K3");
	test_name = "white queen e4 pinned on e-file by rook, moves along file only";
	correct_board = 0x0008080800080800ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// white pawn e3 pinned on e-file, can push forward [PINNED]
	// valid squares: ['e4']
	game.set_board("4k3/4r3/8/8/8/4P3/8/4K3");
	test_name = "white pawn e3 pinned on e-file, can push forward";
	correct_board = 0x0000000008000000ULL;
	if (test_valid_moves(game.get_valid_moves(19), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// white pawn e2 pinned on e-file, can single and double push [PINNED]
	// valid squares: ['e3', 'e4']
	game.set_board("4k3/4r3/8/8/8/8/4P3/4K3");
	test_name = "white pawn e2 pinned on e-file, can single and double push";
	correct_board = 0x0000000008080000ULL;
	if (test_valid_moves(game.get_valid_moves(11), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// white pawn e7 pinned facing pinner direction, no moves [PINNED]
	// valid squares: []
	game.set_board("4K3/4P3/8/8/8/4r3/8/4k3");
	test_name = "white pawn e7 pinned facing pinner direction, no moves";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(51), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// white rook e4 pinned on e-file by rook e6, can move between or capture [PINNED]
	// valid squares: ['e2', 'e3', 'e5', 'e6']
	game.set_board("4k3/8/4r3/8/4R3/8/8/4K3");
	test_name = "white rook e4 pinned on e-file by rook e6, can move between or capture";
	correct_board = 0x0000080800080800ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// white bishop e4 pinned on e-file by queen e7 [PINNED]
	// valid squares: []
	game.set_board("4k3/4q3/8/8/4B3/8/8/4K3");
	test_name = "white bishop e4 pinned on e-file by queen e7";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// white queen e4 pinned on e-file by black queen e7 [PINNED]
	// valid squares: ['e2', 'e3', 'e5', 'e6', 'e7']
	game.set_board("4k3/4q3/8/8/4Q3/8/8/4K3");
	test_name = "white queen e4 pinned on e-file by black queen e7";
	correct_board = 0x0008080800080800ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// RANK PINS
	// white knight d5 pinned on rank 5 by black rook e5 [PINNED]
	// valid squares: []
	game.set_board("8/8/8/K2Nr3/8/8/8/4k3");
	test_name = "white knight d5 pinned on rank 5 by black rook e5";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(36), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// white rook d5 pinned on rank 5 by black rook e5, can move along rank [PINNED]
	// valid squares: ['b5', 'c5', 'e5']
	game.set_board("8/8/8/K2Rr3/8/8/8/4k3");
	test_name = "white rook d5 pinned on rank 5 by black rook e5, can move along rank";
	correct_board = 0x0000006800000000ULL;
	if (test_valid_moves(game.get_valid_moves(36), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// white bishop d5 pinned on rank 5 by black rook e5, no valid moves [PINNED]
	// valid squares: []
	game.set_board("8/8/8/K2Br3/8/8/8/4k3");
	test_name = "white bishop d5 pinned on rank 5 by black rook e5, no valid moves";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(36), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// white queen d5 pinned on rank 5 by black rook e5, can move along rank [PINNED]
	// valid squares: ['b5', 'c5', 'e5']
	game.set_board("8/8/8/K2Qr3/8/8/8/4k3");
	test_name = "white queen d5 pinned on rank 5 by black rook e5, can move along rank";
	correct_board = 0x0000006800000000ULL;
	if (test_valid_moves(game.get_valid_moves(36), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// DIAGONAL PINS
	// white rook b2 pinned diagonally by black bishop g7, no valid moves [PINNED]
	// valid squares: []
	game.set_board("8/6b1/8/8/8/8/1R6/K7");
	test_name = "white rook b2 pinned diagonally by black bishop g7, no valid moves";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(14), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// white queen b2 pinned diagonally by black bishop g7, can only move along diagonal [PINNED]
	// valid squares: ['c3', 'd4', 'e5', 'f6', 'g7']
	game.set_board("8/6b1/8/8/8/8/1Q6/K7");
	test_name = "white queen b2 pinned diagonally by black bishop g7, can only move along diagonal";
	correct_board = 0x0002040810200000ULL;
	if (test_valid_moves(game.get_valid_moves(14), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// white knight b2 pinned diagonally by black bishop g7, no valid moves [PINNED]
	// valid squares: []
	game.set_board("8/6b1/8/8/8/8/1N6/K7");
	test_name = "white knight b2 pinned diagonally by black bishop g7, no valid moves";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(14), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// white bishop b2 pinned on diagonal by black queen f6 [PINNED]
	// valid squares: ['c3', 'd4', 'e5', 'f6']
	game.set_board("8/8/5q2/8/8/8/1B6/K7");
	test_name = "white bishop b2 pinned on diagonal by black queen f6";
	correct_board = 0x0000040810200000ULL;
	if (test_valid_moves(game.get_valid_moves(14), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// white pawn c3 pinned diagonally by black bishop g7, cannot push or capture off diagonal [PINNED]
	// valid squares: []
	game.set_board("8/6b1/8/8/8/2P5/8/K7");
	test_name = "white pawn c3 pinned diagonally by black bishop g7, cannot push or capture off diagonal";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(21), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// white pawn d4 pinned diagonally by black bishop e5, CAN capture the pinning piece [PINNED]
	// valid squares: ['e5']
	game.set_board("8/8/8/4b3/3P4/8/8/K7");
	test_name = "white pawn d4 pinned diagonally by black bishop e5, CAN capture the pinning piece";
	correct_board = 0x0000000800000000ULL;
	if (test_valid_moves(game.get_valid_moves(28), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// ANTI-DIAGONAL PINS
	// white queen g2 pinned on anti-diagonal by black bishop c6 [PINNED]
	// valid squares: ['f3', 'e4', 'd5', 'c6']
	game.set_board("8/8/2b5/8/8/8/6Q1/7K");
	test_name = "white queen g2 pinned on anti-diagonal by black bishop c6";
	correct_board = 0x0000201008040000ULL;
	if (test_valid_moves(game.get_valid_moves(9), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// white rook g2 pinned on anti-diagonal by black bishop c6, no valid moves [PINNED]
	// valid squares: []
	game.set_board("8/8/2b5/8/8/8/6R1/7K");
	test_name = "white rook g2 pinned on anti-diagonal by black bishop c6, no valid moves";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(9), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// white knight g2 pinned on anti-diagonal by black bishop c6, no valid moves [PINNED]
	// valid squares: []
	game.set_board("8/8/2b5/8/8/8/6N1/7K");
	test_name = "white knight g2 pinned on anti-diagonal by black bishop c6, no valid moves";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(9), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// BLACK PIECES PINNED
	// black bishop e5 pinned on e-file by white rook e1 [PINNED]
	// valid squares: []
	game.set_board("4k3/8/8/4b3/8/8/8/4R3");
	test_name = "black bishop e5 pinned on e-file by white rook e1";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(35), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// black rook e5 pinned on e-file by white rook e1, can move along file [PINNED]
	// valid squares: ['e1', 'e2', 'e3', 'e4', 'e6', 'e7']
	game.set_board("4k3/8/8/4r3/8/8/8/4R3");
	test_name = "black rook e5 pinned on e-file by white rook e1, can move along file";
	correct_board = 0x0008080008080808ULL;
	if (test_valid_moves(game.get_valid_moves(35), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// black queen b7 pinned diagonally by white bishop g2 [PINNED]
	// valid squares: ['c6', 'd5', 'e4', 'f3', 'g2']
	game.set_board("k7/1q6/8/8/8/8/6B1/8");
	test_name = "black queen b7 pinned diagonally by white bishop g2";
	correct_board = 0x0000201008040200ULL;
	if (test_valid_moves(game.get_valid_moves(54), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// black knight g7 pinned on anti-diagonal by white queen b2 [PINNED]
	// valid squares: []
	game.set_board("7k/6n1/8/8/8/8/1Q6/8");
	test_name = "black knight g7 pinned on anti-diagonal by white queen b2";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(49), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// SPECIAL PAWN PINS
	// white pawn f2 pinned on f-file, can single and double push [PINNED]
	// valid squares: ['f3', 'f4']
	game.set_board("5k2/5r2/8/8/8/8/5P2/5K2");
	test_name = "white pawn f2 pinned on f-file, can single and double push";
	correct_board = 0x0000000004040000ULL;
	if (test_valid_moves(game.get_valid_moves(10), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// white pawn d5 pinned horizontally by black queen e5 [PINNED]
	// valid squares: []
	game.set_board("8/8/8/K2Pq3/8/8/8/8");
	test_name = "white pawn d5 pinned horizontally by black queen e5";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(36), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 1. King x-ray back check evasion (Rook check)
	// Valid squares: d5, f5, d4, f4, d3, f3
	game.set_board("8/8/8/8/4k3/8/8/4R3");
	test_name = "1. King x-ray back check evasion (Rook check)";
	correct_board = 0x0000001414140000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 2. King x-ray back check evasion (Bishop check)
	// Valid squares: d5, e5, d4, f4, e3, f3
	game.set_board("8/8/8/8/4k3/8/8/1B6");
	test_name = "2. King x-ray back check evasion (Bishop check)";
	correct_board = 0x00000018140C0000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;
	// 3. Pinned pawn cannot block a check (Fixed FEN)
	// Setup: Black King h7, Black pawn g7 pinned by White Rook a7. White Q c2 checks h7.
	// Valid squares: [] (g7 wants to go to g6 to block, but is pinned horizontally)
	game.set_board("8/R5pk/8/8/8/8/2Q5/8 b - - 0 1");
	test_name = "3. Pinned pawn cannot block a check";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(49), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 5. Pinned piece cannot capture pinner to resolve check (Fixed FEN)
	// Setup: Black King e8, Black Knight e5 pinned by White Rook e1. White Q d7 checks e8.
	// Valid squares: [] (Knight cannot capture Q on d7 because it breaks the e-file pin)
	game.set_board("4k3/3Q4/8/4n3/8/8/8/4R2K b - - 0 1");
	test_name = "5. Pinned piece cannot capture pinner to resolve check";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(35), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 6. Pawn double push blocked by intermediate enemy piece
	// Valid squares: []
	game.set_board("8/8/8/8/8/p7/P7/8");
	test_name = "6. Pawn double push blocked by intermediate enemy piece";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(15), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 7. Knight edge wrap (A-file isolation)
	// Valid squares: b6, c5, c3, b2
	game.set_board("8/8/8/8/N7/8/8/8");
	test_name = "7. Knight edge wrap (A-file isolation)";
	correct_board = 0x0000402000204000ULL;
	if (test_valid_moves(game.get_valid_moves(31), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 8. Knight edge wrap (H-file isolation)
	// Valid squares: g6, f5, f3, g2
	game.set_board("8/8/8/8/7N/8/8/8");
	test_name = "8. Knight edge wrap (H-file isolation)";
	correct_board = 0x0000020400040200ULL;
	if (test_valid_moves(game.get_valid_moves(24), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 9. Pinned Rook CAN counter-attack its pinner
	// Valid squares: e3, e1
	game.set_board("8/8/8/8/4k3/8/4r3/4R3");
	test_name = "9. Pinned Rook CAN counter-attack its pinner";
	correct_board = 0x0000000000080008ULL;
	if (test_valid_moves(game.get_valid_moves(11), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 10. Pinned Bishop CANNOT capture pinner on a straight file
	// Valid squares: []
	game.set_board("8/8/8/8/4k3/8/4b3/4R3");
	test_name = "10. Pinned Bishop CANNOT capture pinner on a straight file";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(11), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 11. Pawn diagonal capture edge wrap (A-file prevents H capture)
	// Valid squares: a3, a4
	game.set_board("8/8/8/8/8/7p/P7/8");
	test_name = "11. Pawn diagonal capture edge wrap (A-file prevents H capture)";
	correct_board = 0x0000000080800000ULL;
	if (test_valid_moves(game.get_valid_moves(15), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 12. Pawn diagonal capture edge wrap (H-file prevents A capture)
	// Valid squares: h3, h4
	game.set_board("8/8/8/8/8/p7/7P/8");
	test_name = "12. Pawn diagonal capture edge wrap (H-file prevents A capture)";
	correct_board = 0x0000000001010000ULL;
	if (test_valid_moves(game.get_valid_moves(8), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 13. Pinned pawn capturing along its own pinning diagonal (Fixed FEN)
	// Setup: White King e1, White Pawn d2, Black Bishop c3.
	// Valid squares: c3 (White pawn captures the piece pinning it)
	game.set_board("8/8/8/8/8/2b5/3P4/4K3 w - - 0 1");
	test_name = "13. Pinned pawn capturing along its own pinning diagonal";
	correct_board = 0x0000000000200000ULL; // c3
	if (test_valid_moves(game.get_valid_moves(12), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 14. King capturing protected piece (overlap logic) (Fixed Expected Board)
	// Valid squares: d5, e5, f5 (d4 and f4 are attacked by the White Pawn on e3!)
	game.set_board("8/8/8/8/4k3/4P3/4K3/8 b - - 0 1");
	test_name = "14. King capturing protected piece (overlap logic)";
	correct_board = 0x0000001C00000000ULL; // Removed the incorrect '14' from rank 4
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 15. Rook edge wrap isolation
	// Valid squares: a1..a8 except a4, plus b4..h4
	game.set_board("8/8/8/8/R7/8/8/8");
	test_name = "15. Rook edge wrap isolation";
	correct_board = 0x808080807F808080ULL;
	if (test_valid_moves(game.get_valid_moves(31), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 16. Bishop edge wrap isolation
	// Valid squares: b5, c6, d7, e8, b3, c2, d1
	game.set_board("8/8/8/8/B7/8/8/8");
	test_name = "16. Bishop edge wrap isolation";
	correct_board = 0x0810204000402010ULL;
	if (test_valid_moves(game.get_valid_moves(31), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 17. Double check blocking attempt with Bishop
	// Valid squares: []
	game.set_board("4k3/8/8/4r3/8/3n4/4B3/4K3");
	test_name = "17. Double check blocking attempt with Bishop";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(11), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 18. King moving into backward pawn attack zone
	// Valid squares: d5, e5, f5, d4, f4, e3
	game.set_board("8/8/8/8/4k3/8/4P3/8");
	test_name = "18. King moving into backward pawn attack zone";
	correct_board = 0x0000001C14080000ULL;
	if (test_valid_moves(game.get_valid_moves(27), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 19. Pinned Queen moving AWAY from King on diagonal
	// Valid squares: d3, b1 (capture)
	game.set_board("8/8/8/8/4k3/8/2q5/1B6");
	test_name = "19. Pinned Queen moving AWAY from King on diagonal";
	correct_board = 0x0000000000100040ULL;
	if (test_valid_moves(game.get_valid_moves(13), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// 20. Pawn double push blocked by FRIENDLY piece
	// Valid squares: []
	game.set_board("8/8/8/8/8/P7/P7/8");
	test_name = "20. Pawn double push blocked by FRIENDLY piece";
	correct_board = 0x0000000000000000ULL;
	if (test_valid_moves(game.get_valid_moves(15), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game = chess();
	test_name = "check if pawn can move one space";
	bool correct_result = true;
	if (test_check_moves(game.check_move(8, 16), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	game.set_board("4k3/3q4/4QN2/8/8/8/8/7K b - - 0 1");
	test_name = "4. Double check - Queen cannot block or capture";
	correct_result = false;
	if (test_check_moves(game.check_move(52, 55), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	std::cout << "Passed: " << passed << "/" << total << std::endl;
	std::cout << std::endl;
	std::cout << "Failed:\n" << failed_tests << std::endl;

	return 0;
}
