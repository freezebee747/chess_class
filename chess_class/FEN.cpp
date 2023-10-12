#include "chess.h"
#include <string>

void chess::readFEN(std::string FEN) {
	int i = 0;
	int cell = 56;
	int temp;
	bool isSlash = false;
	while (FEN[i] != '\0') {
		switch (FEN[i]) {
		case 'P':
			WhitePawnField = WhitePawnField | (1ULL << cell);
			WhiteMap = WhiteMap | (1ULL << cell);
			bitmap = bitmap | (1ULL << cell);
			cell++;
			break;
		case 'p':
			BlackPawnField = BlackPawnField | (U64(1) << cell);
			BlackMap = BlackMap | (1ULL << cell);
			bitmap = bitmap | (U64(1) << cell);
			cell++;
			break;

		case 'N':
			WhiteKnightField = WhiteKnightField | (U64(1) << cell);
			WhiteMap = WhiteMap | (1ULL << cell);
			bitmap = bitmap | (U64(1) << cell);
			cell++;
			break;
		case 'n':
			BlackKnightField = BlackKnightField | (U64(1) << cell);
			BlackMap = BlackMap | (1ULL << cell);
			bitmap = bitmap | (U64(1) << cell);
			cell++;
			break;

		case 'R':
			WhiteRookField = WhiteRookField | (U64(1) << cell);
			WhiteMap = WhiteMap | (1ULL << cell);
			bitmap = bitmap | (U64(1) << cell);
			cell++;
			break;
		case 'r':
			BlackRookField = BlackRookField | (U64(1) << cell);
			BlackMap = BlackMap | (1ULL << cell);
			bitmap = bitmap | (U64(1) << cell);
			cell++;
			break;

		case 'B':
			WhiteBeshopField = WhiteBeshopField | (U64(1) << cell);
			WhiteMap = WhiteMap | (1ULL << cell);
			bitmap = bitmap | (U64(1) << cell);
			cell++;
			break;
		case 'b':
			BlackBeshopField = BlackBeshopField | (U64(1) << cell);
			BlackMap = BlackMap | (1ULL << cell);
			bitmap = bitmap | (U64(1) << cell);
			cell++;
			break;

		case 'Q':
			WhiteQueenField = WhiteQueenField | (U64(1) << cell);
			WhiteMap = WhiteMap | (1ULL << cell);
			bitmap = bitmap | (U64(1) << cell);
			cell++;
			break;
		case 'q':
			BlackQueenField = BlackQueenField | (U64(1) << cell);
			BlackMap = BlackMap | (1ULL << cell);
			bitmap = bitmap | (U64(1) << cell);
			cell++;
			break;

		case 'K':
			WhiteKingField = WhiteKingField | (U64(1) << cell);
			WhiteMap = WhiteMap | (1ULL << cell);
			bitmap = bitmap | (U64(1) << cell);
			cell++;
			break;
		case 'k':
			BlackKingField = BlackKingField | (U64(1) << cell);
			BlackMap = BlackMap | (1ULL << cell);
			bitmap = bitmap | (U64(1) << cell);
			cell++;
			break;

		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
			temp = static_cast<int>(FEN[i]) - static_cast<int>('0');
			cell = cell + temp;
			break;

		case '/':
			isSlash = true;
			break;
		}

		i++;

		if (cell % 8 == 0 && cell != 0 && cell != 8 && !isSlash) {
			cell = cell - 16;
		}
		else if (isSlash) {
			isSlash = false;
		}
		else if (cell == 8) break;
	}
	if (FEN[i] == '\0') {
		return;
	}
	if (FEN[i] != ' ') {
		std::cout << "FEN wrong: Turn isn't write" << '\n';
		return;
	}
	else i++;

	if (FEN[i] == 'w') {
		turn = Turn::white;
		i++;
	}
	else if (FEN[i] == 'b') {
		turn = Turn::black;
		i++;
	}
	else {
		std::cout << "FEN wrong: Doesn't know who's turn" << '\n';
		return;
	}

	

	if (FEN[i] != ' ') {
		std::cout << "FEN wrong: casling is not define" << '\n';
		return;
	}
	else i++;

	if (FEN[i] != '-') {

		while (FEN[i] != ' ') {
			if (FEN[i] == 'q' || FEN[i] == 'Q') {
				WhiteQueenSideCasling = true;
				
			}
			else if (FEN[i] == 'k' || FEN[i] == 'K') {
				WhiteKingSideCasling = true;
				
			}
			i++;
		}


	}
	else { i++; }

	if (FEN[i] != ' ') {
		std::cout << "FEN wrong: casling is not define" << '\n';
		return;
	}
	else i++;
	if (FEN[i] != '-') {
		while (FEN[i] != ' ') {
			if (FEN[i] == 'q' || FEN[i] == 'Q') {
				BlackQueenSideCasling = true;

			}
			else if (FEN[i] == 'k' || FEN[i] == 'K') {
				BlackKingSideCasling = true;

			}
			i++;
		}

	}
	else i++;

	if (FEN[i] != ' ') {
		std::cout << "FEN wrong: half move is not define" << '\n';
		return;
	}
	else i++;

	int check = 0;
	
	while (FEN[i + check] != ' ') {
		check++;
	}

	halfmove = std::stoi(FEN.substr(i, i + check));

	i = i + check;

	if (FEN[i] != ' ') {
		std::cout << "FEN wrong: full move is not define" << '\n';
		return;
	}
	else i++;

	check = 0;
	while (FEN[i + check] != '\0') {
		check++;
	}

	fullmove = std::stoi(FEN.substr(i, i + check));
	

}