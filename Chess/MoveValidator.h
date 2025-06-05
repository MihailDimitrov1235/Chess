#pragma once
#include "GameState.h"
#include "consts.h"
#include "Piece.h"

class MoveValidator {
private:
	Piece* (&board)[BOARD_SIZE][BOARD_SIZE];
	const GameState& state;

	void validateCastle(int rowFrom, int colFrom, int rowTo, int colTo);
	void validateKingSafety(int rowFrom, int colFrom, int rowTo, int colTo);

public:
	MoveValidator(Piece* (&board)[BOARD_SIZE][BOARD_SIZE], GameState& state);

	void validateMove(int rowFrom, int colFrom, int rowTo, int colTo);
	bool isKingCapturableAt(int row, int col);

};
