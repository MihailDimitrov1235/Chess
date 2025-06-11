#pragma once

struct GameState {
	bool whiteMove = true;

	bool whiteCanCastleLong = true;
	bool whiteCanCastleShort = true;
	bool blackCanCastleLong = true;
	bool blackCanCastleShort = true;

	int whiteKingRow = 7;
	int whiteKingCol = 4;
	int blackKingRow = 0;
	int blackKingCol = 4;

	int enPassantSquare[2] = { -1, -1 };

	size_t whiteTimeInMs = 0;
	size_t blackTimeInMs = 0;
	size_t incrementInMs = 0;
	bool timedGame = false;
	bool paused = true;
};
