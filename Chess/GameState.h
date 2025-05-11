#pragma once
#include "Piece.h"
#include "consts.h"

struct GameState {
	Piece board[BOARD_SIZE][BOARD_SIZE];
	bool whiteMove;

	bool whiteCanCastleLong;
	bool whiteCanCastleShort;
	bool blackCanCastleLong;
	bool blackCanCastleShort;

	int whiteKingRow;
	int whiteKingCol;
	int blackKingRow;
	int blackKingCol;

	int enPassantSquare[2] = { -1, -1 };

	int positionsSize = 0;
	char** positions = nullptr;
	int* positionsCounter = nullptr;
};
