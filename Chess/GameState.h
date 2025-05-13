#pragma once

struct GameState {
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

	size_t whiteTimeInMs = 0;
	size_t blackTimeInMs = 0;
	size_t incrementInMs = 0;
	bool timedGame = false;
	bool paused = true;
};
