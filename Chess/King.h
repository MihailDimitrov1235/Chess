#pragma once
#include "Piece.h"
class King : public Piece {
public:
	King(COLORS color);
	Piece* clone() const override;
	bool canAttack(int fromRow, int fromCol, int toRow, int toCol, Piece* const board[BOARD_SIZE][BOARD_SIZE]) const override;
	void validateMove(int fromRow, int fromCol, int toRow, int toCol, Piece* const board[BOARD_SIZE][BOARD_SIZE]) const override;
};

