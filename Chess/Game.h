#include "GameState.h"
#include "MoveValidator.h"

class Game {
private:
	GameState state;
	MoveValidator validator;

	void setupPawns(COLORS color, int row);
	void setupBackRank(COLORS color, int row);
	void handleCastlingRelatedMove(int rowFrom, int colFrom, int rowTo, int colTo);
	void handleEnPassantRelatedMove(int rowFrom, int colFrom, int rowTo, int colTo);
	void printCols(bool reverse);

public:
	Game();
	void printBoard();
	bool isGameOver();
	void makeMove();
};
