#include "chess.h"

void chess::printBoard() {
	int bar = 8;
	std::bitset<64> RealBitmap = bitmap;
	int startValue = 64;

	for (int i = 0; i < 8; i++) {
		startValue = startValue - 8;
		std::cout << bar << "  ";

		for (int j = 0; j < 8; j++) {
			std::cout << RealBitmap[startValue + j] << ' ';
		}
		bar--;
		std::cout << '\n';
	}
	std::cout << '\n';
	std::cout << "   A B C D E F G H" << '\n';
	std::cout << "turn: ";
	if (turn == white) {
		std::cout << "white" << '\n';
	}
	else if (turn == black) {
		std::cout << "black" << '\n';
	}
	std::cout << "casling:";
	if (WhiteQueenSideCasling == true) std::cout << "Q";
	if (WhiteKingSideCasling == true) std::cout << "K";
	if (BlackQueenSideCasling == true) std::cout << "q";
	if (BlackKingSideCasling == true) std::cout << "k";
	std::cout << '\n';

	std::cout << "Half move: " << halfmove << "    ";
	std::cout << "Full move: " << fullmove << '\n';
}

void chess::printtest() {
	int bar = 8;
	std::bitset<64> RealBitmap = BlackRookField;
	int startValue = 64;

	for (int i = 0; i < 8; i++) {
		startValue = startValue - 8;
		std::cout << bar << "  ";

		for (int j = 0; j < 8; j++) {
			std::cout << RealBitmap[startValue + j] << ' ';
		}
		bar--;
		std::cout << '\n';
	}
	std::cout << '\n';
	std::cout << "   A B C D E F G H" << '\n';
	std::cout << "turn: ";
	if (turn == white) {
		std::cout << "white" << '\n';
	}
	else if (turn == black) {
		std::cout << "black" << '\n';
	}
	std::cout << "casling:";
	if (WhiteQueenSideCasling == true) std::cout << "Q";
	if (WhiteKingSideCasling == true) std::cout << "K";
	if (BlackQueenSideCasling == true) std::cout << "q";
	if (BlackKingSideCasling == true) std::cout << "k";
	std::cout << '\n';

	std::cout << "Half move: " << halfmove << "    ";
	std::cout << "Full move: " << fullmove << '\n';
}


void chess::resetBitmap() {
	WhiteMap = WhitePawnField | WhiteKnightField | WhiteBeshopField | WhiteRookField | WhiteQueenField | WhiteKingField;
	BlackMap = BlackPawnField | BlackKnightField | BlackBeshopField | BlackRookField | BlackQueenField | BlackKingField;
	bitmap = WhiteMap | BlackMap;

}