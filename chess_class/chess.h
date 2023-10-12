#pragma once
#include <iostream>
#include <bitset>

#define U64 uint64_t

#define DefaultFEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

enum square {
	a1, b1, c1, d1, e1, f1, g1, h1,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a8, b8, c8, d8, e8, f8, g8, h8,
	none
};

enum Turn { white, black };
enum Piece { pawn, knight, beshop, rook, queen, king };
enum casling {none_casling, king_side, queen_side};
struct IO_info {
	Piece piece;
	int target;
	int move = casling::none_casling;
	int casling;
	bool takes = false;
	bool enpassnt = false;
	bool checkmate = false;
	
};

__declspec(selectany) U64 BeshopMask[64];
__declspec(selectany) U64 RookMask[64];
__declspec(selectany) U64 QueenMask[64];
__declspec(selectany) U64 OnlyFile[8] = {
	0x101010101010101, 0x202020202020202, 0x404040404040404, 0x808080808080808,
	0x1010101010101010, 0x2020202020202020, 0x4040404040404040, 0x8080808080808080};


U64 rankMask(int sq);
U64 fileMask(int sq);
U64 diagonalMask(int sq);
U64 antiDiagMask(int sq);
void init_Mask();

unsigned int generate_random_number();
U64 random_U64();
U64 random_fewbits();
int count_bits(U64 botboard);
int get_ls1b_index(U64 bitboard);
U64 set_occupancy(int index, int bits_in_mask, U64 attack_mask);
U64 mask_bishop_attacks(int square);
U64 mask_rook_attacks(int square);
U64 bishop_attacks_on_the_fly(int square, U64 block);
U64 rook_attacks_on_the_fly(int square, U64 block);
U64 find_magic(int square, int relevant_bits, int bishop);
void init_sliders_attacks(int is_bishop);

__declspec(selectany) U64 bishop_masks[64];
__declspec(selectany) U64 rook_masks[64];

// attacks
__declspec(selectany) U64 bishop_attacks[64][512];
__declspec(selectany) U64 rook_attacks[64][4096];

class chess {
private:
	U64 bitmap = 0;
	bool GameEnd = false;

	U64 WhitePawnField = 0;
	U64 WhiteKnightField = 0;
	U64 WhiteBeshopField = 0;
	U64 WhiteRookField = 0;
	U64 WhiteQueenField = 0;
	U64 WhiteKingField = 0;
	U64 WhiteMap = 0;

	U64 BlackPawnField = 0;
	U64 BlackKnightField = 0;
	U64 BlackBeshopField = 0;
	U64 BlackRookField = 0;
	U64 BlackQueenField = 0;
	U64 BlackKingField = 0;
	U64 BlackMap = 0;

	//move flag
	bool WLRookFlag = true;
	bool BLRookFlag = true;
	bool WRRookFlag = true;
	bool BRRookFlag = true;
	bool WKingMoveFlag = true;
	bool BKingMoveFlag = true;

	int turn = Turn::white;

	bool WhiteQueenSideCasling = false;
	bool WhiteKingSideCasling = false;
	bool BlackQueenSideCasling = false;
	bool BlackKingSideCasling = false;

	int enpassant = none;

	int halfmove = 0;
	int fullmove = 0;

	void AttackAndDelete(IO_info* info);
	
	U64 get_bishop_attacks(int square, U64 occupancy);
	U64 get_rook_attacks(int square, U64 occupancy);
	bool addPiece(int piece, int target);
	
	U64 PawnAttackMap();
	U64 KnightAttackMap();
	U64 BeshopAttackMap();
	U64 RookAttackMap();
	U64 QueenAttackMap();

public:

	void readFEN(std::string FEN);
	void printBoard();
	void resetBitmap();
	
	U64 knightAttacks(U64 knights);

	// ACN = Algebraic chess notation , 대수 기본 표기법
	IO_info* readInput(std::string ACN);
	square CheckSquare(std::string sq);
	square FindTarget(IO_info* io);
	square FindTarget(IO_info* io, std::string row);


	//move

	U64 noea(U64 target) { return target << 7; }
	U64 nort(U64 target) { return target << 8; }
	U64 nowe(U64 target) { return target << 9; }
	U64 east(U64 target) { return target >> 1; }
	U64 west(U64 target) { return target << 1; }
	U64 soea(U64 target) { return target >> 9; }
	U64 sout(U64 target) { return target >> 8; }
	U64 sowe(U64 target) { return target >> 7; }

	U64 KingMoveMask(U64 target) {
		return (noea(target) | nort(target) | nowe(target) | east(target) | west(target) | 
			soea(target) | sout(target) | sowe(target));
	}

	bool movePawn(IO_info* info);
	bool moveKnight(IO_info* info);
	bool moveBeshop(IO_info* info);
	bool moveRook(IO_info* info);
	bool moveQueen(IO_info* info);
	bool moveKing(IO_info* info);
	bool Move(IO_info* info);
	void printtest();
	void GameRoop();

	bool checkmate();
	void CheckCheckMate();
};