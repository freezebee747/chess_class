#include "chess.h"

U64 LS1B(U64 x) {
	if (x != 0)
		return x & -static_cast<long long>(x);
	else
		return -1;
}
U64 chess::PawnAttackMap() {
	if (turn == Turn::white) {
		return(WhitePawnField << 7 & -static_cast<long long>(OnlyFile[7])) | (WhitePawnField << 9) & -static_cast<long long>(OnlyFile[0]);
	}
	else if (turn == Turn::black) {
		return ((BlackPawnField >> 7) & -static_cast<long long>(OnlyFile[0])) | ((BlackPawnField >> 9) & -static_cast<long long>(OnlyFile[7]));
	}

}
U64 chess::KnightAttackMap() {
	U64 tempMap = 0;
	U64 returnValue = 0;
	if (turn == Turn::white) {
		tempMap = WhiteKnightField;
	}
	else if (turn == Turn::black) {
		tempMap = BlackKnightField;
	}
	while (true) {
		if (tempMap == 0) break;
		U64 ls1b = LS1B(tempMap);
		tempMap = tempMap - ls1b;
		returnValue = returnValue | knightAttacks(ls1b);
	}
	return returnValue;

}

U64 chess::BeshopAttackMap() {
	U64 tempMap = 0;
	U64 MyMap = 0;
	U64 EnemyMap = 0;
	U64 returnValue = 0;
	if (turn == Turn::white) {
		tempMap = WhiteBeshopField;
		MyMap = WhiteMap;
		EnemyMap = BlackMap;
	}
	else if (turn == Turn::black) {
		tempMap = BlackBeshopField;
		MyMap = BlackMap;
		EnemyMap = WhiteMap;

	}
	while (true) {
		if (tempMap == 0) break;
		U64 ls1b = LS1B(tempMap);
		square target;

		std::bitset<64>FindValue = ls1b;
		for (int i = 0; i < 64; i++) {
			if (FindValue[i] == 1) {
				target = static_cast<square>(i);
			}
		}
		
		tempMap = tempMap - ls1b;
		U64 temp = get_bishop_attacks(target, MyMap);
		U64 temp2 = temp & MyMap;
		temp = temp - temp2;
		returnValue = returnValue | (get_bishop_attacks(target, EnemyMap) & temp);
	}
	return returnValue;
}
U64 chess::RookAttackMap() {
	U64 tempMap = 0;
	U64 MyMap = 0;
	U64 EnemyMap = 0;
	U64 returnValue = 0;
	if (turn == Turn::white) {
		tempMap = WhiteRookField;
		MyMap = WhiteMap;
		EnemyMap = BlackMap;
	}
	else if (turn == Turn::black) {
		tempMap = BlackRookField;
		MyMap = BlackMap;
		EnemyMap = WhiteMap;
	}
	while (true) {
		if (tempMap == 0) break;
		U64 ls1b = LS1B(tempMap);
		square target;

		std::bitset<64>FindValue = ls1b;
		for (int i = 0; i < 64; i++) {
			if (FindValue[i] == 1) {
				target = static_cast<square>(i);
			}
		}

		tempMap = tempMap - ls1b;
		U64 temp = get_rook_attacks(target, MyMap);
		U64 temp2 = temp & MyMap;
		temp = temp - temp2;
		returnValue = returnValue | (get_rook_attacks(target, EnemyMap) & temp);
	}
	return returnValue;
}
U64 chess::QueenAttackMap() {
	U64 tempMap = 0;
	U64 MyMap = 0;
	U64 EnemyMap = 0;
	U64 returnValue = 0;
	if (turn == Turn::white) {
		tempMap = WhiteQueenField;
		MyMap = WhiteMap;
		EnemyMap = BlackMap;
	}
	else if (turn == Turn::black) {
		tempMap = BlackQueenField;
		MyMap = BlackMap;
		EnemyMap = WhiteMap;

	}
	while (true) {
		if (tempMap == 0) break;
		U64 ls1b = LS1B(tempMap);
		square target;

		std::bitset<64>FindValue = ls1b;
		for (int i = 0; i < 64; i++) {
			if (FindValue[i] == 1) {
				target = static_cast<square>(i);
			}
		}

		tempMap = tempMap - ls1b;
		
		U64 temp = get_rook_attacks(target, MyMap);
		U64 temp2 = temp & MyMap;
		temp = temp - temp2;
		returnValue = returnValue | (get_rook_attacks(target, EnemyMap) & temp);
		
		U64 temp3 = get_bishop_attacks(target, MyMap);
		U64 temp4 = temp & MyMap;
		temp = temp3 - temp4;
		returnValue = returnValue | (get_bishop_attacks(target, EnemyMap) & temp);
	}
	return returnValue;
}

//�� ������ ���Ŀ� ����� ŷ�� ���ݹް� �ִ°�?
void chess::CheckCheckMate() {
	U64 king = 0;
	U64 MyBitmap = 0;
	U64 EnemyKing = 0;
	if (turn == Turn::white) {
		king = BlackKingField;
		EnemyKing = WhiteKingField;
		MyBitmap = BlackMap;
	}
	else if (turn == Turn::black) {
		king = WhiteKingField;
		EnemyKing = BlackKingField;
		MyBitmap = WhiteMap;

	}
	U64 Attackmap = PawnAttackMap() | KnightAttackMap() | BeshopAttackMap() |
		RookAttackMap() | QueenAttackMap() | KingMoveMask(EnemyKing);
	if ((Attackmap & king) != 0) {
		if (checkmate()) {
			GameEnd = true;
		}
	}
	else {
		return;
	}
}


//1. �� ŷ�� ���ݹ޾Ҵ°�?
//2. ���ݹ޾Ҵٸ� ������ �� �ִ� ĭ�� �ִ°�?
//3. ������ �� �ִ� ĭ�� �� �⹰�� �����ϴ°�?
//4. üũ����Ʈ!
//true üũ����Ʈ, false üũ����Ʈ �ƴ�
bool chess::checkmate() {
	U64 king = 0;
	U64 MyBitmap = 0;
	U64 EnemyKing = 0;
	if (turn == Turn::white) {
		king = BlackKingField;
		EnemyKing = WhiteKingField;
		MyBitmap = BlackMap;
	}
	else if (turn == Turn::black) {
		king = WhiteKingField;
		EnemyKing = BlackKingField;
		MyBitmap = WhiteMap;
	}

	//���ݹ޾Ҵٸ� ������ �� �ִ� ĭ�� �ִ����� üũ
	//��� �⹰�� ������ or �����Ѵ�
	U64 Attackmap = PawnAttackMap() | KnightAttackMap() | BeshopAttackMap() |
		RookAttackMap() | QueenAttackMap() | KingMoveMask(EnemyKing);

	U64 KingField = KingMoveMask(king);
	U64 temp = Attackmap & KingField;
	U64 KingMoveAble = temp ^ KingField;
	//����� �������� ������ �� �ִ� ĭ�� ����
	if (KingMoveAble == 0) {
		return true;
	}
	U64 KingStuck = KingMoveAble & bitmap;
	KingStuck = KingMoveAble - KingStuck;

	if (KingStuck == 0) {
		return true;
	}
	return false;
}