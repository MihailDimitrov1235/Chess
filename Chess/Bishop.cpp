#include <iostream>
#include "Bishop.h"
#include "utils.h"

using namespace std;

Bishop::Bishop(COLORS color) : Piece(color, BISHOP) {
	setColor(color);
	setType(BISHOP);
	sliding = true;
	const int bishopMoves[4][2] = {
		{  1,  1 }, {  1, -1 },
		{ -1,  1 }, { -1, -1 }
	};

	for (size_t i = 0; i < 4; i++) {
		addMove(bishopMoves[i]);
	}
}

Piece* Bishop::clone() const {
	return new Bishop(*this);
}

bool Bishop::canAttack(int fromRow, int fromCol, int toRow, int toCol, Piece* const board[BOARD_SIZE][BOARD_SIZE]) const {
	if (absVal(toRow - fromRow) != absVal(toCol - fromCol)) {
		return false;
	}
	int rowDir = (toRow - fromRow) > 0 ? 1 : -1;
	int colDir = (toCol - fromCol) > 0 ? 1 : -1;
	int r = fromRow + rowDir, c = fromCol + colDir;
	while (r != toRow && c != toCol) {
		if (!board[r][c]->isEmpty()) return false;
		r += rowDir;
		c += colDir;
	}
	return true;
}

void Bishop::validateMove(int fromRow, int fromCol, int toRow, int toCol, Piece* const board[BOARD_SIZE][BOARD_SIZE]) const {
	if (!canAttack(fromRow, fromCol, toRow, toCol, board)) {
		throw invalid_argument("Invalid Bishop move.");
	}
}

