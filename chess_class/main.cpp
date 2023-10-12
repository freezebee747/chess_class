#include "chess.h"

int main() {

	init_Mask();
	//rook map
	init_sliders_attacks(0);
	//beshop map
	init_sliders_attacks(1);

	chess test;
	//std::string testFEN = "r1bqkb1r/1ppp1ppp/p1n2n2/4p3/B3P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 3 5";
	//std::string testFEN2 = "2kr1b1r/1bq3pp/p7/3np3/Ppp1P3/3nBN2/1PB1QPPP/R4RK1 w - - 0 20"; // exd5
	//std::string testFEN3 = "r2q1rk1/2p2ppp/1pn2n1b/p6N/4N3/B7/PP3PPP/1R1Q1RK1 w - - 0 1"; // Nexf6+
	//test.readFEN(testFEN2);
	//test.printBoard();

	//IO_info *io = test.readInput("exd5");
	//std::cout << io->piece << '\n';
	//std::cout << io->target << '\n';
	//std::cout << io->move << '\n';
	test.readFEN(DefaultFEN);
	
	//test.readFEN("rnbqkrn1/2pp1p2/3b3p/pN2N1pQ/2BPP3/6B1/PPP2PPP/R3K2R w KQq - 4 13");
	
	test.GameRoop();

}