#include "chess.h"

const std::string CompareValueForPiece = "NBRQK";
const std::string CompareValueForSquare = "abcdefgh";
const std::string CompareValueForNumber = "12345678";

IO_info* chess::readInput(std::string ACN) {
	int i = 0;
	IO_info* ReturnValue = new IO_info;

	bool checkStartRow = false;
	bool checkStartCol = false;

	square finishValue;
	square startValue;
	std::string SquareTemp;

	if (ACN == "oo" || ACN == "OO" ) {
		ReturnValue->piece = rook;
		ReturnValue->casling = queen_side;
		return ReturnValue;
	}
	else if (ACN == "ooo" || ACN == "OOO") {
		ReturnValue->piece = rook;
		ReturnValue->casling = king_side;
		return ReturnValue;
	}

	switch (CompareValueForPiece.find(ACN[i])) {
	case 0:
		ReturnValue->piece = Piece::knight;
		i++;
		break;
	case 1:
		ReturnValue->piece = Piece::beshop;
		i++;
		break;
	case 2:
		ReturnValue->piece = Piece::rook;
		i++;
		break;
	case 3:
		ReturnValue->piece = Piece::queen;
		i++;
		break;
	case 4:
		ReturnValue->piece = Piece::king;
		i++;
		break;
	default:
		ReturnValue->piece = Piece::pawn;

	}

	std::string strTemp;
	strTemp.erase();
	int test = 0;
	while (ACN[test] != '\0') {
		if (ACN[test] != 'x' && ACN[test] != '+' && 
			!isupper(ACN[test]))
		{
			strTemp = strTemp + ACN[test];
		}
		test++;
	}

	if (strTemp.size() == 2) {

	}
	else if (strTemp.size() == 3) {
		checkStartCol = true;
		SquareTemp = ACN[i];
		i++;
	}
	else if (strTemp.size() == 4) {
		checkStartRow = true;
		checkStartCol = true;
		SquareTemp = ACN.substr(i, 2);
		i += 2;

	}

	//else if (CompareValueForSquare.find(ACN[i]) < 8 && CompareValueForSquare.find(ACN[i]) >= 0) {
	//	if (CompareValueForNumber.find(ACN[i + 1]) < 8 &&
	//		CompareValueForNumber.find(ACN[i + 1]) >= 0) {
	//		if (ReturnValue->piece == pawn) {

	//		}
	//		else {
	//			checkStartRow = true;
	//			checkStartCol = true;
	//			SquareTemp = ACN.substr(i, 2);
	//			i += 2;
	//		}
	//	}
	//	else {
	//		checkStartCol = true;
	//		SquareTemp = ACN[i];
	//		i++;
	//	}

	//}

	if (ACN[i] == 'x') {
		ReturnValue->takes = true;
		i++;
	}
	//else if (ReturnValue->piece == Piece::pawn && ACN[1] == 'x') {
	//	SquareTemp = ACN[0];
	//	ReturnValue->takes = true;
	//	i += 2;
	//}

	if (ACN.back() == '+' || ACN.back() == '#') {
		//ReturnValue->checkmate = true;
		ACN.pop_back();
	}

	if (!checkStartRow && !checkStartCol) {
		finishValue = CheckSquare(ACN.substr(i, 2));
		if (finishValue == square::none) {
			std::cout << "finish value is not define" << std::endl;
			delete& ACN;
			exit(1);
		}

		ReturnValue->move = finishValue;

		if (SquareTemp.empty()) {
			ReturnValue->target = FindTarget(ReturnValue);
		}
		else {
			ReturnValue->target = FindTarget(ReturnValue, SquareTemp);
		}
		
	}
	else if (!checkStartRow && checkStartCol) {
		
		finishValue = CheckSquare(ACN.substr(i, 2));
		if (finishValue == square::none) {
			std::cout << "finish value is not define" << std::endl;
			delete& ACN;
			exit(1);
		}
		ReturnValue->move = finishValue;
		ReturnValue->target = FindTarget(ReturnValue,SquareTemp);
		
	}
	else if (checkStartRow && checkStartCol) {
		startValue = CheckSquare(ACN.substr(i, 2));
		finishValue = CheckSquare(SquareTemp);
		if (finishValue == square::none || startValue ==square::none) {
			std::cout << "finish value or start value is not define" << std::endl;
		}
		else {
			ReturnValue->move = finishValue;
			ReturnValue->target = startValue;
		
			
		}
	}
	return ReturnValue;
}

square chess::CheckSquare(std::string sq) {
	int col;
	int row;
	int returnValue;

	if (CompareValueForSquare.find(sq[0]) < 8) {
		row = CompareValueForSquare.find(sq[0]);
	}
	else { return square::none; }

	if (CompareValueForNumber.find(sq[1]) < 8) {
		col = CompareValueForNumber.find(sq[1]);
	}
	else { return square::none; }
	
	returnValue = col * 8 + row;

	return static_cast<square>(returnValue);

}

square chess::FindTarget(IO_info* io) {
	switch (io->piece) {
	case Piece::pawn:


		if (turn == Turn::white) {
			U64 PawnMoving = (1ULL << io->move);
			if (io->takes == false) {
				U64 compare = (PawnMoving >> 8);

				if ((compare & WhitePawnField) != 0) {
					return static_cast<square>(io->move - 8);
				}

				else if (((compare >> 8) & WhitePawnField) != 0) {
					return static_cast<square>(io->move - 16);
				}

			}
			else if (io->takes == true) {
				int compare1 = (PawnMoving >> 7);
				int compare2 = (PawnMoving >> 9);
				if ((compare1 & WhitePawnField) != 0) {
					return static_cast<square>(io->move - 7);
				}
				if ((compare2 & WhitePawnField) != 0) {
					return static_cast<square>(io->move - 9);
				}

			}
		}
		/////////////////////////////////////////////
		else if (turn == Turn::black) {
			U64 PawnMoving = (1ULL << io->move);
			if (io->takes == false) {
				U64 compare = (PawnMoving << 8);

				if ((compare & BlackPawnField) != 0) {
					return static_cast<square>(io->move + 8);
				}

				else if (((compare << 8) & BlackPawnField) != 0) {
					return static_cast<square>(io->move + 16);
				}

			}
			else if (io->takes == true) {
				int compare1 = (PawnMoving << 7);
				int compare2 = (PawnMoving << 9);
				if ((compare1 & BlackPawnField) != 0) {
					return static_cast<square>(io->move + 7);
				}
				if ((compare2 & BlackPawnField) != 0) {
					return static_cast<square>(io->move + 9);
				}

			}
		}
		break;
	case Piece::knight:
		
		if (turn == Turn::white) {
			U64 CompareValue = knightAttacks(1ULL << io->move);
			if ((WhiteKnightField & CompareValue) != 0) {
				int i = 0;
				std::bitset<64>FindValue = (WhiteKnightField & CompareValue);
				for (i; i < 64; i++) {
					if (FindValue[i] == 1) {
						return static_cast<square>(i);
					}
				}
			}
			else if ((knightAttacks(0) & WhiteKnightField) == 0) {
				return square::none;
			}
		}
		else if (turn == Turn::black) {
			U64 CompareValue = knightAttacks(1ULL << io->move);
			if ((BlackKnightField & CompareValue) != 0) {
				int i = 0;
				std::bitset<64>FindValue = (BlackKnightField & CompareValue);
				if (FindValue == 0 && (knightAttacks(0) | BlackKnightField) == 0) {
					return square::none;
				}
				for (i; i < 64; i++) {
					if (FindValue[i] == 1) {
						return static_cast<square>(i);
					}
				}
			}
			else if ((knightAttacks(0) & BlackKnightField) == 0) {
				return square::none;
			}
		}
	
		break;

	case Piece::beshop:
		if (turn == Turn::white) {
			if ((WhiteBeshopField & BeshopMask[io->move]) != 0) {
				std::bitset<64>FindValue = (WhiteBeshopField & BeshopMask[io->move]);
				for (int i = 0; i < 64; i++) {
					if (FindValue[i] == 1) {
						return static_cast<square>(i);
					}
				}
			}
			else {
				return square::none;
			}
		}
		else if (turn == Turn::black) {
			if ((BlackBeshopField & BeshopMask[io->move]) != 0) {
				std::bitset<64>FindValue = (BlackBeshopField & BeshopMask[io->move]);
				for (int i = 0; i < 64; i++) {
					if (FindValue[i] == 1) {
						return static_cast<square>(i);
					}
				}
			}
			else {
				return square::none;
			}
		}
		break;

	case Piece::rook:
		if (turn == Turn::white) {
			if ((WhiteRookField & RookMask[io->move]) != 0) {
				std::bitset<64>FindValue = (WhiteRookField & RookMask[io->move]);
				for (int i = 0; i < 64; i++) {
					if (FindValue[i] == 1) {
						return static_cast<square>(i);
					}
				}
			}
			else {
				return square::none;
			}
		}
		else if (turn == Turn::black) {
			if ((BlackRookField & RookMask[io->move]) != 0) {
				std::bitset<64>FindValue = (BlackRookField & RookMask[io->move]);
				for (int i = 0; i < 64; i++) {
					if (FindValue[i] == 1) {
						return static_cast<square>(i);
					}
				}
			}
			else {
				return square::none;
			}
		}
		break;
	case Piece::queen:
		if (turn == Turn::white) {
			if ((WhiteQueenField & QueenMask[io->move]) != 0) {
				std::bitset<64>FindValue = (WhiteQueenField & QueenMask[io->move]);
				for (int i = 0; i < 64; i++) {
					if (FindValue[i] == 1) {
						return static_cast<square>(i);
					}
				}
			}
			else {
				return square::none;
			}
		}
		else if (turn == Turn::black) {
			if ((BlackQueenField & QueenMask[io->move]) != 0) {
				std::bitset<64>FindValue = (BlackQueenField & QueenMask[io->move]);
				for (int i = 0; i < 64; i++) {
					if (FindValue[i] == 1) {
						return static_cast<square>(i);
					}
				}
			}
			else {
				return square::none;
			}
		}
		break;
	case Piece::king:
		
		if(turn == Turn::white){
			U64 CompareValue = KingMoveMask(1ULL << (io->move));
			if((WhiteKingField & CompareValue) != 0){
				std::bitset<64>FindValue = (WhiteKingField & CompareValue);
				for (int i = 0; i < 64; i++) {
					if (FindValue[i] == 1) {
						return static_cast<square>(i);
					}
				}
			}
		}
		else if(turn == Turn::black){
			U64 CompareValue = KingMoveMask(1ULL << (io->move));
			if ((BlackKingField & CompareValue) != 0) {
				std::bitset<64>FindValue = (BlackKingField & CompareValue);
				for (int i = 0; i < 64; i++) {
					if (FindValue[i] == 1) {
						return static_cast<square>(i);
					}
				}
			}
		}
		break;
	default:
		return square::none;
		break;
	}
}


square chess::FindTarget(IO_info* io, std::string row) {
	
	int RowCheck = static_cast<int>(row[0]) - static_cast<int>('a');
	U64 target = 0;
	switch(io->piece){
	case Piece::pawn:
		if (turn == Turn::white) { target = WhitePawnField; }
		else if (turn == Turn::black) { target = BlackPawnField; }
		break;

	case Piece::knight:
		if (turn == Turn::white) { target = WhiteKnightField; }
		else if (turn == Turn::black) { target = BlackKnightField; }
		break;

	case Piece::beshop:
		if (turn == Turn::white) { target = WhiteBeshopField; }
		else if (turn == Turn::black) { target = BlackBeshopField; }
		break;

	case Piece::rook:
		if (turn == Turn::white) { target = WhiteRookField; }
		else if (turn == Turn::black) { target = BlackRookField; }
		break;

	case Piece::queen:
		if (turn == Turn::white) { target = WhiteQueenField; }
		else if (turn == Turn::black) { target = BlackQueenField; }
		break;

	case Piece::king:
		if (turn == Turn::white) { target = WhiteKingField; }
		else if (turn == Turn::black) { target = BlackKingField; }
		break;

	}
	if ((OnlyFile[RowCheck] & target) != 0) {
		std::bitset<64>FindValue = (OnlyFile[RowCheck] & target);
		for (int i = 0; i < 64; i++) {
			if (FindValue[i] == 1) {
				return static_cast<square>(i);
			}
		}
	}

	return square::none;
}