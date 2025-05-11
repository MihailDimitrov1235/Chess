#pragma once
#include "GameState.h"

class MoveValidator {
private:
	GameState& state;

	bool isSquareEnemyPiece(int row, int col, PIECES type);
	void validateKingMove(int rowFrom, int colFrom, int rowTo, int colTo);
	void validateCastle(int rowFrom, int colFrom, int rowTo, int colTo);
	void validateSlidingMove(int rowFrom, int colFrom, int rowTo, int colTo, bool allowDiagonal, bool allowStraight);
	void validateKnightMove(int rowFrom, int colFrom, int rowTo, int colTo);
	void validatePawnMove(int rowFrom, int colFrom, int rowTo, int colTo);
	void validateKingSafety(int rowFrom, int colFrom, int rowTo, int colTo);

public:
	MoveValidator(GameState& state);

	void validateMove(int rowFrom, int colFrom, int rowTo, int colTo);
	bool isKingCapturable(int row, int col);

};
