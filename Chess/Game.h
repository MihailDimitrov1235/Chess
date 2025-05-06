#pragma once
#include "Piece.h"
#include "consts.h"
class Game
{
private:
	Piece board[BOARD_SIZE][BOARD_SIZE];
	bool whiteMove;

	void setupPawns(COLORS color, int row);
	void setupBackRank(COLORS color, int row);

public:
	Game();
	void printBoard();
};

