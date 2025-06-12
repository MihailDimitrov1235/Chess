#pragma once
#include "Piece.h"
#include "GameState.h"
#include "PreviousPositions.h"
class DataManager
{
private:
	Piece* (&board)[BOARD_SIZE][BOARD_SIZE];
	GameState& state;
	PreviousPositions& prevPos;

public:
	DataManager(Piece* (&board)[BOARD_SIZE][BOARD_SIZE], GameState& state, PreviousPositions& prevPos);
	void saveGame();
	void loadGame();
};

