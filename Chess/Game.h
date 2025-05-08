#pragma once
#include "Piece.h"
#include "consts.h"
class Game
{
private:
	Piece board[BOARD_SIZE][BOARD_SIZE];
	bool whiteMove;

	bool whiteCanCastleLong;
	bool whiteCanCastleShort;
	bool blackCanCastleLong;
	bool blackCanCastleShort;

	bool blackInCheck;
	bool whiteInCheck;

	int whiteKingRow;
	int whiteKingCol;
	int blackKingRow;
	int blackKingCol;

	void setupPawns(COLORS color, int row);
	void setupBackRank(COLORS color, int row);

	void validateMove(int rowFrom, int colFrom, int rowTo, int colTo);
	void validateKingMove(int rowFrom, int colFrom, int rowTo, int colTo);
	void validateCastle(int rowFrom, int colFrom, int rowTo, int colTo);

	bool isSquareEnemyPiece(int row, int col, PIECES type);
	bool isKingCapturable(int row, int col);

	void printCols(bool reverse);

public:
	Game();
	void printBoard();
	bool isGameOver();
	void makeMove();
};

