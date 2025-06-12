#include "Queen.h"
#include <iostream>
using namespace std;

Queen::Queen(COLORS color) :Piece(color, QUEEN), Rook(color), Bishop(color) {
	setType(QUEEN);
}

Piece* Queen::clone() const {
	return new Queen(*this);
}

bool Queen::canAttack(int fromRow, int fromCol, int toRow, int toCol, const Piece* const board[BOARD_SIZE][BOARD_SIZE]) const {
	return Rook::canAttack(fromRow, fromCol, toRow, toCol, board) || Bishop::canAttack(fromRow, fromCol, toRow, toCol, board);
}

void Queen::validateMove(int fromRow, int fromCol, int toRow, int toCol, const Piece* const board[BOARD_SIZE][BOARD_SIZE]) const {
	if (!canAttack(fromRow, fromCol, toRow, toCol, board)) {
		throw invalid_argument("Invalid Queen move.");
	}
}

