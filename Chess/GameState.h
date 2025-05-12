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
};
