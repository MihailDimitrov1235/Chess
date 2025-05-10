#pragma once
#include "GameState.h"

class MoveValidator {
private:
	GameState& state;

	bool isSquareEnemyPiece(int row, int col, PIECES type);
	bool isKingCapturable(int row, int col);

public:
	MoveValidator(GameState& state);

	void validateMove(int rowFrom, int colFrom, int rowTo, int colTo);
	void validateKingMove(int rowFrom, int colFrom, int rowTo, int colTo);
	void validateCastle(int rowFrom, int colFrom, int rowTo, int colTo);
	void validateQueenMove(int rowFrom, int colFrom, int rowTo, int colTo);
	void validateKingSafety(int rowFrom, int colFrom, int rowTo, int colTo);
};
