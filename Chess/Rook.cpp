#include "Rook.h"

Rook::Rook(COLORS color) {
	setColor(color);
	setType(ROOK);
	sliding = true;
	const int rookMoves[4][2] = {
		{  1,  0 }, {  -1, 0 },
		{ 0,  1 }, { 0, -1 }
	};

	for (size_t i = 0; i < 4; i++) {
		addMove(rookMoves[i]);
	}
}

Piece* Rook::clone() const {
	return new Rook(*this);
}

bool Rook::canAttack(int fromRow, int fromCol, int toRow, int toCol, Piece* const board[BOARD_SIZE][BOARD_SIZE]) const {
	if (fromRow != toRow && fromCol != toCol) {
		return false;
	}

	int rowDir = (toRow - fromRow) == 0 ? 0 : (toRow - fromRow) > 0 ? 1 : -1;
	int colDir = (toCol - fromCol) == 0 ? 0 : (toCol - fromCol) > 0 ? 1 : -1;

	int r = fromRow + rowDir, c = fromCol + colDir;
	while (r != toRow || c != toCol) {
		if (!board[r][c]->isEmpty()) return false;
		r += rowDir;
		c += colDir;
	}
	return true;
}
