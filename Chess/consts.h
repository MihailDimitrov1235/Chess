#pragma once
const int BOARD_SIZE = 8;
const int COORDINATES = 2;

const char fileName[] = "game_save.bin";

const wchar_t CLEAR_SCREEN[] = {
		0x1B, // ESC
		L'[', L'2', L'J',
		0x00  // null terminator
};

const wchar_t CLEAR_SCROLLBACK[] = {
		0x1B, // ESC
		L'[', L'3', L'J',
		0x00  // null terminator
};

const wchar_t MOVE_CURSOR[] = {
	0x1B, // ESC
	L'[', L'H',
	0x00  // null terminator
};

const wchar_t WHITE_PIECES[] = {
	0x2654, // KING
	0x2655, // QUEEN
	0x2656, // ROOK
	0x2657, // BISHOP
	0x2658, // KNIGHT
	0x2659  // PAWN
};

const wchar_t BLACK_PIECES[] = {
	0x265A, // KING
	0x265B, // QUEEN
	0x265C, // ROOK
	0x265D, // BISHOP
	0x265E, // KNIGHT
	0x265F  // PAWN
};

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
