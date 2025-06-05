#include "Queen.h"
Queen::Queen(COLORS color) :Piece(color, QUEEN), Rook(color), Bishop(color) { }

Piece* Queen::clone() const {
	return new Queen(*this);
}

bool Queen::canAttack(int fromRow, int fromCol, int toRow, int toCol, Piece* const board[BOARD_SIZE][BOARD_SIZE]) const
{
	return Rook::canAttack(fromRow, fromCol, toRow, toCol, board) || Bishop::canAttack(fromRow, fromCol, toRow, toCol, board);
}
