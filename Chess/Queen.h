#pragma once
#include "Rook.h"
#include "Bishop.h"
class Queen : public Rook, public Bishop
{
public:
	Queen(COLORS color);
	Piece* clone() const override;
	bool canAttack(int fromRow, int fromCol, int toRow, int toCol, const Piece* const board[BOARD_SIZE][BOARD_SIZE]) const override;
	void validateMove(int fromRow, int fromCol, int toRow, int toCol, const Piece* const board[BOARD_SIZE][BOARD_SIZE]) const override;
};

