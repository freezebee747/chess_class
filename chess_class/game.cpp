#include "chess.h"

void chess::GameRoop() {
	while (true) {
		printBoard();
		std::string input;
		std::cout << "next move: ";
		std::cin >> input;
		if (input == "q") {
			return;
		}
		if (Move(readInput(input)) == false) {
			continue;
		}

		resetBitmap();
		CheckCheckMate();
		if (GameEnd) break;
		if (turn == Turn::white) { turn = Turn::black;}
		else if (turn == Turn::black) { turn = Turn::white; fullmove += 1; }
	}
	std::cout << "game end, winner is ";
	if (turn == Turn::white) std::cout << "white" << std::endl;
	else if (turn == Turn::black) std::cout << "black" << std::endl;
}