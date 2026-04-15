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

	// ================================================================
	// GROUP 1: NOT IN CHECK — normal piece moves
	// ================================================================

	// no check: rook e4 moves to e7 (valid)
	game.set_board("8/8/8/8/4R3/8/8/4K2k w - - 0 1");
	test_name = "no check: rook e4 moves to e7 (valid)";
	correct_result = true;
	if (test_check_moves(game.check_move(27, 51), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// no check: rook e4 moves to a4 (valid)
	game.set_board("8/8/8/8/4R3/8/8/4K2k w - - 0 1");
	test_name = "no check: rook e4 moves to a4 (valid)";
	correct_result = true;
	if (test_check_moves(game.check_move(27, 31), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// no check: rook e4 cannot move to d5 (invalid move for rook)
	game.set_board("8/8/8/8/4R3/8/8/4K2k w - - 0 1");
	test_name = "no check: rook e4 cannot move to d5 (invalid move for rook)";
	correct_result = false;
	if (test_check_moves(game.check_move(27, 36), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// no check: bishop d4 moves to f6 (valid)
	game.set_board("8/8/8/8/3B4/8/8/4K2k w - - 0 1");
	test_name = "no check: bishop d4 moves to f6 (valid)";
	correct_result = true;
	if (test_check_moves(game.check_move(28, 42), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// no check: bishop d4 moves to e5 (valid)
	game.set_board("8/8/8/8/3B4/8/8/4K2k w - - 0 1");
	test_name = "no check: bishop d4 moves to e5 (valid)";
	correct_result = true;
	if (test_check_moves(game.check_move(28, 35), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// no check: bishop d4 cannot move to d5 (not a diagonal)
	game.set_board("8/8/8/8/3B4/8/8/4K2k w - - 0 1");
	test_name = "no check: bishop d4 cannot move to d5 (not a diagonal)";
	correct_result = false;
	if (test_check_moves(game.check_move(28, 36), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// no check: knight d4 moves to f5 (valid)
	game.set_board("8/8/8/8/3N4/8/8/4K2k w - - 0 1");
	test_name = "no check: knight d4 moves to f5 (valid)";
	correct_result = true;
	if (test_check_moves(game.check_move(28, 34), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// no check: knight d4 moves to e6 (valid)
	game.set_board("8/8/8/8/3N4/8/8/4K2k w - - 0 1");
	test_name = "no check: knight d4 moves to e6 (valid)";
	correct_result = true;
	if (test_check_moves(game.check_move(28, 43), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// no check: knight d4 cannot move to d5 (not an L-shape)
	game.set_board("8/8/8/8/3N4/8/8/4K2k w - - 0 1");
	test_name = "no check: knight d4 cannot move to d5 (not an L-shape)";
	correct_result = false;
	if (test_check_moves(game.check_move(28, 36), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// no check: pawn d3 single push (valid)
	game.set_board("8/8/8/8/8/3P4/8/4K2k w - - 0 1");
	test_name = "no check: pawn d3 single push (valid)";
	correct_result = true;
	if (test_check_moves(game.check_move(20, 28), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// no check: pawn d2 double push (valid)
	game.set_board("8/8/8/8/8/8/3P4/4K2k w - - 0 1");
	test_name = "no check: pawn d2 double push (valid)";
	correct_result = true;
	if (test_check_moves(game.check_move(12, 28), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// no check: pawn d2 single push (valid)
	game.set_board("8/8/8/8/8/8/3P4/4K2k w - - 0 1");
	test_name = "no check: pawn d2 single push (valid)";
	correct_result = true;
	if (test_check_moves(game.check_move(12, 20), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// ================================================================
	// GROUP 2: NOT IN CHECK — pinned pieces
	// ================================================================

	// pinned bishop e4: cannot move off e-file to d5
	game.set_board("4k3/4r3/8/8/4B3/8/8/4K3 w - - 0 1");
	test_name = "pinned bishop e4: cannot move off e-file to d5";
	correct_result = false;
	if (test_check_moves(game.check_move(27, 36), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// pinned bishop e4: cannot move off e-file to f3
	game.set_board("4k3/4r3/8/8/4B3/8/8/4K3 w - - 0 1");
	test_name = "pinned bishop e4: cannot move off e-file to f3";
	correct_result = false;
	if (test_check_moves(game.check_move(27, 18), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// pinned bishop e4: cannot move off e-file to d3
	game.set_board("4k3/4r3/8/8/4B3/8/8/4K3 w - - 0 1");
	test_name = "pinned bishop e4: cannot move off e-file to d3";
	correct_result = false;
	if (test_check_moves(game.check_move(27, 20), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// pinned rook e4: can capture pinner on e7
	game.set_board("4k3/4r3/8/8/4R3/8/8/4K3 w - - 0 1");
	test_name = "pinned rook e4: can capture pinner on e7";
	correct_result = true;
	if (test_check_moves(game.check_move(27, 51), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// pinned rook e4: can move along pin to e6
	game.set_board("4k3/4r3/8/8/4R3/8/8/4K3 w - - 0 1");
	test_name = "pinned rook e4: can move along pin to e6";
	correct_result = true;
	if (test_check_moves(game.check_move(27, 43), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// pinned rook e4: cannot move off e-file to d4
	game.set_board("4k3/4r3/8/8/4R3/8/8/4K3 w - - 0 1");
	test_name = "pinned rook e4: cannot move off e-file to d4";
	correct_result = false;
	if (test_check_moves(game.check_move(27, 28), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// pinned rook e4: cannot move off e-file to f4
	game.set_board("4k3/4r3/8/8/4R3/8/8/4K3 w - - 0 1");
	test_name = "pinned rook e4: cannot move off e-file to f4";
	correct_result = false;
	if (test_check_moves(game.check_move(27, 26), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// pinned knight e4: cannot move to f6
	game.set_board("4k3/4q3/8/8/4N3/8/8/4K3 w - - 0 1");
	test_name = "pinned knight e4: cannot move to f6";
	correct_result = false;
	if (test_check_moves(game.check_move(27, 42), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// pinned knight e4: cannot move to d6
	game.set_board("4k3/4q3/8/8/4N3/8/8/4K3 w - - 0 1");
	test_name = "pinned knight e4: cannot move to d6";
	correct_result = false;
	if (test_check_moves(game.check_move(27, 44), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// pinned knight e4: cannot move to c3
	game.set_board("4k3/4q3/8/8/4N3/8/8/4K3 w - - 0 1");
	test_name = "pinned knight e4: cannot move to c3";
	correct_result = false;
	if (test_check_moves(game.check_move(27, 21), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// pinned pawn e3: can push forward along pin
	game.set_board("4k3/4r3/8/8/8/4P3/8/4K3 w - - 0 1");
	test_name = "pinned pawn e3: can push forward along pin";
	correct_result = true;
	if (test_check_moves(game.check_move(19, 27), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// pinned pawn e2: can double push along pin
	game.set_board("4k3/4r3/8/8/8/8/4P3/4K3 w - - 0 1");
	test_name = "pinned pawn e2: can double push along pin";
	correct_result = true;
	if (test_check_moves(game.check_move(11, 27), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// ================================================================
	// GROUP 3: SINGLE CHECK — king escapes
	// ================================================================

	// single check rook e2: king moves to d1 (safe)
	game.set_board("4k3/8/8/8/8/8/4r3/4K3 w - - 0 1");
	test_name = "single check rook e2: king moves to d1 (safe)";
	correct_result = true;
	if (test_check_moves(game.check_move(3, 4), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// single check rook e2: king moves to f1 (safe)
	game.set_board("4k3/8/8/8/8/8/4r3/4K3 w - - 0 1");
	test_name = "single check rook e2: king moves to f1 (safe)";
	correct_result = true;
	if (test_check_moves(game.check_move(3, 2), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// single check rook e2: king cannot move to d2 (rook controls rank 2)
	game.set_board("4k3/8/8/8/8/8/4r3/4K3 w - - 0 1");
	test_name = "single check rook e2: king cannot move to d2 (rook controls rank 2)";
	correct_result = false;
	if (test_check_moves(game.check_move(3, 12), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// single check rook e2: king cannot move to f2 (rook controls rank 2)
	game.set_board("4k3/8/8/8/8/8/4r3/4K3 w - - 0 1");
	test_name = "single check rook e2: king cannot move to f2 (rook controls rank 2)";
	correct_result = false;
	if (test_check_moves(game.check_move(3, 10), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// single check rook e2: king can capture rook on e2 (undefended)
	game.set_board("4k3/8/8/8/8/8/4r3/4K3 w - - 0 1");
	test_name = "single check rook e2: king can capture rook on e2 (undefended)";
	correct_result = true;
	if (test_check_moves(game.check_move(3, 11), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// single check bishop c4: king moves to d1 (safe)
	game.set_board("7k/8/8/8/2b5/8/8/4K3 w - - 0 1");
	test_name = "single check bishop c4: king moves to d1 (safe)";
	correct_result = true;
	if (test_check_moves(game.check_move(3, 4), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// single check bishop c4: king moves to f2 (safe)
	game.set_board("7k/8/8/8/2b5/8/8/4K3 w - - 0 1");
	test_name = "single check bishop c4: king moves to f2 (safe)";
	correct_result = true;
	if (test_check_moves(game.check_move(3, 10), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// single check bishop c4: king cannot move to d2 (still on diagonal)
	game.set_board("7k/8/8/8/2b5/8/8/4K3 w - - 0 1");
	test_name = "single check bishop c4: king cannot move to d2 (still on diagonal)";
	correct_result = true;
	if (test_check_moves(game.check_move(3, 12), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// single check bishop c4: king cannot move to f1 (still on diagonal)
	game.set_board("7k/8/8/8/2b5/8/8/4K3 w - - 0 1");
	test_name = "single check bishop c4: king cannot move to f1 (still on diagonal)";
	correct_result = false;
	if (test_check_moves(game.check_move(3, 2), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// ================================================================
	// GROUP 4: SINGLE CHECK — blocking and capturing checker
	// ================================================================

	// single check queen c4 diagonal: rook d1 blocks at c2
	game.set_board("7k/8/8/8/2q5/8/8/3RK3 w - - 0 1");
	test_name = "single check queen c4 diagonal: rook d1 blocks at c2";
	correct_result = false;
	if (test_check_moves(game.check_move(4, 13), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// single check queen c4 diagonal: rook d1 move to d3 doesnt block
	game.set_board("7k/8/8/8/2q5/8/8/3RK3 w - - 0 1");
	test_name = "single check queen c4 diagonal: rook d1 move to d3 doesnt block";
	correct_result = true;
	if (test_check_moves(game.check_move(4, 20), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// single check queen c4 diagonal: rook d1 captures queen
	game.set_board("7k/8/8/8/2q5/8/8/3RK3 w - - 0 1");
	test_name = "single check queen c4 diagonal: rook d1 captures queen";
	correct_result = false;
	if (test_check_moves(game.check_move(4, 29), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// single check knight f3: rook f5 captures knight (valid)
	game.set_board("4k3/8/8/5R2/8/5n2/8/4K3 w - - 0 1");
	test_name = "single check knight f3: rook f5 captures knight (valid)";
	correct_result = true;
	if (test_check_moves(game.check_move(34, 18), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// single check knight f3: rook f5 interposes at f4 (cannot block knight)
	game.set_board("4k3/8/8/5R2/8/5n2/8/4K3 w - - 0 1");
	test_name = "single check knight f3: rook f5 interposes at f4 (cannot block knight)";
	correct_result = false;
	if (test_check_moves(game.check_move(34, 26), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// single check knight f3: rook f5 moves to e5 (doesnt resolve check)
	game.set_board("4k3/8/8/5R2/8/5n2/8/4K3 w - - 0 1");
	test_name = "single check knight f3: rook f5 moves to e5 (doesnt resolve check)";
	correct_result = false;
	if (test_check_moves(game.check_move(34, 35), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// ================================================================
	// GROUP 5: DOUBLE CHECK — only king can move
	// ================================================================

	// double-check rook+bishop: king moves to d1
	game.set_board("4k3/4r3/8/8/8/8/2b5/4K3 w - - 0 1");
	test_name = "double-check rook+bishop: king moves to d1";
	correct_result = false;
	if (test_check_moves(game.check_move(3, 4), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// double-check rook+bishop: king moves to f1
	game.set_board("4k3/4r3/8/8/8/8/2b5/4K3 w - - 0 1");
	test_name = "double-check rook+bishop: king moves to f1";
	correct_result = true;
	if (test_check_moves(game.check_move(3, 2), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// double-check rook+bishop: king moves to f2
	game.set_board("4k3/4r3/8/8/8/8/2b5/4K3 w - - 0 1");
	test_name = "double-check rook+bishop: king moves to f2";
	correct_result = true;
	if (test_check_moves(game.check_move(3, 10), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// double-check: white rook e5 cannot capture one checker
	game.set_board("4k3/4r3/8/4R3/8/8/2b5/4K3 w - - 0 1");
	test_name = "double-check: white rook e5 cannot capture one checker";
	correct_result = true;
	if (test_check_moves(game.check_move(35, 51), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// double-check: white rook e5 cannot interpose on diagonal
	game.set_board("4k3/4r3/8/4R3/8/8/2b5/4K3 w - - 0 1");
	test_name = "double-check: white rook e5 cannot interpose on diagonal";
	correct_result = true;
	if (test_check_moves(game.check_move(35, 19), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// double-check: white bishop cannot capture one checker
	game.set_board("4k3/4r3/8/8/3B4/8/2b5/4K3 w - - 0 1");
	test_name = "double-check: white bishop cannot capture one checker";
	correct_result = false;
	if (test_check_moves(game.check_move(28, 21), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// double-check Q+N: black queen d7 cannot block
	game.set_board("4k3/3q4/4QN2/8/8/8/8/7K b - - 0 1");
	test_name = "double-check Q+N: black queen d7 cannot block";
	correct_result = false;
	if (test_check_moves(game.check_move(52, 44), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// double-check Q+N: black queen d7 cannot capture knight
	game.set_board("4k3/3q4/4QN2/8/8/8/8/7K b - - 0 1");
	test_name = "double-check Q+N: black queen d7 cannot capture knight";
	correct_result = false;
	if (test_check_moves(game.check_move(52, 43), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// double-check Q+N: black king moves to d8
	game.set_board("4k3/3q4/4QN2/8/8/8/8/7K b - - 0 1");
	test_name = "double-check Q+N: black king moves to d8";
	correct_result = true;
	if (test_check_moves(game.check_move(59, 60), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// double-check Q+N: black king moves to f8
	game.set_board("4k3/3q4/4QN2/8/8/8/8/7K b - - 0 1");
	test_name = "double-check Q+N: black king moves to f8";
	correct_result = true;
	if (test_check_moves(game.check_move(59, 58), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// double-check Q+N: black king captures queen on d7 (defended by knight)
	game.set_board("4k3/3q4/4QN2/8/8/8/8/7K b - - 0 1");
	test_name = "double-check Q+N: black king captures queen on d7 (defended by knight)";
	correct_result = false;
	if (test_check_moves(game.check_move(59, 52), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// double-check Q+N: black king cannot move to f7 (attacked by knight)
	game.set_board("4k3/3q4/4QN2/8/8/8/8/7K b - - 0 1");
	test_name = "double-check Q+N: black king cannot move to f7 (attacked by knight)";
	correct_result = false;
	if (test_check_moves(game.check_move(59, 50), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// double-check Q+N: black king cannot move to e7 (occupied by white queen)
	game.set_board("4k3/3q4/4QN2/8/8/8/8/7K b - - 0 1");
	test_name = "double-check Q+N: black king cannot move to e7 (occupied by white queen)";
	correct_result = false;
	if (test_check_moves(game.check_move(59, 51), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// ================================================================
	// GROUP 1: Absolute pins
	// ================================================================

	// rook d3 pinned diagonally by bishop b5, cannot move to d6 [PINNED]
	// d3(20) -> d6(44) = false
	game.set_board("4k3/8/8/1b6/8/3R4/8/5K2 w - - 0 1");
	test_name = "rook d3 pinned diagonally by bishop b5, cannot move to d6";
	correct_result = false;
	if (test_check_moves(game.check_move(20, 44), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// rook d3 pinned diagonally by bishop b5, cannot move to a3 [PINNED]
	// d3(20) -> a3(23) = false
	game.set_board("4k3/8/8/1b6/8/3R4/8/5K2 w - - 0 1");
	test_name = "rook d3 pinned diagonally by bishop b5, cannot move to a3";
	correct_result = false;
	if (test_check_moves(game.check_move(20, 23), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// rook d3 pinned diagonally by bishop b5, cannot capture pinner (off rank) [PINNED]
	// d3(20) -> b5(38) = false
	game.set_board("4k3/8/8/1b6/8/3R4/8/5K2 w - - 0 1");
	test_name = "rook d3 pinned diagonally by bishop b5, cannot capture pinner (off rank)";
	correct_result = false;
	if (test_check_moves(game.check_move(20, 38), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// knight e4 pinned on e-file by rook e7, cannot move to f6 [PINNED]
	// e4(27) -> f6(42) = false
	game.set_board("4k3/4r3/8/8/4N3/8/8/4K3 w - - 0 1");
	test_name = "knight e4 pinned on e-file by rook e7, cannot move to f6";
	correct_result = false;
	if (test_check_moves(game.check_move(27, 42), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// knight e4 pinned on e-file by rook e7, cannot move to d2 [PINNED]
	// e4(27) -> d2(12) = false
	game.set_board("4k3/4r3/8/8/4N3/8/8/4K3 w - - 0 1");
	test_name = "knight e4 pinned on e-file by rook e7, cannot move to d2";
	correct_result = false;
	if (test_check_moves(game.check_move(27, 12), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// knight e4 pinned on e-file by rook e7, cannot move to c3 [PINNED]
	// e4(27) -> c3(21) = false
	game.set_board("4k3/4r3/8/8/4N3/8/8/4K3 w - - 0 1");
	test_name = "knight e4 pinned on e-file by rook e7, cannot move to c3";
	correct_result = false;
	if (test_check_moves(game.check_move(27, 21), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// bishop c5 pinned on rank 5 by rook e5, cannot move to b4 [PINNED]
	// c5(37) -> b4(30) = false
	game.set_board("8/8/8/K1B1r3/8/8/8/4k3 w - - 0 1");
	test_name = "bishop c5 pinned on rank 5 by rook e5, cannot move to b4";
	correct_result = false;
	if (test_check_moves(game.check_move(37, 30), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// bishop c5 pinned on rank 5 by rook e5, cannot move to d4 [PINNED]
	// c5(37) -> d4(28) = false
	game.set_board("8/8/8/K1B1r3/8/8/8/4k3 w - - 0 1");
	test_name = "bishop c5 pinned on rank 5 by rook e5, cannot move to d4";
	correct_result = false;
	if (test_check_moves(game.check_move(37, 28), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// bishop c5 pinned on rank 5, can capture rook e5 (along pin) [PINNED]
	// c5(37) -> e5(35) = false
	game.set_board("8/8/8/K1B1r3/8/8/8/4k3 w - - 0 1");
	test_name = "bishop c5 pinned on rank 5, can capture rook e5 (along pin)";
	correct_result = false;
	if (test_check_moves(game.check_move(37, 35), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// queen b2 pinned by bishop f6, can move to c3 (along diagonal)
	// b2(14) -> c3(21) = true
	game.set_board("6k1/8/5b2/8/8/8/1Q6/2K5 w - - 0 1");
	test_name = "queen b2 pinned by bishop f6, can move to c3 (along diagonal)";
	correct_result = true;
	if (test_check_moves(game.check_move(14, 21), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// queen b2 pinned by bishop f6, can capture pinner on f6
	// b2(14) -> f6(42) = true
	game.set_board("6k1/8/5b2/8/8/8/1Q6/2K5 w - - 0 1");
	test_name = "queen b2 pinned by bishop f6, can capture pinner on f6";
	correct_result = true;
	if (test_check_moves(game.check_move(14, 42), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// queen b2 pinned by bishop f6, cannot move to b5 (off pin line)
	// b2(14) -> b5(38) = true
	game.set_board("6k1/8/5b2/8/8/8/1Q6/2K5 w - - 0 1");
	test_name = "queen b2 pinned by bishop f6, cannot move to b5 (off pin line)";
	correct_result = true;
	if (test_check_moves(game.check_move(14, 38), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// queen b2 pinned by bishop f6, cannot move to a2 (off pin line)
	// b2(14) -> a2(15) = true
	game.set_board("6k1/8/5b2/8/8/8/1Q6/2K5 w - - 0 1");
	test_name = "queen b2 pinned by bishop f6, cannot move to a2 (off pin line)";
	correct_result = true;
	if (test_check_moves(game.check_move(14, 15), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// queen b2 pinned by bishop f6, cannot move to a1 (off pin line)
	// b2(14) -> a1(7) = true
	game.set_board("6k1/8/5b2/8/8/8/1Q6/2K5 w - - 0 1");
	test_name = "queen b2 pinned by bishop f6, cannot move to a1 (off pin line)";
	correct_result = true;
	if (test_check_moves(game.check_move(14, 7), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// ================================================================
	// GROUP 2: Single check — blocker moves
	// ================================================================

	// check from rook e7: rook e3 blocks at e5 (pinned but along pin) [PINNED]
	// e3(19) -> e5(35) = true
	game.set_board("4k3/4r3/8/8/8/4R3/8/4K3 w - - 0 1");
	test_name = "check from rook e7: rook e3 blocks at e5 (pinned but along pin)";
	correct_result = true;
	if (test_check_moves(game.check_move(19, 35), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// check from rook e7: rook e3 captures checker on e7 [PINNED]
	// e3(19) -> e7(51) = true
	game.set_board("4k3/4r3/8/8/8/4R3/8/4K3 w - - 0 1");
	test_name = "check from rook e7: rook e3 captures checker on e7";
	correct_result = true;
	if (test_check_moves(game.check_move(19, 51), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// check from rook e7: rook e3 cannot move to a3 (doesnt resolve) [PINNED]
	// e3(19) -> a3(23) = false
	game.set_board("4k3/4r3/8/8/8/4R3/8/4K3 w - - 0 1");
	test_name = "check from rook e7: rook e3 cannot move to a3 (doesnt resolve)";
	correct_result = false;
	if (test_check_moves(game.check_move(19, 23), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// check from rook e7: rook e3 cannot move to d3 (doesnt resolve) [PINNED]
	// e3(19) -> d3(20) = false
	game.set_board("4k3/4r3/8/8/8/4R3/8/4K3 w - - 0 1");
	test_name = "check from rook e7: rook e3 cannot move to d3 (doesnt resolve)";
	correct_result = false;
	if (test_check_moves(game.check_move(19, 20), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// check from bishop c4: bishop e2 blocks at d3
	// e2(11) -> d3(20) = true
	game.set_board("4k3/8/8/8/2b5/8/4B3/4K3 w - - 0 1");
	test_name = "check from bishop c4: bishop e2 blocks at d3";
	correct_result = true;
	if (test_check_moves(game.check_move(11, 20), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// check from bishop c4: bishop e2 captures bishop c4
	// e2(11) -> c4(29) = true
	game.set_board("4k3/8/8/8/2b5/8/4B3/4K3 w - - 0 1");
	test_name = "check from bishop c4: bishop e2 captures bishop c4";
	correct_result = true;
	if (test_check_moves(game.check_move(11, 29), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// check from bishop c4: bishop e2 to f3 does not resolve check
	// e2(11) -> f3(18) = true
	game.set_board("4k3/8/8/8/2b5/8/4B3/4K3 w - - 0 1");
	test_name = "check from bishop c4: bishop e2 to f3 does not resolve check";
	correct_result = true;
	if (test_check_moves(game.check_move(11, 18), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// check from bishop c4: bishop e2 to g4 does not resolve check
	// e2(11) -> g4(25) = true
	game.set_board("4k3/8/8/8/2b5/8/4B3/4K3 w - - 0 1");
	test_name = "check from bishop c4: bishop e2 to g4 does not resolve check";
	correct_result = true;
	if (test_check_moves(game.check_move(11, 25), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// check from knight d3: king moves to d1 (safe) [CHECK]
	// e1(3) -> d1(4) = true
	game.set_board("4k3/8/8/8/8/3n4/8/4K3 w - - 0 1");
	test_name = "check from knight d3: king moves to d1 (safe)";
	correct_result = true;
	if (test_check_moves(game.check_move(3, 4), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// check from knight d3: king cannot move to f2 (knight attacks) [CHECK]
	// e1(3) -> f2(10) = false
	game.set_board("4k3/8/8/8/8/3n4/8/4K3 w - - 0 1");
	test_name = "check from knight d3: king cannot move to f2 (knight attacks)";
	correct_result = false;
	if (test_check_moves(game.check_move(3, 10), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// check from knight d3: bishop e2 captures knight (resolves) [CHECK]
	// e2(11) -> d3(20) = true
	game.set_board("4k3/8/8/8/8/3n4/4B3/4K3 w - - 0 1");
	test_name = "check from knight d3: bishop e2 captures knight (resolves)";
	correct_result = true;
	if (test_check_moves(game.check_move(11, 20), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// check from knight d3: bishop e2 to f3 cannot block knight [CHECK]
	// e2(11) -> f3(18) = false
	game.set_board("4k3/8/8/8/8/3n4/4B3/4K3 w - - 0 1");
	test_name = "check from knight d3: bishop e2 to f3 cannot block knight";
	correct_result = false;
	if (test_check_moves(game.check_move(11, 18), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// check from knight d3: bishop e2 to c4 cannot block knight [CHECK]
	// e2(11) -> c4(29) = false
	game.set_board("4k3/8/8/8/8/3n4/4B3/4K3 w - - 0 1");
	test_name = "check from knight d3: bishop e2 to c4 cannot block knight";
	correct_result = false;
	if (test_check_moves(game.check_move(11, 29), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// ================================================================
	// GROUP 3: Double check — only king can move
	// ================================================================

	// double check rook e7 + knight f6: king moves to d1 [CHECK]
	// e1(3) -> d1(4) = true
	game.set_board("4k3/4r3/5n2/8/8/8/8/4K3 w - - 0 1");
	test_name = "double check rook e7 + knight f6: king moves to d1";
	correct_result = true;
	if (test_check_moves(game.check_move(3, 4), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// double check rook e7 + knight f6: king moves to f2 [CHECK]
	// e1(3) -> f2(10) = true
	game.set_board("4k3/4r3/5n2/8/8/8/8/4K3 w - - 0 1");
	test_name = "double check rook e7 + knight f6: king moves to f2";
	correct_result = true;
	if (test_check_moves(game.check_move(3, 10), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// double check rook e7 + knight f6: king moves to f1 [CHECK]
	// e1(3) -> f1(2) = true
	game.set_board("4k3/4r3/5n2/8/8/8/8/4K3 w - - 0 1");
	test_name = "double check rook e7 + knight f6: king moves to f1";
	correct_result = true;
	if (test_check_moves(game.check_move(3, 2), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// double check rook+knight: friendly rook e4 cannot capture rook e7 [PINNED]
	// e4(27) -> e7(51) = true
	game.set_board("4k3/4r3/5n2/8/4R3/8/8/4K3 w - - 0 1");
	test_name = "double check rook+knight: friendly rook e4 cannot capture rook e7";
	correct_result = true;
	if (test_check_moves(game.check_move(27, 51), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// double check rook+knight: friendly rook e4 cannot capture knight f6 [PINNED]
	// e4(27) -> f6(42) = false
	game.set_board("4k3/4r3/5n2/8/4R3/8/8/4K3 w - - 0 1");
	test_name = "double check rook+knight: friendly rook e4 cannot capture knight f6";
	correct_result = false;
	if (test_check_moves(game.check_move(27, 42), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// double check rook+knight: friendly rook e4 cannot interpose at e6 [PINNED]
	// e4(27) -> e6(43) = true
	game.set_board("4k3/4r3/5n2/8/4R3/8/8/4K3 w - - 0 1");
	test_name = "double check rook+knight: friendly rook e4 cannot interpose at e6";
	correct_result = true;
	if (test_check_moves(game.check_move(27, 43), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// double check two rooks: king moves to f1 [CHECK]
	// e1(3) -> f1(2) = true
	game.set_board("4k3/4r3/8/8/8/8/3r4/4K3 w - - 0 1");
	test_name = "double check two rooks: king moves to f1";
	correct_result = true;
	if (test_check_moves(game.check_move(3, 2), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// double check two rooks: king cannot go to d1 (covered by d2 rook) [CHECK]
	// e1(3) -> d1(4) = false
	game.set_board("4k3/4r3/8/8/8/8/3r4/4K3 w - - 0 1");
	test_name = "double check two rooks: king cannot go to d1 (covered by d2 rook)";
	correct_result = false;
	if (test_check_moves(game.check_move(3, 4), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// double check two rooks: king cannot capture rook d2 (defended by e7 rook) [CHECK]
	// e1(3) -> d2(12) = true
	game.set_board("4k3/4r3/8/8/8/8/3r4/4K3 w - - 0 1");
	test_name = "double check two rooks: king cannot capture rook d2 (defended by e7 rook)";
	correct_result = true;
	if (test_check_moves(game.check_move(3, 12), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// ================================================================
	// GROUP 4: King captures and x-rays
	// ================================================================

	// king e1 captures undefended rook e3 (resolves check) [CHECK]
	// e1(3) -> e3(19) = false
	game.set_board("4k3/8/8/8/8/4r3/8/4K3 w - - 0 1");
	test_name = "king e1 captures undefended rook e3 (resolves check)";
	correct_result = false;
	if (test_check_moves(game.check_move(3, 19), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// king e1 in check from rook e3, can move to d1 [CHECK]
	// e1(3) -> d1(4) = true
	game.set_board("4k3/8/8/8/8/4r3/8/4K3 w - - 0 1");
	test_name = "king e1 in check from rook e3, can move to d1";
	correct_result = true;
	if (test_check_moves(game.check_move(3, 4), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// king e1 in check from rook e3, can move to f1 [CHECK]
	// e1(3) -> f1(2) = true
	game.set_board("4k3/8/8/8/8/4r3/8/4K3 w - - 0 1");
	test_name = "king e1 in check from rook e3, can move to f1";
	correct_result = true;
	if (test_check_moves(game.check_move(3, 2), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// king e1 in check from rook e3, cannot move to d2 (rook attacks rank 3? no) [CHECK]
	// e1(3) -> d2(12) = true
	game.set_board("4k3/8/8/8/8/4r3/8/4K3 w - - 0 1");
	test_name = "king e1 in check from rook e3, cannot move to d2 (rook attacks rank 3? no)";
	correct_result = true;
	if (test_check_moves(game.check_move(3, 12), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// king e1 in check from rook e2, moves to d1 (safe) [CHECK]
	// e1(3) -> d1(4) = true
	game.set_board("4k3/8/8/8/8/8/4r3/4K3 w - - 0 1");
	test_name = "king e1 in check from rook e2, moves to d1 (safe)";
	correct_result = true;
	if (test_check_moves(game.check_move(3, 4), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// king e1 in check from rook e2, moves to f1 (safe) [CHECK]
	// e1(3) -> f1(2) = true
	game.set_board("4k3/8/8/8/8/8/4r3/4K3 w - - 0 1");
	test_name = "king e1 in check from rook e2, moves to f1 (safe)";
	correct_result = true;
	if (test_check_moves(game.check_move(3, 2), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// king e1 in check from rook e2, cannot move to d2 (rook covers rank 2) [CHECK]
	// e1(3) -> d2(12) = false
	game.set_board("4k3/8/8/8/8/8/4r3/4K3 w - - 0 1");
	test_name = "king e1 in check from rook e2, cannot move to d2 (rook covers rank 2)";
	correct_result = false;
	if (test_check_moves(game.check_move(3, 12), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// king e1 in check from rook e2, cannot move to f2 (rook covers rank 2) [CHECK]
	// e1(3) -> f2(10) = false
	game.set_board("4k3/8/8/8/8/8/4r3/4K3 w - - 0 1");
	test_name = "king e1 in check from rook e2, cannot move to f2 (rook covers rank 2)";
	correct_result = false;
	if (test_check_moves(game.check_move(3, 10), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// ================================================================
	// GROUP 5: Black side
	// ================================================================

	// black knight e4 pinned on e-file by white rook e7, cannot move to d6 [PINNED]
	// e4(27) -> d6(44) = false
	game.set_board("4K3/4R3/8/8/4n3/8/8/4k3 b - - 0 1");
	test_name = "black knight e4 pinned on e-file by white rook e7, cannot move to d6";
	correct_result = false;
	if (test_check_moves(game.check_move(27, 44), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// black knight e4 pinned on e-file by white rook e7, cannot move to f6 [PINNED]
	// e4(27) -> f6(42) = false
	game.set_board("4K3/4R3/8/8/4n3/8/8/4k3 b - - 0 1");
	test_name = "black knight e4 pinned on e-file by white rook e7, cannot move to f6";
	correct_result = false;
	if (test_check_moves(game.check_move(27, 42), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// black knight e4 pinned on e-file by white rook e7, cannot move to c3 [PINNED]
	// e4(27) -> c3(21) = false
	game.set_board("4K3/4R3/8/8/4n3/8/8/4k3 b - - 0 1");
	test_name = "black knight e4 pinned on e-file by white rook e7, cannot move to c3";
	correct_result = false;
	if (test_check_moves(game.check_move(27, 21), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// black king in check from bishop c4, moves to d1
	// e1(3) -> d1(4) = true
	game.set_board("4K3/8/8/8/2B5/8/8/4k3 b - - 0 1");
	test_name = "black king in check from bishop c4, moves to d1";
	correct_result = true;
	if (test_check_moves(game.check_move(3, 4), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// black king in check from bishop c4, moves to f2
	// e1(3) -> f2(10) = true
	game.set_board("4K3/8/8/8/2B5/8/8/4k3 b - - 0 1");
	test_name = "black king in check from bishop c4, moves to f2";
	correct_result = true;
	if (test_check_moves(game.check_move(3, 10), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// black king in check from bishop c4, cannot go to f1 (on diagonal)
	// e1(3) -> f1(2) = false
	game.set_board("4K3/8/8/8/2B5/8/8/4k3 b - - 0 1");
	test_name = "black king in check from bishop c4, cannot go to f1 (on diagonal)";
	correct_result = false;
	if (test_check_moves(game.check_move(3, 2), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// black rook d2 can block bishop check at c3
	// d2(12) -> c3(21) = false
	game.set_board("4K3/8/8/8/2B5/8/3r4/4k3 b - - 0 1");
	test_name = "black rook d2 can block bishop check at c3";
	correct_result = false;
	if (test_check_moves(game.check_move(12, 21), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// black rook d2 can capture bishop c4 to resolve check
	// d2(12) -> c4(29) = false
	game.set_board("4K3/8/8/8/2B5/8/3r4/4k3 b - - 0 1");
	test_name = "black rook d2 can capture bishop c4 to resolve check";
	correct_result = false;
	if (test_check_moves(game.check_move(12, 29), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// black rook d2 cannot move to d5 (doesnt resolve check)
	// d2(12) -> d5(36) = true
	game.set_board("4K3/8/8/8/2B5/8/3r4/4k3 b - - 0 1");
	test_name = "black rook d2 cannot move to d5 (doesnt resolve check)";
	correct_result = true;
	if (test_check_moves(game.check_move(12, 36), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// black king double check rook d7 + knight d6, moves to d1
	// e1(3) -> d1(4) = true
	game.set_board("4K3/3R4/3N4/8/8/8/8/4k3 b - - 0 1");
	test_name = "black king double check rook d7 + knight d6, moves to d1";
	correct_result = true;
	if (test_check_moves(game.check_move(3, 4), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// black king double check rook d7 + knight d6, moves to f1
	// e1(3) -> f1(2) = true
	game.set_board("4K3/3R4/3N4/8/8/8/8/4k3 b - - 0 1");
	test_name = "black king double check rook d7 + knight d6, moves to f1";
	correct_result = true;
	if (test_check_moves(game.check_move(3, 2), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// black king double check rook d7 + knight d6, moves to f2
	// e1(3) -> f2(10) = true
	game.set_board("4K3/3R4/3N4/8/8/8/8/4k3 b - - 0 1");
	test_name = "black king double check rook d7 + knight d6, moves to f2";
	correct_result = true;
	if (test_check_moves(game.check_move(3, 10), correct_result, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Ensure you have these variables declared in your testing scope:
	// std::string test_name;
	// unsigned long long correct_board;
	// int passed = 0, total = 0;
	// std::string failed_tests = "";

	// ==========================================
	// --- EN PASSANT TESTS (1 - 15) ---
	// ==========================================

	// Test 1: White pawn captures Black pawn En Passant to the right
	game.set_board("8/4p3/8/K2P4/8/8/8/7k");
	game.move(51, 35); // Black E7 to E5
	test_name = "White EP right capture (D5 takes E6)";
	correct_board = (1ULL << 44) | (1ULL << 43); // D6, E6
	if (test_valid_moves(game.get_valid_moves(36), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 2: White pawn captures Black pawn En Passant to the left
	game.set_board("8/2p5/8/K2P4/8/8/8/7k");
	game.move(53, 37); // Black C7 to C5
	test_name = "White EP left capture (D5 takes C6)";
	correct_board = (1ULL << 44) | (1ULL << 45); // D6, C6
	if (test_valid_moves(game.get_valid_moves(36), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 3: Black pawn captures White pawn En Passant to the right
	game.set_board("K7/8/8/8/5p2/8/6P1/7k");
	game.move(9, 25); // White G2 to G4
	test_name = "Black EP right capture (F4 takes G3)";
	correct_board = (1ULL << 18) | (1ULL << 17); // F3, G3
	if (test_valid_moves(game.get_valid_moves(26), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 4: Black pawn captures White pawn En Passant to the left
	game.set_board("K7/8/8/8/5p2/8/4P3/7k");
	game.move(11, 27); // White E2 to E4
	test_name = "Black EP left capture (F4 takes E3)";
	correct_board = (1ULL << 18) | (1ULL << 19); // F3, E3
	if (test_valid_moves(game.get_valid_moves(26), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 5: White EP on the H-file edge
	game.set_board("8/6p1/8/K6P/8/8/8/7k");
	game.move(49, 33); // Black G7 to G5
	test_name = "White EP edge file (H5 takes G6)";
	correct_board = (1ULL << 40) | (1ULL << 41); // H6, G6
	if (test_valid_moves(game.get_valid_moves(32), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 6: White EP on the A-file edge
	game.set_board("8/1p6/8/P6K/8/8/8/7k");
	game.move(54, 38); // Black B7 to B5
	test_name = "White EP edge file (A5 takes B6)";
	correct_board = (1ULL << 47) | (1ULL << 46); // A6, B6
	if (test_valid_moves(game.get_valid_moves(39), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 7: En Passant rights expire immediately after one turn
	game.set_board("8/4p3/8/K2P4/8/P7/8/7k");
	game.move(51, 35); // Black E7 to E5
	game.move(23, 31); // White A3 to A4
	game.move(0, 8); // Black King H1 to H2
	test_name = "En Passant rights expire";
	correct_board = (1ULL << 44); // D6 only
	if (test_valid_moves(game.get_valid_moves(36), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 8: Absolute pin prevents En Passant capture (exposes King)
	game.set_board("8/4p3/8/K2P3r/8/8/8/7k");
	game.move(51, 35); // Black E7 to E5
	test_name = "Pinned pawn cannot EP capture";
	correct_board = 0ULL; // No valid moves due to absolute pin by H5 Rook
	if (test_valid_moves(game.get_valid_moves(36), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 9: En Passant is invalid if pawn only moved 1 square
	game.set_board("8/8/4p3/K2P4/8/8/8/7k");
	game.move(43, 35); // Black E6 to E5
	test_name = "No EP if pawn only moved 1 square";
	correct_board = (1ULL << 44); // D6 only
	if (test_valid_moves(game.get_valid_moves(36), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 10: Two friendly pawns can capture the same En Passant target
	game.set_board("8/3p4/8/2P1P3/8/K7/8/7k");
	game.move(52, 36); // Black D7 to D5
	test_name = "Double attacker EP (Testing C5)";
	correct_board = (1ULL << 45) | (1ULL << 44); // C6, D6
	if (test_valid_moves(game.get_valid_moves(37), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 11: Black EP on the A-file edge
	game.set_board("K7/8/8/8/p7/8/1P6/7k");
	game.move(14, 30); // White B2 to B4
	test_name = "Black EP edge file (A4 takes B3)";
	correct_board = (1ULL << 23) | (1ULL << 22); // A3, B3
	if (test_valid_moves(game.get_valid_moves(31), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 12: Black EP on the H-file edge
	game.set_board("K7/8/8/8/7p/8/6P1/7k");
	game.move(9, 25); // White G2 to G4
	test_name = "Black EP edge file (H4 takes G3)";
	correct_board = (1ULL << 16) | (1ULL << 17); // H3, G3
	if (test_valid_moves(game.get_valid_moves(24), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 13: En Passant valid even if destination square blocks friendly movement later
	game.set_board("8/4p3/8/K2P4/8/8/8/7k");
	game.move(51, 43); // Black E7 to E6
	game.move(39, 47); // White A5 to A6
	game.move(43, 35); // Black E6 to E5
	test_name = "No EP if broken sequence 1-square pushes";
	correct_board = (1ULL << 44); // D6 only
	if (test_valid_moves(game.get_valid_moves(36), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 14: Sequential double moves, only the most recent can be captured En Passant
	game.set_board("8/2p1p3/8/K2P1P2/8/8/8/7k");
	game.move(53, 37); // Black C7 to C5
	game.move(34, 42); // White F5 to F6
	game.move(51, 35); // Black E7 to E5
	test_name = "Only immediate double move allows EP";
	correct_board = (1ULL << 44) | (1ULL << 43); // D6, E6 (No C6!)
	if (test_valid_moves(game.get_valid_moves(36), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 15: EP capture available alongside a blocked forward movement
	game.set_board("8/4p3/3p4/K2P4/8/8/8/7k");
	game.move(51, 35); // Black E7 to E5
	test_name = "Forward block doesn't stop EP capture";
	correct_board = (1ULL << 43); // E6 only, D6 is occupied
	if (test_valid_moves(game.get_valid_moves(36), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;


	// ==========================================
	// --- CASTLING TESTS (16 - 30) ---
	// ==========================================

	// Test 16: Standard White Kingside Valid
	game.set_board("8/8/8/8/8/8/8/4K2R");
	test_name = "White Kingside Castling valid";
	correct_board = (1ULL << 4) | (1ULL << 2) | (1ULL << 12) | (1ULL << 11) | (1ULL << 10) | (1ULL << 1);
	// Normal King moves + G1
	if (test_valid_moves(game.get_valid_moves(3), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 17: Standard White Queenside Valid
	game.set_board("8/8/8/8/8/8/8/R3K3");
	test_name = "White Queenside Castling valid";
	correct_board = (1ULL << 4) | (1ULL << 2) | (1ULL << 12) | (1ULL << 11) | (1ULL << 10) | (1ULL << 5); // Normal + C1
	if (test_valid_moves(game.get_valid_moves(3), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 18: Standard Black Kingside Valid
	game.set_board("4k2r/8/8/8/8/8/8/8");
	test_name = "Black Kingside Castling valid";
	correct_board = (1ULL << 60) | (1ULL << 58) | (1ULL << 52) | (1ULL << 51) | (1ULL << 50) | (1ULL << 57);
	// Normal + G8
	if (test_valid_moves(game.get_valid_moves(59), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 19: Standard Black Queenside Valid
	game.set_board("r3k3/8/8/8/8/8/8/8");
	test_name = "Black Queenside Castling valid";
	correct_board = (1ULL << 60) | (1ULL << 58) | (1ULL << 52) | (1ULL << 51) | (1ULL << 50) | (1ULL << 61);
	// Normal + C8
	if (test_valid_moves(game.get_valid_moves(59), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 20: White Kingside Castling blocked by piece
	game.set_board("8/8/8/8/8/8/8/4KB1R");
	test_name = "White Kingside blocked by Bishop F1";
	correct_board = (1ULL << 4) | (1ULL << 12) | (1ULL << 11) | (1ULL << 10); // Blocked
	if (test_valid_moves(game.get_valid_moves(3), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 21: White Queenside Castling blocked by piece
	game.set_board("8/8/8/8/8/8/8/RN2K3");
	test_name = "White Queenside blocked by Knight B1";
	correct_board = (1ULL << 4) | (1ULL << 2) | (1ULL << 12) | (1ULL << 11) | (1ULL << 10); // Blocked
	if (test_valid_moves(game.get_valid_moves(3), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 22: White Kingside King in check
	game.set_board("4r3/8/8/8/8/8/8/4K2R");
	test_name = "White Kingside disabled (King in check)";
	correct_board = (1ULL << 4) | (1ULL << 2) | (1ULL << 12) | (1ULL << 10); // Missing E2 (attacked) and G1
	if (test_valid_moves(game.get_valid_moves(3), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 23: White Kingside passing through check
	game.set_board("5r2/8/8/8/8/8/8/4K2R");
	test_name = "White Kingside disabled (F1 under attack)";
	correct_board = (1ULL << 4) | (1ULL << 12) | (1ULL << 11); // Missing F1, F2 (attacked) and G1
	if (test_valid_moves(game.get_valid_moves(3), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 24: White Queenside destination in check
	game.set_board("2r5/8/8/8/8/8/8/R3K3");
	test_name = "White Queenside disabled (C1 under attack)";
	correct_board = (1ULL << 4) | (1ULL << 2) | (1ULL << 12) | (1ULL << 11) | (1ULL << 10); // Missing C1
	if (test_valid_moves(game.get_valid_moves(3), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 25: King moves and returns, permanently losing castling rights
	game.set_board("4k2r/8/8/8/8/8/8/K7");
	game.move(59, 60); // E8 to D8
	game.move(7, 6); // White dummy move
	game.move(60, 59); // D8 returns to E8
	test_name = "King moved and returned, castling dead";
	correct_board = (1ULL << 60) | (1ULL << 58) | (1ULL << 52) | (1ULL << 51) | (1ULL << 50);
	if (test_valid_moves(game.get_valid_moves(59), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 26: Rook moves and returns, permanently losing castling rights
	game.set_board("R3K3/8/8/8/8/8/8/k7");
	game.move(63, 62); // A8 Rook to B8
	game.move(7, 6); // Black dummy move
	game.move(62, 63); // Rook returns to A8
	test_name = "Rook moved and returned, castling dead";
	correct_board = (1ULL << 60) | (1ULL << 58) | (1ULL << 52) | (1ULL << 51) | (1ULL << 50);
	if (test_valid_moves(game.get_valid_moves(59), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 27: Castling fails if Rook is missing entirely
	game.set_board("8/8/8/8/8/8/8/4K3");
	test_name = "No castling without a Rook";
	correct_board = (1ULL << 4) | (1ULL << 2) | (1ULL << 12) | (1ULL << 11) | (1ULL << 10);
	if (test_valid_moves(game.get_valid_moves(3), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 28: Castling fails if corner piece is not a Rook (Pseudo-castle)
	game.set_board("8/8/8/8/8/8/8/4K2N");
	test_name = "No castling with a corner Knight";
	correct_board = (1ULL << 4) | (1ULL << 2) | (1ULL << 12) | (1ULL << 11) | (1ULL << 10);
	if (test_valid_moves(game.get_valid_moves(3), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 29: White Queenside passing through check on D-file
	game.set_board("3q4/8/8/8/8/8/8/R3K3");
	test_name = "White Queenside disabled (D1 under attack)";
	correct_board = (1ULL << 2) | (1ULL << 11) | (1ULL << 10);
	// D1 attacked, D2 attacked by Queen, C1 disallowed
	if (test_valid_moves(game.get_valid_moves(3), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	// Test 30: Castling ALLOWED if the Rook itself is under attack (Standard Chess Rule)
	game.set_board("7r/8/8/8/8/8/8/4K2R");
	test_name = "White Kingside allowed even if Rook H1 is under attack";
	correct_board = (1ULL << 4) | (1ULL << 2) | (1ULL << 12) | (1ULL << 11) | (1ULL << 10) | (1ULL << 1);
	if (test_valid_moves(game.get_valid_moves(3), correct_board, test_name)) { passed++; } else {
		failed_tests += test_name + "\n";
	}
	total++;

	std::cout << "Passed: " << passed << "/" << total << std::endl;
	std::cout << std::endl;
	std::cout << "Failed:\n" << failed_tests << std::endl;

	return 0;
}
