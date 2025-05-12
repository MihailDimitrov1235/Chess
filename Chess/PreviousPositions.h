#pragma once
#include "Piece.h"
#include "consts.h"

struct PreviousPositions {
	int positionsSize = 0;
	char** positions = nullptr;
	int* positionsCounter = nullptr;
};
