#include "chess.h"



U64 rankMask(int sq) { return  0xffULL << (sq & 56); }

U64 fileMask(int sq) { return 0x0101010101010101ULL << (sq & 7); }

U64 diagonalMask(int sq) {
	const U64 maindia = 0x8040201008040201ULL;
	int diag = 8 * (sq & 7) - (sq & 56);
	int nort = -diag & (diag >> 31);
	int sout = diag & (-diag >> 31);
	return (maindia >> sout) << nort;
}

U64 antiDiagMask(int sq) {
	const U64 maindia = 0x0102040810204080ULL;
	int diag = 56 - 8 * (sq & 7) - (sq & 56);
	int nort = -diag & (diag >> 31);
	int sout = diag & (-diag >> 31);
	return (maindia >> sout) << nort;
}


void init_Mask() {
	for (int i = 0; i < 64; i++) {
		BeshopMask[i] = diagonalMask(i) | antiDiagMask(i);
		RookMask[i] = fileMask(i) | rankMask(i);
		QueenMask[i] = BeshopMask[i] | RookMask[i];
	}
}