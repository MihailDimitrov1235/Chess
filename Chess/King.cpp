#include "King.h"
#include "utils.h"

King::King(COLORS color) : Piece(color, KING) {
	sliding = false;
	const int kingMoves[10][2] = {
		{  1,  1 }, {  1, 0 },
		{ 1,  -1 }, { 0, -1 },
		{ -1,  -1 }, { -1, 0 },
		{ -1,  1 }, { 0, 1 },
		{ 0,  2 }, { 0, -2 }
	};

	for (size_t i = 0; i < 10; i++) {
		addMove(kingMoves[i]);
	}
}

Piece* King::clone() const {
	return new King(*this);
}

bool King::canAttack(int fromRow, int fromCol, int toRow, int toCol, Piece* const board[BOARD_SIZE][BOARD_SIZE]) const {
	int rowDiff = absVal(fromRow - toRow);
	int colDiff = absVal(fromCol - toCol);
	return (rowDiff <= 1 && colDiff <= 1 && (rowDiff + colDiff != 0));
}