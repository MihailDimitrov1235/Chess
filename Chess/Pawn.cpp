#include "Pawn.h"
#include "utils.h"

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

Piece* Pawn::clone() const {
	return new Pawn(*this);
}

bool Pawn::canAttack(int fromRow, int fromCol, int toRow, int toCol, Piece* const board[BOARD_SIZE][BOARD_SIZE]) const {
	int direction = (getColor() == WHITE) ? -1 : 1;

	if (toRow - fromRow == direction && absVal(toCol - fromCol) == 1) {
		Piece* target = board[toRow][toCol];
		return !target->isEmpty() && target->getColor() != getColor();
	}

	return false;
}

