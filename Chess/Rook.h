#pragma once
#include "Piece.h"
class Rook : virtual public Piece
{
public:
	Rook(COLORS color);
	Piece* clone() const override;
	bool canAttack(int fromRow, int fromCol, int toRow, int toCol, Piece* const board[BOARD_SIZE][BOARD_SIZE]) const override;
	void validateMove(int fromRow, int fromCol, int toRow, int toCol,
		Piece* const board[BOARD_SIZE][BOARD_SIZE]) const override;
};

