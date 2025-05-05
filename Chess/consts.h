#pragma once
const wchar_t* WHITE[] = {
	L"\u2654", // king
	L"\u2655", // queen
	L"\u2656", // rook
	L"\u2657", // bishop
	L"\u2658", // knight
	L"\u2659"  // pawn
};

const wchar_t* BLACK[] = {
	L"\u265A", // king
	L"\u265B", // queen
	L"\u265C", // rook
	L"\u265D", // bishop
	L"\u265E", // knight
	L"\u265F"  // pawn
};

const enum PIECES {
	KING,
	QUEEN,
	ROOK,
	BISHOP,
	KNIGHT,
	PAWN
};

const int WHITE_COLOR = 15;
const int BLACK_COLOR = 0;
const int LIGHT_GRAY_COLOR = 7;
const int DARK_GRAY_COLOR = 8;