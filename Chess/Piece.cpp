#include "Piece.h"
#include "consts.h"

Piece::Piece() : color(NONE), type(EMPTY_SQUARE), code(L" ") {}

Piece::Piece(COLORS color, PIECES type) : color(color), type(type) {
	if (color == WHITE)
		code = WHITE_PIECES[type];
	else if (color == BLACK)
		code = BLACK_PIECES[type];
	else
		code = L" ";
}


COLORS Piece::getColor() const { return color; }
PIECES Piece::getType() const { return type; }
const wchar_t* Piece::getCode() const { return code; }

void Piece::setType(PIECES type) {
	this->type = type;
	if (color == WHITE)
		code = WHITE_PIECES[type];
	else if (color == BLACK)
		code = BLACK_PIECES[type];
}

void Piece::setColor(COLORS color) {
	this->color = color;
	if (type != EMPTY_SQUARE) {
		if (color == WHITE)
			code = WHITE_PIECES[type];
		else if (color == BLACK)
			code = BLACK_PIECES[type];
	}
}

bool Piece::isEmpty() const {
	return color == NONE || type == EMPTY_SQUARE;
}
