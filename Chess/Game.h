#pragma once
#include "Piece.h"
#include "GameState.h"
#include "PreviousPositions.h"
#include "MoveValidator.h"
#include "DataManager.h"

class Game {
private:
	Piece* board[BOARD_SIZE][BOARD_SIZE];
	GameState state;
	PreviousPositions prevPos;
	MoveValidator validator;
	DataManager dataManager;
	size_t lastMoveTimestamp;

	void setupPawns(COLORS color, int row);
	void setupBackRank(COLORS color, int row);
	void handleCastlingRelatedMove(int rowFrom, int colFrom, int rowTo, int colTo);
	void handleEnPassantRelatedMove(int rowFrom, int colFrom, int rowTo, int colTo);
	bool doesPieceHaveLegalMoves(int row, int col);
	bool doesPlayerHaveLegalMoves();
	void savePosition();
	void handlePromotion(int row, int col);
	void handleTimeControl();
	void printCols(bool reverse) const;

	void chooseGamemode();
	void printBoard() const;
	bool isGameOver();
	void makeMove();
	void setTimeControl(size_t totalTimeInMs, size_t timePerMoveInMs);

	void freePositionsMemory();
	void free();
	void copyFrom(const Game& other);
public:
	Game();
	Game(const Game& other);
	Game& operator=(const Game& other);
	~Game();
	void startGame();
};
