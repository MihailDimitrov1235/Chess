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
	bool doesPieceHaveLegalMoves(int row, int col);
	bool doesPlayerHaveLegalMoves();
	bool hasPiecesForMate();
	bool hasThreefoldRepetition();
	char* encodeBoard();
	void savePosition();
	void handlePromotion(int row, int col);
	void printCols(bool reverse);
	void freePositionsMemory();

	void copyFrom(const Game& other);
public:
	Game();
	Game(const Game& other);
	Game& operator=(const Game& other);
	~Game();
	void printBoard();
	bool isGameOver();
	void makeMove();
};
