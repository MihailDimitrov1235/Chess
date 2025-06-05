#pragma once
const int BOARD_SIZE = 8;
const int COORDINATES = 2;

extern const wchar_t* WHITE_PIECES[];
extern const wchar_t* BLACK_PIECES[];

enum PIECES {
	EMPTY_SQUARE = -1,
	KING = 0,
	QUEEN = 1,
	ROOK = 2,
	BISHOP = 3,
	KNIGHT = 4,
	PAWN = 5,
};

enum COLORS {
	NONE = -1,
	WHITE = 0,
	BLACK = 1
};
const int BLACK_OFFSET = 6;

const int WHITE_COLOR = 15;
const int BLACK_COLOR = 0;
const int LIGHT_GRAY_COLOR = 7;
const int DARK_GRAY_COLOR = 8;

extern const char* const fileName;
