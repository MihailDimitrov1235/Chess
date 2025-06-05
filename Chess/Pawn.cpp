#include "Pawn.h"

Pawn::Pawn(COLORS color) : Piece(color, PAWN) {
	sliding = false;
	const int pawnMoves[4][2] = {
		{  1,  0 }, {  1, 1 },
		{ 1,  -1 }, { 2, 0 }
	};

	for (size_t i = 0; i < 4; i++) {
		int move[2];
		move[0] = color == WHITE ? pawnMoves[i][0] : -pawnMoves[i][0];
		move[1] = pawnMoves[i][1];
		addMove(move);
	}
}
