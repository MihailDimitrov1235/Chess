#include "Pawn.h"
#include "utils.h"
#include <iostream>
using namespace std;

Pawn::Pawn(COLORS color) : Piece(color, PAWN) {
	sliding = false;
	const int pawnMoves[4][2] = {
		{  1,  0 }, {  1, 1 },
		{ 1,  -1 }, { 2, 0 }
	};

	for (size_t i = 0; i < 4; i++) {
		int move[2];
		move[0] = color == WHITE ? -pawnMoves[i][0] : pawnMoves[i][0];
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

void Pawn::validateMove(int fromRow, int fromCol, int toRow, int toCol, Piece* const board[BOARD_SIZE][BOARD_SIZE]) const
{
	COLORS enemyColor = getColor() == WHITE ? BLACK : WHITE;
	int direction = (getColor() == WHITE) ? -1 : 1;
	int startRow = (getColor() == WHITE) ? 6 : 1;
	int rowDiff = toRow - fromRow;
	int colDiff = toCol - fromCol;

	if (abs(colDiff) == 1 && rowDiff == direction) {
		if (board[toRow][toCol]->getColor() == enemyColor) {
			return;
		}
		throw invalid_argument("Invalid pawn capture.");
	}

	if (colDiff == 0) {
		if (rowDiff == direction) {
			if (!board[toRow][toCol]->isEmpty()) {
				throw invalid_argument("Cannot move into occupied square.");
			}
			return;
		}

		if (rowDiff == 2 * direction && fromRow == startRow) {
			int intermediateRow = fromRow + direction;
			if (!board[intermediateRow][fromCol]->isEmpty() || !board[toRow][toCol]->isEmpty()) {
				throw invalid_argument("Cannot jump over or into occupied square.");
			}
			return;
		}
	}

	throw invalid_argument("Invalid pawn move.");
}


