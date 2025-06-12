#include "Knight.h"
#include "utils.h"
#include <iostream>
using namespace std;

Knight::Knight(COLORS color) : Piece(color, KNIGHT) {
	sliding = false;
	const int knightMoves[8][2] = {
		{  2,  1 }, {  2, -1 },
		{ -2,  1 }, { -2, -1 },
		{  1,  2 }, {  1, -2 },
		{ -1,  2 }, { -1, -2 }
	};

	for (size_t i = 0; i < 8; i++) {
		addMove(knightMoves[i]);
	}
}

Piece* Knight::clone() const {
	return new Knight(*this);
}

bool Knight::canAttack(int fromRow, int fromCol, int toRow, int toCol, Piece* const board[BOARD_SIZE][BOARD_SIZE]) const {
	int rowDiff = absVal(toRow - fromRow);
	int colDiff = absVal(toCol - fromCol);
	return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
}

void Knight::validateMove(int fromRow, int fromCol, int toRow, int toCol, Piece* const board[BOARD_SIZE][BOARD_SIZE]) const {
	if (!canAttack(fromRow, fromCol, toRow, toCol, board)) {
		throw invalid_argument("Invalid Knight move.");
	}
}

